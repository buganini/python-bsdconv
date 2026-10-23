#!/usr/bin/env python3
import sys
from bsdconv import Bsdconv

p=Bsdconv(sys.argv[1])
if not p:
	print(Bsdconv.error())
	del p
	sys.exit(1)
p.init()
s=sys.stdin.buffer.read(1024)
while s:
	sys.stdout.buffer.write(p.conv_chunk(s))
	s=sys.stdin.buffer.read(1024)

sys.stdout.buffer.write(p.conv_chunk_last(s))
sys.stdout.buffer.flush()
print('\n====================================')
print(p.counter())
del p
