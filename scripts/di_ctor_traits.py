#!/usr/bin/env python

import sys
#sys.path.append(".")
#import pprint
#import ctypes
#import re
#from clang.cindex import Index
#from clang.cindex import CursorKind
#from clang.cindex import Type
#from clang.cindex import TypeKind
#from clang.cindex import TranslationUnit
#from optparse import OptionParser, OptionGroup
#import pyclang.cindex as clang

from clang.cindex import *
  
di_ctor_traits = """
    #ifndef BOOST_DI_CTOR_TRAITS
    #define BOOST_DI_CTOR_TRAITS

    %(includes)s

    namespace boost {
    namespace di {

        %(ctor_traits)s

    } // namespace di
    } // namespace boost

    #endif

"""

di_ctor_trait = """
    template<>
    struct ctor_traits< %(class)s >
    {
        static void ctor(%(args)s);
    };
"""

di_include = "#include <%(include)s>\n"

def get_ctors(node):
    ctors = {}
    get_ctors_impl(ctors, node)
    return ctors

def get_ctors_impl(ctors, node, namespace = ""):
    if node.kind == CursorKind.CONSTRUCTOR:
        #if namespace.startswith("::"):
        ctors[namespace + "::" + node.spelling] = node.displayname[len(node.spelling) + 1 : -1]

        #for el in node.get_tokens():
            #if el.spelling == "iii":
                #loc = SourceLocation.from_position(node.translation_unit, el.location.file, 8, 3)
                #cursor = Cursor.from_location(node.translation_unit, loc)
                #print cursor.location.line
                #print cursor.location.column
                #print cursor.kind
                #print cursor.underlying_typedef_type.kind.spelling

                #print cursor.underlying_typedef_type

#                loc2 = SourceLocation.from_position(node.translation_unit, node.location.file, cursor.get_definition().location.line, cursor.get_definition().location.column)
 #               cursor2 = Cursor.from_location(node.translation_unit, loc2)

    elif node.kind == CursorKind.NAMESPACE:
        namespace += "::" + node.displayname
        #if namespace.startswith("::"):
            [get_ctors_impl(ctors, c, namespace) for c in node.get_children()]
    else:
        [get_ctors_impl(ctors, c, namespace) for c in node.get_children()]

def generate_ctor_traits(ctors):
    result = ""
    for class_name, args in ctors.items():
        result += di_ctor_trait % { 'class' : class_name, 'args' : args }
    return result

def generate_includes(includes):
    result = ''
    for include in includes:
        result += di_include % { 'include' : include }
    return result

    #global opts

    #parser = OptionParser("usage: %prog [options] {filename} [clang-args*]")
    #parser.add_option("-i", "--include", dest="include", help="include pattern", default=None);
    #parser.add_option("-e", "--exclude", dest="exclude", help="exclude pattern", default=None);
    #parser.add_option("-o", "--output", dest="output", help="output file", default=None);
    #parser.disable_interspersed_args()
    #(opts, args) = parser.parse_args()

    #if len(args) == 0:
    #    parser.error('invalid number arguments')

    #print args
    #print opts.exclude
    #print opts.include

index = Index.create(excludeDecls = True)

f = open("di_ctor_traits.hpp", "w")

tu = index.parse(
    path = "__di__.hpp"
  , unsaved_files = [("__di__.hpp", generate_includes(sys.argv[1:]))]
  , options = TranslationUnit.PARSE_SKIP_FUNCTION_BODIES | TranslationUnit.PARSE_INCOMPLETE
)

if not tu:
    parser.error("unable to load input")

f.write(di_ctor_traits % {
    'includes' : ""
  , 'ctor_traits' : generate_ctor_traits(get_ctors(tu.cursor))
})

f.close()

#TODO template classes, ctors
#TODO 2 or more ctors (the longest one)
#TODO move or copy ctors
#TODO default, delete ctors, explicit
#TODO inherited ctors
#TODO typedefs
#TODO ignore empty ctor

