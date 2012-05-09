#! /usr/bin/env python
# encoding: utf-8

import os

# Necessary since we override different Contexts
import waflib.extras.wurftools as wt

APPNAME = 'kodo_encode_decode_simple'
VERSION = '0.1'


def load_helper(ctx, name):
    if ctx.is_system_dependency(name):
        ctx.fatal('Load a tool to find %s as system dependency' % name)
    else:
        ctx.load_dependency(name)

def options(opt):
    opt.load('wurftools')

def configure(conf):

    conf.load('wurftools')


def build(bld):

    cxxflags = []

    if bld.env.TOOLCHAIN == 'linux':
        cxxflags += ['-O2', '-g', '-ftree-vectorize',
                     '-Wextra', '-Wall']

    if bld.env.TOOLCHAIN == 'win32':
        cxxflags += ['/O2', '/Ob2', '/W3', '/EHsc']

    bld.program(features = 'cxx',
                source   = ['example.cpp','Postoffice.cpp'],
                target   = 'example',
                include  = 'Postoffice.h',
                cxxflags = cxxflags)

