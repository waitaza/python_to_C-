from setuptools import setup, Extension

module = Extension('dateutil_c',
                   sources=['dateutil.c'])

setup(name='dateutil_c',
      version='1.0',
      description='C extension for dateutil',
      ext_modules=[module])
