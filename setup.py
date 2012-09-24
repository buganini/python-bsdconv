#!/usr/bin/env python

from distutils.core import setup,Extension

setup(name='bsdconv',
      version='9.0',
      description='bsdconv library',
      author='Buganini',
      author_email='buganini@gmail.com',
      url='http://github.com/buganini/bsdconv',
      ext_modules=[Extension('bsdconv', ['bsdconv.c'],
              include_dirs=['/usr/local/include','c:/bsdconv/include'],
              libraries=['bsdconv'],
              library_dirs=['/usr/local/lib','c:/bsdconv'])],
     )

