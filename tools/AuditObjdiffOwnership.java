// Report objdiff placement conflicts and high-confidence callback names.
// @category Lemmings Paintball

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.symbol.Reference;

import java.io.File;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

public class AuditObjdiffOwnership extends GhidraScript {
	private static final String UNIT_TAG = "objdiff-unit:";

	private static class Entry {
		Function function;
		String kind;

		Entry(Function function, String kind) {
			this.function = function;
			this.kind = kind;
		}
	}

	private static class Table {
		Address base;
		List<Entry> entries;
		Set<String> owners = new HashSet<>();

		Table(Address base, List<Entry> entries) {
			this.base = base;
			this.entries = entries;
		}
	}

	private final Map<Address, String> findings = new TreeMap<>();
	private final Map<Address, Table> vtables = new LinkedHashMap<>();
	private final Set<Function> provisionalCallbacks = new HashSet<>();
	private final Set<Function> namedCallbacks = new HashSet<>();
	private int shared;
	private int thunks;

	private String unit(Function function) {
		if (function == null) {
			return null;
		}
		return function.getTags().stream()
			.map(tag -> tag.getName())
			.filter(name -> name.startsWith(UNIT_TAG))
			.map(name -> name.substring(UNIT_TAG.length()))
			.findFirst().orElse(null);
	}

	private Entry resolve(Address address) {
		Function function = currentProgram.getFunctionManager().getFunctionAt(address);
		if (function != null) {
			Instruction first = currentProgram.getListing().getInstructionAt(address);
			if (first != null && first.getFlowType().isJump() && first.getFlows().length == 1) {
				Function target = currentProgram.getFunctionManager().getFunctionAt(first.getFlows()[0]);
				if (target != null && target != function) {
					return new Entry(target, "adjustor_thunk");
				}
			}
			return new Entry(function, "direct");
		}
		Instruction instruction = currentProgram.getListing().getInstructionAt(address);
		if (instruction != null && instruction.getFlowType().isJump() && instruction.getFlows().length == 1) {
			function = currentProgram.getFunctionManager().getFunctionAt(instruction.getFlows()[0]);
			if (function != null) {
				return new Entry(function, "adjustor_thunk");
			}
		}
		return null;
	}

	private String context(Function function, String candidateUnit) {
		Set<Function> callers = new HashSet<>();
		Set<Address> sharedGlobals = new HashSet<>();
		for (Reference reference : getReferencesTo(function.getEntryPoint())) {
			Function caller = currentProgram.getFunctionManager().getFunctionContaining(reference.getFromAddress());
			if (caller != null) {
				callers.add(caller);
			}
		}
		for (Instruction instruction : currentProgram.getListing().getInstructions(function.getBody(), true)) {
			for (Reference reference : instruction.getReferencesFrom()) {
				Address target = reference.getToAddress();
				if (!target.isMemoryAddress() || currentProgram.getMemory().getBlock(target) == null ||
					currentProgram.getMemory().getBlock(target).isExecute()) {
					continue;
				}
				for (Reference use : getReferencesTo(target)) {
					Function owner = currentProgram.getFunctionManager().getFunctionContaining(use.getFromAddress());
					if (owner != null && owner != function) {
						sharedGlobals.add(target);
						break;
					}
				}
			}
		}
		int nearby = 0;
		int matching = 0;
		long address = function.getEntryPoint().getOffset();
		for (Function neighbor : currentProgram.getFunctionManager().getFunctions(true)) {
			if (neighbor != function && Math.abs(neighbor.getEntryPoint().getOffset() - address) <= 0x1000) {
				String neighborUnit = unit(neighbor);
				if (neighborUnit != null) {
					nearby++;
					if (neighborUnit.equals(candidateUnit)) {
						matching++;
					}
				}
			}
		}
		return ";callers:" + callers.size() + ";shared_globals:" + sharedGlobals.size() +
			";address_cluster:" + matching + "/" + nearby;
	}

