#!/bin/sh
set -eu

VERSION="${VERSION:-LEMBALL}"
TARGET_DIR="build/target/${VERSION}"
MANIFEST="manifests/target_slices/${VERSION}.txt"
WATCOM_ASM="${WATCOM_ASM:-wasm}"
DOCKER_IMAGE="${DOCKER_IMAGE:-lemball-openwatcom}"
OBJECT="${OBJECT:-}"
UNIT_ARG=""

mkdir -p "${TARGET_DIR}"

if [ -n "${LEMBALL_EXE:-}" ] && [ -f "${LEMBALL_EXE}" ]; then
    :
elif [ -f "orig/LEMBALL/LEMBALL.EXE" ]; then
    LEMBALL_EXE="orig/LEMBALL/LEMBALL.EXE"
elif [ -f "../LemBall_Decompilation/LEMBALL/LEMBALL.EXE" ]; then
    LEMBALL_EXE="../LemBall_Decompilation/LEMBALL/LEMBALL.EXE"
elif [ -f "../OpenPaintball/reference/LEMBALL.EXE" ]; then
    LEMBALL_EXE="../OpenPaintball/reference/LEMBALL.EXE"
else
    echo "LEMBALL.EXE not found. Set LEMBALL_EXE to the real executable path." >&2
    exit 1
fi

if [ ! -f "${MANIFEST}" ]; then
    echo "target slice manifest not found: ${MANIFEST}" >&2
    exit 1
fi

if command -v "${WATCOM_ASM}" >/dev/null 2>&1; then
    :
elif command -v docker >/dev/null 2>&1; then
    EXE_DIR=$(cd "$(dirname "${LEMBALL_EXE}")" && pwd)
    EXE_NAME=$(basename "${LEMBALL_EXE}")

    docker build -t "${DOCKER_IMAGE}" toolchains/openwatcom >/dev/null

    exec docker run --rm \
        -v "$(pwd)":/work \
        -v "${EXE_DIR}":/input-lemball:ro \
        -w /work \
        -e VERSION="${VERSION}" \
        -e LEMBALL_EXE="/input-lemball/${EXE_NAME}" \
        -e WATCOM_ASM="${WATCOM_ASM}" \
        "${DOCKER_IMAGE}" \
        sh -lc "./scripts/build_target_objects.sh"
else
    echo "Open Watcom assembler not found (${WATCOM_ASM})." >&2
    echo "Try this on a machine with Open Watcom installed, or with Docker available for the prepared container path." >&2
    exit 1
fi

if [ -n "${OBJECT}" ]; then
    UNIT_ARG="--unit ${OBJECT}"
fi

python3 tools/build_target_objects.py \
    --exe "${LEMBALL_EXE}" \
    --manifest "${MANIFEST}" \
    --outdir "${TARGET_DIR}" \
    --assembler "${WATCOM_ASM}" \
    ${UNIT_ARG}
