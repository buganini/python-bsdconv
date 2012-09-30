#!/usr/bin/env python
import sys
import tempfile
import bsdconv

tmp=tempfile.TemporaryFile()
clist=open("characters_list.txt","w+")

p=bsdconv.Bsdconv("utf-8:score_train:null")
p.ctl(bsdconv.CTL_ATTACH_SCORE, tmp, 0)
p.ctl(bsdconv.CTL_ATTACH_OUTPUT_FILE, clist, 0)

if not p:
	print bsdconv.error()
	del p
	sys.exit()

p.init()
f=open(sys.argv[1])
s=f.read(1024)
while s:
	p.conv_chunk(s),
	s=f.read(1024)

p.conv_chunk_last(s)
f.close()
clist.close()
