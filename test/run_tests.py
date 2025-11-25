"""Automated Test Runner for VM Project."""

from __future__ import annotations

import argparse
import subprocess
import sys
from dataclasses import dataclass
from enum import Enum, auto
from pathlib import Path
from typing import Iterator

# =============================================================================
# Configuration
# =============================================================================

PROJECT_NAME = "oop_cnu_term_project"
DEFAULT_TIMEOUT = 2
BUILD_DIRS = ("cmake-build-debug", "build", "bin", "")


class Color(Enum):
    """ANSI color codes for terminal output."""

    GREEN = "\033[92m"
    RED = "\033[91m"
    YELLOW = "\033[93m"
    RESET = "\033[0m"

    def __str__(self) -> str:
        return self.value


class TestStatus(Enum):
    """Possible test outcomes."""

    PASSED = auto()
    FAILED = auto()
    TIMEOUT = auto()
    ERROR = auto()


# =============================================================================
# Test Result
# =============================================================================


@dataclass
class TestResult:
    """Result of a single test execution."""

    name: str
    status: TestStatus
    message: str = ""
    expected: str = ""
    actual: str = ""

    @property
    def passed(self) -> bool:
        return self.status == TestStatus.PASSED

    def print(self) -> None:
        """Print the test result."""
        icons = {
            TestStatus.PASSED: f"{Color.GREEN}[PASS]{Color.RESET}",
            TestStatus.FAILED: f"{Color.RED}[FAIL]{Color.RESET}",
            TestStatus.TIMEOUT: f"{Color.RED}[TIME]{Color.RESET}",
            TestStatus.ERROR: f"{Color.RED}[ERR!]{Color.RESET}",
        }
        icon = icons[self.status]
        print(f"{icon} {self.name}", end="")

        if self.message:
            print(f": {self.message}")
        else:
            print()

        if self.status == TestStatus.FAILED and self.expected:
            print(f"   --- Expected ---\n{self.expected}")
            print(f"   --- Actual ---\n{self.actual}")


# =============================================================================
# Test Runner
# =============================================================================


class TestRunner:
    """Discovers and executes tests."""

    def __init__(
        self,
        executable: Path,
        bin_dir: Path,
        answer_dir: Path,
        timeout: int = DEFAULT_TIMEOUT,
    ) -> None:
        self.executable = executable
        self.bin_dir = bin_dir
        self.answer_dir = answer_dir
        self.timeout = timeout

    def discover(self) -> Iterator[tuple[Path, Path]]:
        """Yield (bin_file, answer_file) pairs for valid tests."""
        for bin_file in sorted(self.bin_dir.glob("*.bin")):
            answer_file = self.answer_dir / f"{bin_file.stem}.txt"
            if answer_file.exists():
                yield bin_file, answer_file

    def run_test(self, bin_file: Path, answer_file: Path) -> TestResult:
        """Execute a single test and return the result."""
        test_name = bin_file.stem

        try:
            result = subprocess.run(
                [self.executable, bin_file],
                capture_output=True,
                text=True,
                encoding="utf-8",
                timeout=self.timeout,
                check=False,
            )
        except subprocess.TimeoutExpired:
            return TestResult(test_name, TestStatus.TIMEOUT, f"Exceeded {self.timeout}s")
        except OSError as e:
            return TestResult(test_name, TestStatus.ERROR, str(e))

        if result.returncode != 0:
            msg = f"Exit code {result.returncode}"
            if result.stderr:
                msg += f"\n{result.stderr.strip()}"
            return TestResult(test_name, TestStatus.ERROR, msg)

        actual = result.stdout.replace("\r\n", "\n").strip()
        expected = answer_file.read_text(encoding="utf-8").replace("\r\n", "\n").strip()

        if actual == expected:
            return TestResult(test_name, TestStatus.PASSED)

        return TestResult(test_name, TestStatus.FAILED, "Output mismatch", expected, actual)

    def run_all(self) -> tuple[int, int]:
        """Run all tests and return (passed, total)."""
        passed = 0
        total = 0

        for bin_file, answer_file in self.discover():
            result = self.run_test(bin_file, answer_file)
            result.print()
            total += 1
            if result.passed:
                passed += 1

        return passed, total


# =============================================================================
# Executable Discovery
# =============================================================================


def find_executable(project_root: Path) -> Path | None:
    """Locate the VM executable in common build directories."""
    exe_name = PROJECT_NAME + (".exe" if sys.platform == "win32" else "")

    for build_dir in BUILD_DIRS:
        path = project_root / build_dir / exe_name if build_dir else project_root / exe_name
        if path.is_file():
            return path.resolve()

    return None


# =============================================================================
# CLI
# =============================================================================


def parse_args() -> argparse.Namespace:
    """Parse command line arguments."""
    parser = argparse.ArgumentParser(description="Automated Test Runner for VM Project")
    parser.add_argument("--exe", type=Path, help="Path to the VM executable")
    parser.add_argument("--timeout", type=int, default=DEFAULT_TIMEOUT, help="Timeout per test in seconds")
    return parser.parse_args()


def main() -> int:
    """Main entry point."""
    args = parse_args()

    # Setup paths
    test_dir = Path(__file__).parent.resolve()
    project_root = test_dir.parent
    bin_dir = test_dir / "bin"
    answer_dir = test_dir / "answer"

    # Locate executable
    executable = args.exe or find_executable(project_root)

    if not executable:
        print(f"{Color.RED}Error: Could not find executable.{Color.RESET}")
        print("Build the project first or specify path with --exe")
        return 1

    if not bin_dir.exists():
        print(f"{Color.RED}Error: {bin_dir} does not exist.{Color.RESET}")
        return 1

    # Display configuration
    print(f"Executable : {executable}")
    print(f"Test Dir   : {bin_dir}")
    print(f"Timeout    : {args.timeout}s\n")

    # Run tests
    runner = TestRunner(executable, bin_dir, answer_dir, args.timeout)
    passed, total = runner.run_all()

    # Summary
    print(f"\n{'=' * 50}")
    print(f"RESULT: {passed}/{total} passed")
    print("=" * 50)

    if total > 0 and passed == total:
        print(f"{Color.GREEN}ALL TESTS PASSED!{Color.RESET}")
        return 0

    print(f"{Color.RED}SOME TESTS FAILED.{Color.RESET}")
    return 1


if __name__ == "__main__":
    sys.exit(main())
