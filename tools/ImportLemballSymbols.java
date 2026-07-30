// Import the reviewed LEMBALL symbol manifest into a clean Ghidra project.
// @category Lemmings Paintball

import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.Namespace;
import ghidra.program.model.symbol.SourceType;

import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ImportLemballSymbols extends GhidraScript {
	private static final String EXPECTED_SHA256 = "d6337b58ccaf98df728b1490812cad0f927802d2e2c5fc932d00961f97027f63";
	private static final String UNIT_TAG = "objdiff-unit:";
	private static final String ILT_TAG = "linker-ilt";
	private static final long ILT_START = 0x00401000L;
	private static final long ILT_END = 0x0040388cL;

	private static Map<String, String> parse(String header, String line) {
		String[] names = header.split(",", -1);
		String[] values = line.split(",", -1);
		Map<String, String> row = new HashMap<>();
		for (int i = 0; i < names.length; i++) row.put(names[i], i < values.length ? values[i] : "");
		return row;
	}

	private List<String> tags(Function function) {
		return function.getTags().stream().map(tag -> tag.getName()).toList();
	}

	private boolean isDefaultName(String name) {
		return name.startsWith("FUN_") || name.startsWith("thunk_FUN_");
	}

	private boolean validIlt(Address address) throws Exception {
		long offset = address.getOffset();
		MemoryBlock block = currentProgram.getMemory().getBlock(address);
		if (offset < ILT_START || offset > ILT_END || (offset - ILT_START) % 5 != 0 ||
			block == null || !block.isExecute() || !block.contains(address.add(4)) ||
			(getByte(address) & 0xff) != 0xe9) return false;
		Address target = address.add(5L + getInt(address.add(1)));
		MemoryBlock targetBlock = currentProgram.getMemory().getBlock(target);
		return targetBlock != null && targetBlock.isExecute();
	}

	public void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length < 2) throw new IllegalArgumentException("manifest.csv report.csv [apply]");
		boolean apply = args.length >= 3 && "apply".equals(args[2]);
		if (!EXPECTED_SHA256.equalsIgnoreCase(currentProgram.getExecutableSHA256()))
			throw new IllegalArgumentException("Wrong executable SHA-256: " + currentProgram.getExecutableSHA256());

		List<String> input = Files.readAllLines(Path.of(args[0]));
		List<String> report = new ArrayList<>();
		report.add("address,kind,status,name,detail");
		int renamed = 0, tagged = 0, missing = 0, ilts = 0, conflicts = 0;
		int pendingRenames = 0, pendingTags = 0;
		String header = input.get(0);
		Map<String, Namespace> namespaces = new HashMap<>();

		int blockers = 0;
		for (int index = 1; index < input.size(); index++) {
			Map<String, String> row = parse(header, input.get(index));
			Address address = toAddr(Long.decode(row.get("address")));
			String kind = row.get("kind"), name = row.get("name"), unit = row.get("unit");
			Function function = currentProgram.getFunctionManager().getFunctionAt(address);
			boolean isIlt = "linker-ilt".equals(kind);
			if (isIlt && !validIlt(address)) { blockers++; continue; }
			if (function == null) {
				if (!isIlt) { blockers++; continue; }
				for (int i = 0; i < 5; i++) {
					if (currentProgram.getFunctionManager().getFunctionContaining(address.add(i)) != null) blockers++;
				}
				continue;
			}
			if (!isDefaultName(function.getName()) && !name.equals(function.getName())) blockers++;
			List<String> tags = tags(function);
			List<String> units = tags.stream().filter(tag -> tag.startsWith(UNIT_TAG)).toList();
			if (isIlt) {
				if (!units.isEmpty()) blockers++;
			}
			else if (!unit.isEmpty()) {
				if (tags.contains(ILT_TAG) || units.size() > 1 ||
					(units.size() == 1 && !units.get(0).equals(UNIT_TAG + unit))) blockers++;
			}
			else if (tags.contains(ILT_TAG) || !units.isEmpty()) blockers++;
		}
		if (apply && blockers != 0) throw new IllegalStateException("refusing apply with " + blockers + " preflight blockers");

		int transaction = apply ? currentProgram.startTransaction("Import reviewed LEMBALL symbols") : -1;
		boolean commit = false;
		try {
		for (int index = 1; index < input.size(); index++) {
			Map<String, String> row = parse(header, input.get(index));
			Address address = toAddr(Long.decode(row.get("address")));
			String kind = row.get("kind"), name = row.get("name"), unit = row.get("unit");
			Function function = currentProgram.getFunctionManager().getFunctionAt(address);
			boolean rowPending = false;
			if ("linker-ilt".equals(kind)) {
				if (!validIlt(address)) throw new IllegalArgumentException("Invalid ILT at " + address);
				if (function == null && apply) {
					AddressSet body = new AddressSet(address, address.add(4));
					CreateFunctionCmd command = new CreateFunctionCmd(null, address, body, SourceType.USER_DEFINED);
					if (!command.applyTo(currentProgram)) throw new IllegalArgumentException("Cannot create ILT at " + address);
					function = currentProgram.getFunctionManager().getFunctionAt(address);
				}
				if (function == null) { missing++; report.add(address + ",linker-ilt,missing,,"); continue; }
				if (!tags(function).contains(ILT_TAG)) {
					if (apply) function.addTag(ILT_TAG); else { pendingTags++; rowPending = true; }
				}
				ilts++;
			}
			else if (function == null) {
				Function containing = currentProgram.getFunctionManager().getFunctionContaining(address);
				String detail = containing == null ? "" : "contained_by=" + containing.getEntryPoint();
				missing++; report.add(address + ",function,missing," + name + "," + detail); continue;
			}

			String currentName = function == null ? "" : function.getName();
			boolean currentNameIsDefault = function != null && isDefaultName(currentName);
			boolean mayRename = currentNameIsDefault || name.equals(currentName);
			String namespaceName = row.get("namespace");
			boolean namespaceMismatch = function != null && (namespaceName.isEmpty()
				? !function.getParentNamespace().isGlobal()
				: !namespaceName.equals(function.getParentNamespace().getName()));
			if (!mayRename) {
				conflicts++; report.add(address + "," + kind + ",conflict," + name + ",current=" + currentName); continue;
			}
			if (function != null && mayRename && !name.isEmpty() &&
				(!name.equals(function.getName()) || namespaceMismatch)) {
				if (!apply) {
					pendingRenames++; rowPending = true;
				}
				else {
					if (namespaceMismatch) {
						Namespace namespace = namespaceName.isEmpty() ? currentProgram.getGlobalNamespace() : namespaces.get(namespaceName);
						if (namespace == null) {
							namespace = currentProgram.getSymbolTable().getNamespace(namespaceName, currentProgram.getGlobalNamespace());
							if (namespace == null) namespace = currentProgram.getSymbolTable().createNameSpace(currentProgram.getGlobalNamespace(), namespaceName, SourceType.USER_DEFINED);
							namespaces.put(namespaceName, namespace);
						}
						function.setParentNamespace(namespace);
					}
					if (!name.equals(function.getName())) function.setName(name, SourceType.USER_DEFINED);
					renamed++;
				}
			}
			if (function != null && !unit.isEmpty()) {
				String expectedTag = UNIT_TAG + unit;
				if (!tags(function).contains(expectedTag)) {
					if (apply) function.addTag(expectedTag); else { pendingTags++; rowPending = true; }
				}
				tagged++;
			}
			report.add(address + "," + kind + "," + (rowPending ? "pending" : "ok") + "," + name + ",size=" + function.getBody().getNumAddresses());
		}
		commit = true;
		Files.write(Path.of(args[1]), report);
		println("apply=" + apply + " renamed=" + renamed + " tagged=" + tagged + " ilt=" + ilts + " missing=" + missing + " conflicts=" + conflicts);
		println("pending_renames=" + pendingRenames + " pending_tags=" + pendingTags);
		}
		finally {
			if (transaction != -1) currentProgram.endTransaction(transaction, commit);
		}
	}
}
