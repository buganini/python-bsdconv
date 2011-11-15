#!/usr/bin/env python
import sys
import bsdconv

p=bsdconv.Bsdconv('utf-8:utf-8')
p.insert_phase('full', bsdconv.INTER, 1)
print(p.conv('test'))

print(bsdconv.codec_check(bsdconv.FROM,"_utf-8"))
print(bsdconv.codec_check(bsdconv.INTER,"_utf-8"))
print(bsdconv.codecs_list())

