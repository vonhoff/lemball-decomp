// Export original-binary function spans for review. Run against LEMBALL.EXE.
// Script argument: output CSV path. Does not modify the Ghidra program.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import java.io.File;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;

public class ExportFunctionSizes extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 1) {
            throw new IllegalArgumentException("Expected one output CSV path");
        }
        File output = new File(args[0]);
        try (PrintWriter writer = new PrintWriter(output, StandardCharsets.UTF_8)) {
            writer.println("# Analysis-derived original function spans; review before use.");
            writer.println("# Program SHA-256: " + currentProgram.getExecutableSHA256());
            writer.println("address,size,evidence");
            FunctionIterator functions = currentProgram.getFunctionManager().getFunctions(true);
            while (functions.hasNext()) {
                Function function = functions.next();
                if (function.isExternal()) {
                    continue;
                }
                AddressSetView body = function.getBody();
                long size = body.getMaxAddress().subtract(function.getEntryPoint()) + 1;
                writer.println("0x" + function.getEntryPoint() + "," + size + ",Ghidra body span");
            }
        }
        println("Exported original function spans to " + output);
    }
}
