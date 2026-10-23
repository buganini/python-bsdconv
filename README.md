# Python support

Supported versions: CPython 3.8–3.14. Python 2 is no longer supported.
The `ctl()` regression tests have been run on CPython 3.12 and 3.14.
Other Python versions and implementations have not been verified.

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

#Support
	IRC: irc://irc.freenode.net#bsdconv
