#!/usr/bin/env python
import sys
import bsdconv

p=bsdconv.new(sys.argv[1])
if not p:
	print(p.error())
	del p
	sys.exit()
s=sys.stdin.read()
print(p.conv(s))
print('====================================')
print(p.info())
del p