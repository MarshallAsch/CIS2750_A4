#!/usr/local/bin/python3

from distutils.core import setup, Extension
setup(name='dbwrapper', version='1.0',  \
      ext_modules=[Extension('dbwrapper',
      	sources = ['dbWrapper.c'],
      	extra_objects = ["out/dbAccess.o"],
      	libraries = ['mystr', "mysqlclient"],
      	include_dirs = ['/usr/local/mysql/include'],
      	library_dirs = ["lib", "/usr/local/mysql/lib"])])