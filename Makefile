VERSION ?= LEMBALL
HOST_CXX ?= c++
HOST_CXXFLAGS ?= -std=c++98 -Wall -Wextra -Werror -ILEMBALL -ILEMBALL/VISOS

.PHONY: build-target build-base objdiff-config pipeline status host-check watcom-check verify validate-target

build-target:
	@mkdir -p build/target/$(VERSION)
	@if [ -x scripts/build_target_objects.sh ]; then \
		VERSION="$(VERSION)" scripts/build_target_objects.sh; \
	fi

build/target/$(VERSION)/%.obj:
	@mkdir -p build/target/$(VERSION)
	@VERSION="$(VERSION)" OBJECT="$*" scripts/build_target_objects.sh

build-base:
	@mkdir -p build/base/$(VERSION)
	@VERSION="$(VERSION)" scripts/check_watcom_profile.sh

build/base/$(VERSION)/%.obj:
	@mkdir -p build/base/$(VERSION)
	@VERSION="$(VERSION)" OBJECT="$*" scripts/check_watcom_profile.sh

objdiff-config:
	@python3 tools/generate_objdiff.py --version "$(VERSION)" --output objdiff.json

pipeline: validate-target build-base build-target objdiff-config status

status:
	@mkdir -p build
	@python3 tools/generate_objdiff.py --version "$(VERSION)" --output objdiff.json
	@python3 tools/generate_status_report.py --version "$(VERSION)" --output build/status.json

host-check:
	@scripts/check_host_sources.sh "$(HOST_CXX)" "$(HOST_CXXFLAGS)"

watcom-check:
	@scripts/check_watcom_profile.sh

validate-target:
	@python3 tools/validate_target_binary.py --version "$(VERSION)"

verify:
	@tools/verify
