#! /usr/bin/env python
# encoding: utf-8

import os

# Necessary since we override different Contexts
import waflib.extras.wurftools as wt

APPNAME = 'kodo_encode_decode_simple'
VERSION = '0.1'

wt.add_dependency('boost', 'git://github.com/steinwurf/external-boost.git',
                  '1.1.0-boost_1_48_0')

def load_helper(ctx, name):
    if ctx.is_system_dependency(name):
        ctx.fatal('Load a tool to find %s as system dependency' % name)
    else:
        ctx.load_dependency(name)

def options(opt):
    opt.load('wurftools')
    load_helper(opt, 'boost')

def configure(conf):

    conf.load('wurftools')
    load_helper(conf, 'boost')


def build(bld):

    cxxflags = []
    load_helper(bld, 'boost')

    if bld.env.TOOLCHAIN == 'linux':
        cxxflags += ['-O2', '-g', '-ftree-vectorize',
                     '-Wextra', '-Wall']

    if bld.env.TOOLCHAIN == 'win32':
        cxxflags += ['/O2', '/Ob2', '/W3', '/EHsc']

    bld.program(features = 'cxx',
                source   = ['example.cpp','Postoffice.cpp'],
                target   = 'example',
                includes  = '..',
                use	= ['boost_includes'],
    	        defines = '__STDC_CONSTANT_MACROS',
                cxxflags = cxxflags)

