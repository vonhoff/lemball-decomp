HOST_CXX ?= c++
HOST_CXXFLAGS ?= -std=c++98 -Wall -Wextra -Werror -Isrc -Isrc/visos
GHIDRA_PROJECT_DIR ?= ../LemBall_Decompilation
GHIDRA_PROJECT_NAME ?= LEMBALL_DECOMP
GHIDRA_PROGRAM ?= LEMBALL.EXE
GHIDRA_HOME ?= $(HOME)/Applications/Ghidra

.PHONY: pipeline status verify validate-target ghidra-functions report annotation-candidates

pipeline: validate-target status

ghidra-functions:
	@mkdir -p build/ghidra-project build/ghidra data
	@rm -rf build/ghidra-project/$(GHIDRA_PROJECT_NAME).gpr build/ghidra-project/$(GHIDRA_PROJECT_NAME).rep
	@rsync -a --exclude '$(GHIDRA_PROJECT_NAME).lock' --exclude '$(GHIDRA_PROJECT_NAME).lock~' "$(GHIDRA_PROJECT_DIR)/$(GHIDRA_PROJECT_NAME).gpr" "$(GHIDRA_PROJECT_DIR)/$(GHIDRA_PROJECT_NAME).rep" build/ghidra-project/
	@"$(GHIDRA_HOME)/support/analyzeHeadless" "$(CURDIR)/build/ghidra-project" "$(GHIDRA_PROJECT_NAME)" -process "$(GHIDRA_PROGRAM)" -scriptPath "$(CURDIR)/tools" -postScript WriteFunctionExport.java "$(CURDIR)/build/ghidra/LEMBALL.functions.raw.json" -noanalysis
	@python3 tools/generate_function_manifest.py --input build/ghidra/LEMBALL.functions.raw.json --output data/manifest.json

annotation-candidates:
	@python3 tools/generate_annotation_candidates.py --source-root src --output build/annotation-candidates.json

status:
	@mkdir -p build
	@python3 tools/generate_status_report.py --output build/status.json

report:
	@python3 tools/write_progress_report.py -o build/report.json

validate-target:
	@python3 tools/validate_target_binary.py

verify:
	@python3 tools/compare_rebuilt_functions.py
