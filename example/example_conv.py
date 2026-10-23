#!/usr/bin/env python3
import sys
from bsdconv import Bsdconv

p=Bsdconv(sys.argv[1])
if not p:
	print(Bsdconv.error())
	del p
	sys.exit(1)
s=sys.stdin.buffer.read()
sys.stdout.buffer.write(p.conv(s))
sys.stdout.buffer.flush()
print('\n====================================')
print(p.counter())
del p
