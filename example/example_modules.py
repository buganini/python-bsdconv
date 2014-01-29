#!/usr/bin/env python
import sys
from bsdconv import Bsdconv

print(Bsdconv.module_check(Bsdconv.FROM,"_utf-8"))
print(Bsdconv.module_check(Bsdconv.INTER,"_utf-8"))
print('Filter:')
print(Bsdconv.modules_list(Bsdconv.FILTER))
print('From:')
print(Bsdconv.modules_list(Bsdconv.FROM))
print('Inter:')
print(Bsdconv.modules_list(Bsdconv.INTER))
print('To:')
print(Bsdconv.modules_list(Bsdconv.TO))

