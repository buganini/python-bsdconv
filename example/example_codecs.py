#!/usr/bin/env python
import sys
import bsdconv

print(bsdconv.codec_check(bsdconv.FROM,"_utf-8"))
print(bsdconv.codec_check(bsdconv.INTER,"_utf-8"))
print('From:')
print(bsdconv.codecs_list(bsdconv.FROM))
print('Inter:')
print(bsdconv.codecs_list(bsdconv.INTER))
print('To:')
print(bsdconv.codecs_list(bsdconv.TO))
