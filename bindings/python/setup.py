#!/usr/bin/python
from distutils.core import setup, Extension
import os

scriptDir = os.path.dirname(os.path.realpath(__file__))
buildDir = os.path.join(scriptDir, '../../build')
includeDirs = [buildDir, os.path.join(scriptDir, '../../src')]
libraryDirs = [buildDir]

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
