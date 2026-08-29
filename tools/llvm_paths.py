#!/usr/bin/env python3
"""Resolve the LLVM build and matching source tree."""

import os
from pathlib import Path


def llvm_build_root() -> Path:
    value = os.environ.get("LLVM_BUILD")
    if not value:
        raise RuntimeError("LLVM_BUILD is required")
    return Path(value).resolve()


def llvm_project_root() -> Path:
    cache = llvm_build_root() / "CMakeCache.txt"
    try:
        lines = cache.read_text(encoding="utf-8").splitlines()
    except OSError as error:
        raise RuntimeError(f"cannot read {cache}: {error}") from error

    prefix = "LLVM_SOURCE_DIR:STATIC="
    for line in lines:
        if line.startswith(prefix):
            source = Path(line.removeprefix(prefix)).resolve()
            return source.parent if source.name == "llvm" else source
    raise RuntimeError(f"LLVM_SOURCE_DIR is missing from {cache}")


if __name__ == "__main__":
    print(llvm_project_root())
