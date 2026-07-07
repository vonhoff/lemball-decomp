#!/bin/sh
set -eu

VERSION="${VERSION:-LEMBALL}"
OUTDIR="build/base/${VERSION}"
WATCOM_CXX="${WATCOM_CXX:-wpp386}"
WATCOM_DISASM="${WATCOM_DISASM:-wdis}"
WATCOM_PROFILE="${WATCOM_PROFILE:-toolchains/openwatcom/wpp386-byteprobe.flags}"
DOCKER_IMAGE="${DOCKER_IMAGE:-lemball-openwatcom}"
PROFILE_FLAGS=""
OBJECT="${OBJECT:-}"

if [ -f "${WATCOM_PROFILE}" ]; then
    PROFILE_FLAGS=$(grep -v '^[[:space:]]*#' "${WATCOM_PROFILE}" | grep -v '^[[:space:]]*$' | tr '\n' ' ')
fi

if command -v "${WATCOM_CXX}" >/dev/null 2>&1; then
    :
elif command -v docker >/dev/null 2>&1; then
    if ! docker image inspect "${DOCKER_IMAGE}" >/dev/null 2>&1; then
        docker build -t "${DOCKER_IMAGE}" toolchains/openwatcom >/dev/null
    fi

    exec docker run --rm \
        -v "$(pwd)":/work \
        -w /work \
        -e VERSION="${VERSION}" \
        -e WATCOM_CXX="${WATCOM_CXX}" \
        "${DOCKER_IMAGE}" \
        sh -lc "./scripts/check_watcom_profile.sh"
else
    echo "Open Watcom compiler not found (${WATCOM_CXX})." >&2
    echo "Try this on a machine with Open Watcom installed, or with Docker available for the prepared container path." >&2
    exit 1
fi

mkdir -p "${OUTDIR}"
mkdir -p "${OUTDIR}/disasm"

if [ -z "${OBJECT}" ]; then
    rm -f "${OUTDIR}"/*.obj
    rm -f "${OUTDIR}/disasm"/*.lst
fi

BUILT=0

for src in $(find LEMBALL -type f \( -name '*.CPP' -o -name '*.cpp' \) | sort)
do
    base_name=$(basename "${src}" .CPP)
    base_name=$(basename "${base_name}" .cpp)

    if [ -n "${OBJECT}" ] && [ "${base_name}" != "${OBJECT}" ]; then
        continue
    fi

    "${WATCOM_CXX}" \
        -q \
        ${PROFILE_FLAGS} \
        -i=LEMBALL \
        -i=LEMBALL/VISOS \
        -fo="${OUTDIR}/${base_name}.obj" \
        "${src}"

    if command -v "${WATCOM_DISASM}" >/dev/null 2>&1; then
        "${WATCOM_DISASM}" -l="${OUTDIR}/disasm/${base_name}.lst" "${OUTDIR}/${base_name}.obj"
    fi

    BUILT=1
done

if [ -n "${OBJECT}" ] && [ "${BUILT}" -eq 0 ]; then
    echo "source for object ${OBJECT} not found under LEMBALL/" >&2
    exit 1
fi
