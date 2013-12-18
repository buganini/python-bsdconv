#!/usr/bin/env python
import sys
from bsdconv import Bsdconv

p=Bsdconv(sys.argv[1])
if not p:
	print(Bsdconv.error())
	del p
	sys.exit()
p.conv_file(sys.argv[2], sys.argv[3])
print(p)
print(p.counter())
del p
