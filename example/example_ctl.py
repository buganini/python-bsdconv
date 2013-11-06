#!/usr/bin/env python
import os
import sys
import bsdconv

a=bsdconv.mktemp("score.XXXXXX")
os.unlink(a[1])
clist=bsdconv.fopen("characters_list.txt","w+")

p=bsdconv.Bsdconv("utf-8:score-train:null")
if not p:
	print(bsdconv.error())
	del p
	sys.exit()

p.ctl(bsdconv.CTL_ATTACH_SCORE, a[0], 0)
p.ctl(bsdconv.CTL_ATTACH_OUTPUT_FILE, clist, 0)

p.init()
f=open(sys.argv[1])
s=f.read(1024)
while s:
	p.conv_chunk(s),
	s=f.read(1024)

p.conv_chunk_last(s)
f.close()
