#!/usr/bin/env python3

from setuptools import setup, Extension

setup(name='bsdconv',
      version='11.7.0',
      description='bsdconv library',
      python_requires='>=3.8',
      author='Buganini',
      author_email='buganini@gmail.com',
      url='http://github.com/buganini/bsdconv',
      ext_modules=[Extension('bsdconv', ['bsdconv.c'],
              include_dirs=['/usr/local/include','/usr/include','c:/bsdconv/include'],
              libraries=['bsdconv'],
              library_dirs=['/usr/local/lib','/usr/lib','c:/bsdconv'])],
     )

