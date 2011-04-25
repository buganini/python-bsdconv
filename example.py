#!/usr/bin/env python
import sys
import bsdconv

p=bsdconv.create(sys.argv[1])
if p==None:
	print bsdconv.error()
	sys.exit()
s=sys.stdin.read()
print bsdconv.conv(p,s)
print '===================================='
print bsdconv.info(p)
bsdconv.destroy(p)
