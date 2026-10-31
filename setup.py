#!/usr/bin/env python3

import os
from pathlib import Path
import subprocess
import sys

from setuptools import setup, Extension

include_dirs = ['/usr/local/include', '/usr/include', 'c:/bsdconv/include']
library_dirs = ['/usr/local/lib', '/usr/lib', 'c:/bsdconv']

# Homebrew uses different prefixes on Apple Silicon and Intel Macs.
prefix = os.environ.get('BSDCONV_PREFIX')
if prefix is None and sys.platform == 'darwin':
    try:
        prefix = subprocess.check_output(
            ['brew', '--prefix', 'bsdconv'],
            text=True, stderr=subprocess.DEVNULL, timeout=10,
        ).strip() or None
    except (OSError, subprocess.SubprocessError):
        pass
    if prefix is None:
        # Also support builds where Homebrew is not on PATH.
        for candidate in ('/opt/homebrew/opt/bsdconv', '/usr/local/opt/bsdconv',
                          '/opt/homebrew'):
            if (Path(candidate) / 'include' / 'bsdconv.h').is_file():
                prefix = candidate
                break

if prefix:
    include_dirs.insert(0, str(Path(prefix) / 'include'))
    library_dirs.insert(0, str(Path(prefix) / 'lib'))

# Package metadata is defined in pyproject.toml.
setup(
    ext_modules=[
        Extension(
            'bsdconv',
            sources=['bsdconv.c'],
            include_dirs=include_dirs,
            library_dirs=library_dirs,
            libraries=['bsdconv'],
        ),
    ],
)
