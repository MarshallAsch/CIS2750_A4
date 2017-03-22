#!/usr/local/bin/python3
# change use on the server
#!/usr/bin/python3

from distutils.core import setup, Extension
setup(name='dbAccess', version='1.0',  \
      ext_modules=[Extension('dbAccess', ['dbAccess.c', 'dbWrapper.c'])])