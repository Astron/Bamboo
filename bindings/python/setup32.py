#!/usr/bin/python
from distutils.core import setup, Extension
from generator import generate
import os

try:
	os.mkdir('build')
except OSError:
	pass

moduleFilename = os.path.join('build', 'pythonBindings.cpp')
with open(moduleFilename, 'wt') as file_:
    print('Generating file {}'.format(moduleFilename))
    generate(file_)

module = Extension('bamboo32',
	include_dirs = ['../../src'],
	sources = [moduleFilename],
	libraries = ['bamboo32'])
module.extra_compile_args = ['--std=c++11']
setup(name='bamboo32', version='0.0',
      description='Bamboo32 is a version of bamboo utilizing 32-bit sizetags.',
      author='kestred',
      author_email='kestred@riotcave.com',
      url = 'https://github.com/Astron/Bamboo',
      ext_modules=[module])