	private void addPlacement(Function function, String candidate, String confidence, String evidence,
							  String className, Address vtable, Integer slot) {
		String current = unit(function);
		if (current == null || candidate == null || current.equals(candidate)) {
			return;
		}
		findings.putIfAbsent(function.getEntryPoint(), row("placement", function.getEntryPoint(),
			function.getName(), current, candidate, className, vtable, slot, confidence, evidence));
	}

	private String row(String kind, Address address, String name, String currentUnit, String candidateUnit,
					   String className, Address vtable, Integer slot, String confidence, String evidence) {
		return String.join(",",
			kind,
			address.toString(),
			name,
			value(currentUnit),
			value(candidateUnit),
			value(className),
			vtable == null ? "" : vtable.toString(),
			slot == null ? "" : Integer.toString(slot),
			confidence,
			evidence);
	}

	private String value(String text) {
		return text == null ? "" : text;
	}

	private void auditSourceStrings(Map<String, String> unitsByFile) {
		for (Data data : currentProgram.getListing().getDefinedData(true)) {
			Object value = data.getValue();
			if (!(value instanceof String text) || !text.toUpperCase().matches(".*[A-Z0-9_]+\\.CPP")) {
				continue;
			}
			String file = text.replace('\\', '/');
			file = file.substring(file.lastIndexOf('/') + 1).toUpperCase();
			String candidate = unitsByFile.get(file);
			boolean referenced = false;
			for (Reference reference : getReferencesTo(data.getAddress())) {
				Function owner = currentProgram.getFunctionManager().getFunctionContaining(reference.getFromAddress());
				if (owner != null) {
					referenced = true;
					addPlacement(owner, candidate, "proven", "source_string:" + file, null, null, null);
				}
			}
			if (referenced && candidate == null) {
				println("Missing tagged unit for referenced source name: " + file);
			}
		}
	}

	private void findVtable(Address base, Function owner) {
		Table known = vtables.get(base);
		if (known != null) {
			known.owners.add(owner.getName());
			return;
		}
		Memory memory = currentProgram.getMemory();
		List<Entry> entries = new ArrayList<>();
		for (int slot = 0; slot < 128; slot++) {
			Entry entry;
			try {
				entry = resolve(toAddr(Integer.toUnsignedLong(memory.getInt(base.add(slot * 4L)))));
			}
			catch (Exception exception) {
				break;
			}
			if (entry == null) {
				break;
			}
			entries.add(entry);
		}
		if (entries.size() >= 3) {
			Table table = new Table(base, entries);
			table.owners.add(owner.getName());
			vtables.put(base, table);
		}
	}

	private String className(Table table) {
		Set<String> classes = new HashSet<>();
		for (String owner : table.owners) {
			if (owner.startsWith("construct_")) {
				classes.add(owner.substring("construct_".length()).replaceFirst("_fields$", ""));
			}
		}
		return classes.size() == 1 ? classes.iterator().next() : null;
	}

	private String callbackName(Function function, String className, int slot) {
		String name = function.getName();
		String behavior = null;
		if (name.startsWith("noop_") || name.startsWith("noop_callback_")) {
			behavior = "noop";
		}
		else if (name.startsWith("return_zero_") || name.startsWith("return_false_")) {
			behavior = "return_zero";
		}
		else if (name.startsWith("return_one_") || name.startsWith("return_true_")) {
			behavior = "return_one";
		}
		else if (name.startsWith("return_two_")) {
			behavior = "return_two";
		}
		if (behavior == null || !name.matches(".*_[0-9a-fA-F]{8}$")) {
			return null;
		}
		provisionalCallbacks.add(function);
		String candidate = behavior + "_" + className + "_slot_" + slot;
		return candidate.length() <= 64 ? candidate : null;
	}

