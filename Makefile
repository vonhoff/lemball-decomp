HOST_CXX ?= c++
HOST_CXXFLAGS ?= -std=c++98 -Wall -Wextra -Werror -Isrc
CLANG_FORMAT ?= clang-format
CLANG_TIDY ?= clang-tidy
FORMAT_SOURCES = $(shell rg --files src -g '*.CPP' -g '*.H')
TIDY_SOURCES = $(shell rg --files src -g '*.CPP')

.PHONY: pipeline verify validate-target ghidra-functions report annotation-candidates format tidy

pipeline: validate-target

report:
	@python3 tools/generate_reccmp_report.py

validate-target:
	@python3 tools/validate_target_binary.py

verify:
	@python3 tools/compare_rebuilt_functions.py

format:
	@$(CLANG_FORMAT) -i $(FORMAT_SOURCES)

tidy:
	@$(CLANG_TIDY) $(TIDY_SOURCES) -- -x c++ -std=c++98 -DLEMBALL_1996_CXX=1 -Isrc
