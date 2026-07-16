HOST_CXX ?= c++
HOST_CXXFLAGS ?= -std=c++98 -Wall -Wextra -Werror -Isrc
CLANG_FORMAT ?= clang-format
CLANG_TIDY ?= clang-tidy
FORMAT_SOURCES = $(shell rg --files src -g '*.CPP' -g '*.H')
TIDY_SOURCES = $(shell rg --files src -g '*.CPP')

RECCMP_TARGET ?= LEMBALL
RECCMP_ARGS ?=
RECCMP_REPORT ?= build/reccmp.json
PYTHON ?= python
GHIDRA_PROJECT ?=
GHIDRA_HOME ?=

.PHONY: pipeline verify validate-target ghidra-functions report audit analyze snapshot decomplint reccmp stackcmp roadmap vtable datacmp aggregate format tidy

pipeline: validate-target

report:
	@$(PYTHON) tools/generate_reccmp_report.py

ghidra-functions:
	@$(PYTHON) tools/generate_ghidra_manifest.py $(if $(GHIDRA_PROJECT),--project "$(GHIDRA_PROJECT)",) $(if $(GHIDRA_HOME),--ghidra-home "$(GHIDRA_HOME)",)

validate-target:
	@$(PYTHON) tools/validate_target_binary.py

verify:
	@$(PYTHON) tools/compare_rebuilt_functions.py

# Fast correctness checks suitable for local use and CI.
audit: decomplint vtable datacmp

# Full structural overview; roadmap is informative rather than an assertion.
analyze: audit roadmap

# Save a native reccmp JSON sample for aggregate/diff workflows.
snapshot:
	@$(PYTHON) tools/compare_rebuilt_functions.py --json $(RECCMP_REPORT) --silent $(RECCMP_ARGS)

decomplint:
	@reccmp-decomplint --target $(RECCMP_TARGET) $(RECCMP_ARGS)

reccmp:
	@reccmp-reccmp --target $(RECCMP_TARGET) $(RECCMP_ARGS)

stackcmp:
	@reccmp-stackcmp --target $(RECCMP_TARGET) $(RECCMP_ARGS)

roadmap:
	@reccmp-roadmap --target $(RECCMP_TARGET) $(RECCMP_ARGS)

vtable:
	@reccmp-vtable --target $(RECCMP_TARGET) $(RECCMP_ARGS)

datacmp:
	@reccmp-datacmp --target $(RECCMP_TARGET) $(RECCMP_ARGS)

aggregate:
	@reccmp-aggregate $(RECCMP_ARGS)

format:
	@$(CLANG_FORMAT) -i $(FORMAT_SOURCES)

tidy:
	@$(CLANG_TIDY) $(TIDY_SOURCES) -- -x c++ -std=c++98 -DLEMBALL_1996_CXX=1 -Isrc
