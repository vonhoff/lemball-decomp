// Export objdiff function inventory from Ghidra.
// @category Lemmings Paintball

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;

import java.io.File;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.List;

public class ExportObjdiffFunctions extends GhidraScript {
	private static final String UNIT_TAG = "objdiff-unit:";

	public void run() throws Exception {
		String[] args = getScriptArgs();
		File output = args.length == 0 ? askFile("Save objdiff-functions.csv", "Save") : new File(args[0]);
		List<String> lines = new ArrayList<>();
		lines.add("address,size,name,unit");

		FunctionIterator functions = currentProgram.getFunctionManager().getFunctions(true);
		while (functions.hasNext()) {
			Function function = functions.next();
			List<String> units = function.getTags().stream()
				.map(tag -> tag.getName())
				.filter(name -> name.startsWith(UNIT_TAG))
				.toList();
			if (units.isEmpty()) {
				continue;
			}
			if (units.size() != 1) {
				throw new IllegalArgumentException(function.getName() + " has multiple objdiff unit tags");
			}
			Instruction instruction = currentProgram.getListing().getInstructionAt(function.getEntryPoint());
			if (instruction == null ||
				"INT3".equals(instruction.getMnemonicString()) ||
				!function.getBody().contains(instruction.getMinAddress(), instruction.getMaxAddress()) ||
				(instruction.getFallThrough() != null &&
					!function.getBody().contains(instruction.getFallThrough()))) {
				throw new IllegalArgumentException("Invalid function body: " + function.getName());
			}
			String name = function.getName();
			if (!name.matches("[a-z][a-z0-9_]{0,63}")) {
				throw new IllegalArgumentException("Invalid function name: " + name);
			}
			if (name.matches("(?:.*_)?(?:fun|unknown|method|reserved[0-9a-f]*)(?:_.*)?")) {
				throw new IllegalArgumentException("Vague function name: " + name);
			}
			lines.add(String.format(
				"0x%08X,%d,%s,%s",
				function.getEntryPoint().getOffset(),
				function.getBody().getNumAddresses(),
				name,
				units.get(0).substring(UNIT_TAG.length())
			));
		}

		Files.write(output.toPath(), lines);
		println("Exported " + (lines.size() - 1) + " functions to " + output);
	}
}
