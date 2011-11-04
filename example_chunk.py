#!/usr/bin/env python
import sys
import bsdconv

p=bsdconv.Bsdconv(sys.argv[1])
if not p:
	print p.error()
	del p
	sys.exit()
p.insert_phase('full',bsdconv.INTER,1);
p.init()
s=sys.stdin.read(1024)
while s:
	print p.conv_chunk(s),
	s=sys.stdin.read(1024)

print p.conv_chunk_last(s)
print '===================================='
print p.info()
del p
