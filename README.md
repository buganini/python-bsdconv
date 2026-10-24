# Python support

Supported versions: CPython 3.8–3.14. Python 2 is no longer supported.
The `ctl()` regression tests have been run on CPython 3.12 and 3.14.
Other Python versions and implementations have not been verified.

# Installation

On macOS with Homebrew, install from this checkout:

```sh
brew install bsdconv
python3 -m venv .venv
. .venv/bin/activate
python -m pip install .
```

The build automatically discovers Homebrew's bsdconv headers and library on
Apple Silicon and Intel Macs. On other systems, install libbsdconv and its
development headers before running `python -m pip install .`.

For a custom libbsdconv installation, set `BSDCONV_PREFIX` to the directory
containing its `include` and `lib` directories:

```sh
BSDCONV_PREFIX=/path/to/bsdconv python -m pip install .
```

# Patch reference

The patch submitted in
[FreeBSD Bug 299855](https://bugs.freebsd.org/bugzilla/show_bug.cgi?id=299855)
is a reference for adapting this binding to Python 3 C API changes. It addresses
argument length types (`PY_SSIZE_T_CLEAN` and `Py_ssize_t`), replacement of the
unexported `PyIOBase_Type`, file type initialization, and filesystem path encoding,
including non-UTF-8 filenames. The `ctl()` file ownership and error-handling fixes
in this project extend that patch.

#PyPI
	https://pypi.python.org/pypi/bsdconv

#Documentation
	http://buganini.github.io/bsdconv/python-bsdconv/doc/api.html

	see example/* and gbsdconv for example
