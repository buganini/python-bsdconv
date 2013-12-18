#!/usr/bin/env python
import sys
from bsdconv import Bsdconv

p=Bsdconv(sys.argv[1])
if not p:
	print(Bsdconv.error())
	del p
	sys.exit()
p.init()
s=sys.stdin.read(1024)
while s:
	print(p.conv_chunk(s),)
	s=sys.stdin.read(1024)

print(p.conv_chunk_last(s))
print('====================================')
print(p.counter())
del p
