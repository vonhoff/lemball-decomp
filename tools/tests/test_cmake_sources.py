"""New source files must be discovered by the next ordinary build."""

import pathlib
import shutil
import subprocess
import tempfile
import unittest


class SourceDiscoveryTests(unittest.TestCase):
    def setUp(self):
        root = pathlib.Path(__file__).resolve().parents[2]
        cache = root / "build-msvc400" / "CMakeCache.txt"
        values = {}
        if cache.exists():
            for line in cache.read_text().splitlines():
                if "=" in line and ":" in line.split("=", 1)[0]:
                    key, value = line.split("=", 1)
                    values[key.split(":", 1)[0]] = value
        self.cmake = shutil.which("cmake") or values.get("CMAKE_COMMAND")
        self.make = shutil.which("ninja") or values.get("CMAKE_MAKE_PROGRAM")
        self.generator = "Ninja" if shutil.which("ninja") else values.get("CMAKE_GENERATOR")
        if not self.cmake or not self.make or not self.generator:
            self.skipTest("requires CMake and a build tool, on PATH or in the project cache")

    def test_build_detects_new_cpp_without_manual_reconfigure(self):
        root = pathlib.Path(__file__).resolve().parents[2]
        declaration = next(line for line in (root / "CMakeLists.txt").read_text().splitlines()
                           if line.startswith("file(GLOB_RECURSE LEMBALL_SOURCES "))
        self.check_discovery(declaration.replace(" CONFIGURE_DEPENDS", ""), False)
        self.check_discovery(declaration, True)

    def check_discovery(self, declaration, expected):
        with tempfile.TemporaryDirectory() as temporary:
            source = pathlib.Path(temporary)
            (source / "src").mkdir()
            (source / "src" / "First.cpp").write_text("// first\n")
            (source / "CMakeLists.txt").write_text(
                "cmake_minimum_required(VERSION 3.15)\nproject(discovery NONE)\n"
                + declaration + '\nfile(WRITE "${CMAKE_BINARY_DIR}/sources.txt" "${LEMBALL_SOURCES}")\n'
            )
            build = source / "build"
            self.run_cmake("-S", str(source), "-B", str(build), "-G", self.generator,
                           "-DCMAKE_MAKE_PROGRAM=" + self.make)
            self.assertNotIn("Added.cpp", (build / "sources.txt").read_text())
            (source / "src" / "Added.cpp").write_text("// newly reconstructed source\n")
            self.run_cmake("--build", str(build))
            self.assertEqual("Added.cpp" in (build / "sources.txt").read_text(), expected)

    def run_cmake(self, *arguments):
        result = subprocess.run([self.cmake, *arguments], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
