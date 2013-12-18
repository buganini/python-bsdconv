#!/usr/bin/env python
import sys
from bsdconv import Bsdconv

print(Bsdconv.codec_check(Bsdconv.FROM,"_utf-8"))
print(Bsdconv.codec_check(Bsdconv.INTER,"_utf-8"))
print('From:')
print(Bsdconv.codecs_list(Bsdconv.FROM))
print('Inter:')
print(Bsdconv.codecs_list(Bsdconv.INTER))
print('To:')
print(Bsdconv.codecs_list(Bsdconv.TO))

