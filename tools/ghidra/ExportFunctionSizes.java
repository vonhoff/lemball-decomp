// Export original-binary function spans for review. Run against LEMBALL.EXE.
// Script argument: output CSV path. Does not modify the Ghidra program.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.symbol.Reference;
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
                Address entry = function.getEntryPoint();
                Address end = body.getMaxAddress();
                Function next = getFunctionAfter(entry);
                Listing listing = currentProgram.getListing();
                InstructionIterator instructions = listing.getInstructions(body, true);
                // Switch tables are data; Ghidra excludes them from the function body.
                while (instructions.hasNext()) {
                    Instruction instruction = instructions.next();
                    for (Reference reference : instruction.getReferencesFrom()) {
                        Address cursor = reference.getToAddress();
                        if (!reference.getReferenceType().isData()
                                || !cursor.getAddressSpace().equals(entry.getAddressSpace())
                                || cursor.compareTo(entry) < 0
                                || next == null || cursor.compareTo(next.getEntryPoint()) >= 0) {
                            continue;
                        }
                        Data data = listing.getDefinedDataAt(cursor);
                        while (data != null && data.getMaxAddress().compareTo(next.getEntryPoint()) < 0) {
                            if (data.getMaxAddress().compareTo(end) > 0) {
                                end = data.getMaxAddress();
                            }
                            cursor = data.getMaxAddress().next();
                            data = cursor == null ? null : listing.getDefinedDataAt(cursor);
                        }
                    }
                }
                long size = end.subtract(entry) + 1;
                writer.println("0x" + entry + "," + size + ",Ghidra body and local data span");
            }
        }
        println("Exported original function spans to " + output);
    }
}
