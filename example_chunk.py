#!/usr/bin/env python
import sys
import bsdconv

p=bsdconv.create(sys.argv[1])
if p==None:
	print bsdconv.error()
	sys.exit()
bsdconv.insert_phase(p,'normal_score',bsdconv.INTER,1);
bsdconv.init(p)
s=sys.stdin.read(1024)
while s:
	print bsdconv.conv_chunk(p,s),
	s=sys.stdin.read(1024)

print bsdconv.conv_chunk_last(p,s)
print '===================================='
print bsdconv.info(p)
bsdconv.destroy(p)
