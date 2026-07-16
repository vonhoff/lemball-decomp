// Export the current program's function inventory as deterministic JSON.
// @category LEMBALL

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;

import java.io.BufferedWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class ExportFunctionManifest extends GhidraScript {
    private static String jsonString(String value) {
        StringBuilder out = new StringBuilder("\"");
        for (int i = 0; i < value.length(); i++) {
            char c = value.charAt(i);
            switch (c) {
                case '\\': out.append("\\\\"); break;
                case '"': out.append("\\\""); break;
                case '\n': out.append("\\n"); break;
                case '\r': out.append("\\r"); break;
                case '\t': out.append("\\t"); break;
                default:
                    if (c < 0x20) out.append(String.format("\\u%04X", (int)c));
                    else out.append(c);
            }
        }
        return out.append('"').toString();
    }

    private static String category(Function function) {
        if (function.isThunk()) {
            Function target = function.getThunkedFunction(true);
            return target != null && target.isExternal() ? "import" : "thunk";
        }
        return function.getSymbol().getSource() == SourceType.USER_DEFINED
            ? "internal" : "runtime";
    }

    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) throw new IllegalArgumentException("expected OUTPUT TARGET");
        List<Function> functions = new ArrayList<>();
        currentProgram.getFunctionManager().getFunctions(true).forEach(functions::add);
        Map<String, Integer> counts = new LinkedHashMap<>();
        for (String name : new String[]{"internal", "runtime", "thunk", "import", "external"})
            counts.put(name, 0);
        for (Function function : functions) {
            String type = category(function);
            counts.put(type, counts.get(type) + 1);
        }

        try (BufferedWriter out = Files.newBufferedWriter(Path.of(args[0]), StandardCharsets.UTF_8)) {
            out.write("{\n  \"version\": " + jsonString(args[1]) + ",\n");
            out.write("  \"program\": " + jsonString(currentProgram.getName()) + ",\n");
            out.write("  \"function_count\": " + functions.size() + ",\n  \"summary\": {\n");
            out.write("    \"total\": " + functions.size());
            for (Map.Entry<String, Integer> entry : counts.entrySet())
                out.write(",\n    " + jsonString(entry.getKey()) + ": " + entry.getValue());
            out.write("\n  },\n  \"functions\": [\n");
            for (int i = 0; i < functions.size(); i++) {
                Function function = functions.get(i);
                String type = category(function);
                out.write("    {\n");
                out.write("      \"address\": " + jsonString(String.format("%08X", function.getEntryPoint().getOffset())) + ",\n");
                out.write("      \"name\": " + jsonString(function.getName()) + ",\n");
                out.write("      \"size\": " + function.getBody().getNumAddresses() + ",\n");
                out.write("      \"category\": " + jsonString(type) + ",\n");
                out.write("      \"is_thunk\": " + function.isThunk() + ",\n");
                out.write("      \"is_external\": " + function.isExternal() + ",\n");
                out.write("      \"provenance\": \"ghidra\"\n    }");
                out.write(i + 1 == functions.size() ? "\n" : ",\n");
            }
            out.write("  ]\n}\n");
        }
        println("Exported " + functions.size() + " functions to " + args[0]);
    }
}
