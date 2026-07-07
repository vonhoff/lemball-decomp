HOST_CXX ?= c++
HOST_CXXFLAGS ?= -std=c++98 -Wall -Wextra -Werror -Isrc -Isrc/visos
CLANG_FORMAT ?= clang-format
CLANG_TIDY ?= clang-tidy
FORMAT_SOURCES := $(shell rg --files src -g '*.cpp' -g '*.h')
TIDY_SOURCES := $(shell rg --files src -g '*.cpp')

.PHONY: pipeline status verify validate-target ghidra-functions report annotation-candidates format tidy

pipeline: validate-target status

status:
	@mkdir -p build
	@python3 tools/generate_status_report.py --output build/status.json

report:
	@python3 tools/generate_reccmp_report.py -o build/report.json

validate-target:
	@python3 tools/validate_target_binary.py

verify:
	@python3 tools/compare_rebuilt_functions.py

format:
	@$(CLANG_FORMAT) -i $(FORMAT_SOURCES)

tidy:
	@$(CLANG_TIDY) $(TIDY_SOURCES) -- -x c++ -std=c++98 -DLEMBALL_1996_CXX=1 -Isrc -Isrc/visos
