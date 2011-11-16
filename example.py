#!/usr/bin/env python
import sys
import bsdconv

p=bsdconv.Bsdconv(sys.argv[1])
if not p:
	print(bsdconv.error())
	del p
	sys.exit()
s=sys.stdin.read()
print(p.conv(s))
print('====================================')
print(p.info())
del p
