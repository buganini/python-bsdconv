#!/usr/bin/env python
import sys
from bsdconv import Bsdconv

p=Bsdconv(sys.argv[1])
if not p:
	print(Bsdconv.error())
	del p
	sys.exit()
s=sys.stdin.read()
print(p.conv(s))
print('====================================')
print(p.counter())
del p
