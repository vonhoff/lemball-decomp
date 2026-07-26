// Export direct calls between objdiff-owned functions from Ghidra.
// @category Lemmings Paintball

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;

import java.io.File;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

public class ExportFunctionEdges extends GhidraScript {
	private static final String UNIT_TAG = "objdiff-unit:";

	public void run() throws Exception {
		String[] args = getScriptArgs();
		File output = args.length == 0 ? askFile("Save function-edges.csv", "Save") : new File(args[0]);
		List<Function> functions = new ArrayList<>();
		Set<Long> owned = new HashSet<>();
		FunctionIterator iterator = currentProgram.getFunctionManager().getFunctions(true);
		while (iterator.hasNext()) {
			Function function = iterator.next();
			boolean tagged = function.getTags().stream()
				.anyMatch(tag -> tag.getName().startsWith(UNIT_TAG));
			if (tagged) {
				functions.add(function);
				owned.add(function.getEntryPoint().getOffset());
			}
		}

		List<String> lines = new ArrayList<>();
		lines.add("from_address,to_address,kind");
		for (Function function : functions) {
			TreeSet<Long> targets = new TreeSet<>();
			for (Function target : function.getCalledFunctions(monitor)) {
				long address = target.getEntryPoint().getOffset();
				if (owned.contains(address)) {
					targets.add(address);
				}
			}
			for (long target : targets) {
				lines.add(String.format(
					"0x%08X,0x%08X,call",
					function.getEntryPoint().getOffset(),
					target
				));
			}
		}

		Files.write(output.toPath(), lines);
		println("Exported " + (lines.size() - 1) + " function edges to " + output);
	}
}
