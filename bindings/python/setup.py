#!/usr/bin/python
from distutils.core import setup, Extension
from generator import generate
import os

buildDir = None
scriptDir = os.path.dirname(os.path.realpath(__file__))
if 'BINDINGS_BUILD_DIR' in os.environ:
  buildDir = os.environ['BINDINGS_BUILD_DIRECTORY']
else:
  buildDir = os.path.join(scriptDir, '../../build')

if 'BINDINGS_INCLUDE_DIRS' in os.environ:
  includeDirs = os.environ['BINDINGS_INCLUDE_DIRS'].split(';')
else:
  includeDirs = [buildDir, os.path.join(scriptDir, '../../src')]

if 'BINDINGS_LIBRARY_DIRS' in os.environ:
  libraryDirs = os.environ['BINDINGS_LIBRARY_DIRS'].split(';')
else:
  libraryDirs = [buildDir]

try:
	os.mkdir(buildDir)
except OSError:
	pass

moduleFilename = os.path.join(buildDir, 'python/pythonBindings.cpp')
with open(moduleFilename, 'wt') as file_:
    print('Generating file {}'.format(moduleFilename))
    generate(file_)

module = Extension('bamboo',
	include_dirs = includeDirs,
	sources = ['pythonBindings.cpp'],
	libraries = ['bamboo'],
  library_dirs = libraryDirs)
module.extra_compile_args = ['--std=c++11']
setup(name='bamboo', version='0.0',
      description='Bamboo is a library for defining object-oriented message protocols.',
      author='kestred',
      author_email='kestred@riotcave.com',
      url = 'https://github.com/Astron/Bamboo',
      ext_modules=[module])
