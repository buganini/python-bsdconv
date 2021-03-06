#!/usr/bin/env python
from bsdconv import Bsdconv

sin="utf-8:utf-8,ascii"
sout=Bsdconv.insert_phase(sin, "upper", Bsdconv.INTER, 1)
print(sout)

sin=sout
sout=Bsdconv.replace_phase(sin, "full", Bsdconv.INTER, 1)
print(sout)

sin=sout
sout=Bsdconv.replace_codec(sin, "big5", 2, 1)
print(sout)

sin=sout
sout=Bsdconv.insert_codec(sin, "ascii", 0, 1)
print(sout)
