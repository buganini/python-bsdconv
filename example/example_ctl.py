#!/usr/bin/env python
import os
import sys
from bsdconv import Bsdconv

a=Bsdconv.mktemp("score.XXXXXX")
os.unlink(a[1])
clist=Bsdconv.fopen("characters_list.txt","w+")

p=Bsdconv("utf-8:score-train:null")
if not p:
	print(Bsdconv.error())
	del p
	sys.exit()

p.ctl(Bsdconv.CTL_ATTACH_SCORE, a[0], 0)
p.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, clist, 0)

p.init()
f=open(sys.argv[1])
s=f.read(1024)
while s:
	p.conv_chunk(s),
	s=f.read(1024)

p.conv_chunk_last(s)
f.close()