	private void auditVtables() {
		Map<Function, Set<Address>> appearances = new HashMap<>();
		for (Table table : vtables.values()) {
			for (Entry entry : table.entries) {
				appearances.computeIfAbsent(entry.function, ignored -> new HashSet<>()).add(table.base);
			}
		}

		for (Table table : vtables.values()) {
			Map<String, Integer> votes = new HashMap<>();
			int tagged = 0;
			for (Entry entry : table.entries) {
				String memberUnit = unit(entry.function);
				if (memberUnit != null) {
					tagged++;
					votes.merge(memberUnit, 1, Integer::sum);
				}
			}
			Map.Entry<String, Integer> winner = votes.entrySet().stream()
				.max(Map.Entry.comparingByValue()).orElse(null);
			String className = className(table);
			for (int slot = 0; slot < table.entries.size(); slot++) {
				Entry entry = table.entries.get(slot);
				if (entry.function.getName().matches(".*_[0-9a-fA-F]{8}$") &&
					entry.function.getName().matches("(?:noop|return_(?:zero|false|one|true|two)).*")) {
					provisionalCallbacks.add(entry.function);
				}
				if (appearances.get(entry.function).size() != 1) {
					shared++;
					continue;
				}
				if (!entry.kind.equals("direct")) {
					thunks++;
					continue;
				}
				if (entry.function.getName().contains("thunk")) {
					thunks++;
					continue;
				}
				if (winner != null && winner.getValue() >= 3 && winner.getValue() * 3 >= tagged * 2 &&
					entry.function.getBody().getNumAddresses() > 5) {
					addPlacement(entry.function, winner.getKey(), "review",
						"unique_vtable;unit_majority:" + winner.getValue() + "/" + tagged +
							context(entry.function, winner.getKey()),
						className, table.base, slot);
				}
				if (className != null) {
					String candidate = callbackName(entry.function, className, slot);
					if (candidate != null && !candidate.equals(entry.function.getName())) {
						namedCallbacks.add(entry.function);
						findings.putIfAbsent(entry.function.getEntryPoint(), row("rename",
							entry.function.getEntryPoint(), candidate, unit(entry.function), unit(entry.function),
							className, table.base, slot, "strong",
							"current_name:" + entry.function.getName() + ";unique_constructor_vtable;behavior"));
					}
				}
			}
		}
	}

	public void run() throws Exception {
		String[] args = getScriptArgs();
		File output = args.length == 0 ? askFile("Save ownership audit", "Save") : new File(args[0]);
		Map<String, String> unitsByFile = new LinkedHashMap<>();
		for (Function function : currentProgram.getFunctionManager().getFunctions(true)) {
			String functionUnit = unit(function);
			if (functionUnit != null) {
				String file = functionUnit.substring(functionUnit.lastIndexOf('/') + 1).toUpperCase();
				unitsByFile.putIfAbsent(file, functionUnit);
			}
		}
		auditSourceStrings(unitsByFile);

		for (Function owner : currentProgram.getFunctionManager().getFunctions(true)) {
			if (!owner.getName().matches("(?:construct|initialize|destroy|delete)_.*")) {
				continue;
			}
			for (Instruction instruction : currentProgram.getListing().getInstructions(owner.getBody(), true)) {
				for (Reference reference : instruction.getReferencesFrom()) {
					Address target = reference.getToAddress();
					if (target.isMemoryAddress() && currentProgram.getMemory().getBlock(target) != null &&
						!currentProgram.getMemory().getBlock(target).isExecute()) {
						findVtable(target, owner);
					}
				}
			}
		}
		auditVtables();

		List<String> lines = new ArrayList<>();
		lines.add("kind,address,name,current_unit,candidate_unit,class_name,vtable,slot,confidence,evidence");
		lines.addAll(findings.values());
		Files.write(output.toPath(), lines);
		println("Wrote " + findings.size() + " actionable findings to " + output);
		println("Unresolved shared vtable memberships: " + shared);
		println("Unresolved adjustor thunk entries: " + thunks);
		println("Unresolved address-suffixed callbacks: " +
			(provisionalCallbacks.size() - namedCallbacks.size()));
	}
}
