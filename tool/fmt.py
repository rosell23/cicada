#!/usr/bin/env python3

# Copyright (c) 2025 The Cicada Project. All rights reserved.
#
# THE WORK IS PROVIDED “AS IS”, WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN CONNECTION
# WITH THE WORK OR ITS USE.

import os
import subprocess
import sys
from enum import Enum


class Colors(Enum):
    RESET = "\x1b[0m"
    RED = "\x1b[31m"
    GREEN = "\x1b[32m"


def main():
    print(f"{Colors.GREEN.value}==> Checking for format files.{Colors.RESET.value}")

    if not os.path.exists(".clang-format"):
        print(f"{Colors.RED.value}==> ERROR: Didn't find .clang-format file.{Colors.RESET.value}")
        print("Are you in the right directory?")
        sys.exit(1)

    print(f"{Colors.GREEN.value}==> Found format file; searching for source files...{Colors.RESET.value}")

    # Find all C/C++ source files
    try:
        output = subprocess.check_output(
            ["find", ".", "-type", "f", "-name", "*.c", "-o", "-name", "*.cpp", "-o", "-name", "*.h"],
            text=True
        )
    except subprocess.CalledProcessError:
        print(f"{Colors.RED.value}==> ERROR: Find wasn't successful.{Colors.RESET.value}")
        sys.exit(1)

    srcs = [line.strip() for line in output.splitlines() if line.strip()]
    if not srcs:
        print(f"{Colors.RED.value}==> No source files found to format.{Colors.RESET.value}")
        sys.exit(1)

    print(f"{Colors.GREEN.value}==> Formatting {len(srcs)} file(s)...{Colors.RESET.value}")

    for src in srcs:
        subprocess.run(["clang-format", "-i", src])

    print(f"{Colors.GREEN.value}==> Done formatting.{Colors.RESET.value}")


if __name__ == "__main__":
    main()
