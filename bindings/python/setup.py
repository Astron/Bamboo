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

module = Extension('bamboo', sources = [moduleFilename], include_dirs = ['../../src'])
module.extra_compile_args = ['--std=c++11']
setup(name='bamboo', version='0.0',
      description='Bamboo is a library for defining object-oriented message protocols.',
      author='kestred',
      author_email='kestred@riotcave.com',
      url = 'https://github.com/Astron/Bamboo',
      ext_modules=[module])
