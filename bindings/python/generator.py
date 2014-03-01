#!/usr/bin/python
from pybindgen.module import Module, SubModule
from pybindgen import retval, param
from mappings import altnames
from mappings import docstrings

def generate(file_):
    # Declare modules
    bamboo = Module('bamboo', cpp_namespace="::bamboo")
    traits = SubModule('traits', bamboo)
    module = SubModule('module', bamboo)
    dcfile = SubModule('dcfile', bamboo)

    # Declare includes
    traits.add_include('"traits/byteorder.h"')
    traits.add_include('"traits/hashes.h"')
    traits.add_include('"traits/sizetag.h"')
    module.add_include('"module/ArrayType.h"')
    module.add_include('"module/Class.h"')
    module.add_include('"module/DistributedType.h"')
    module.add_include('"module/Field.h"')
    module.add_include('"module/KeywordList.h"')
    module.add_include('"module/Method.h"')
    module.add_include('"module/Module.h"')
    module.add_include('"module/MolecularField.h"')
    module.add_include('"module/NumericRange.h"')
    module.add_include('"module/NumericType.h"')
    module.add_include('"module/Parameter.h"')
    module.add_include('"module/Struct.h"')
    dcfile.add_include('"dcfile/format.h"')
    dcfile.add_include('"dcfile/parse.h"')
    dcfile.add_include('"dcfile/write.h"')

    # Declare enums
    enumSubtype = module.add_enum('Subtype', [
        'kTypeInt8', 'kTypeInt16', 'kTypeInt32', 'kTypeInt64', 'kTypeUint8', 'kTypeUint16',
        'kTypeUint32', 'kTypeUint64', 'kTypeChar', 'kTypeFloat32', 'kTypeFloat64', 'kTypeString',
        'kTypeVarstring', 'kTypeBlob', 'kTypeVarblob', 'kTypeStruct', 'kTypeMethod', 'kTypeInvalid'])

    # Declare classes
    clsModule = module.add_class('Module')
    clsDistType = module.add_class('DistributedType')
    clsNumType = module.add_class('NumericType')
    clsArrType = module.add_class('ArrayType')
    clsStruct = module.add_class('Struct')
    clsMethod = module.add_class('Method')

    # Declare functions/methods
    add_method(clsDistType, 'get_subtype', retval('bamboo::Subtype'), [], is_const = True)
    add_method(clsDistType, 'has_fixed_size', retval('bool'), [], is_const = True)
    add_method(clsDistType, 'get_size', retval('int'), [], is_const = True)
    add_method(clsDistType, 'has_alias', retval('bool'), [], is_const = True)
    add_method(clsDistType, 'as_numeric', retval('bamboo::NumericType *', caller_owns_return = False), [], is_virtual = True)
    add_method(clsDistType, 'as_array', retval('bamboo::ArrayType *', caller_owns_return = False), [], is_virtual = True)
    add_method(clsDistType, 'as_struct', retval('bamboo::Struct*', caller_owns_return = False), [], is_virtual = True)
    add_method(clsDistType, 'as_method', retval('bamboo::Method *', caller_owns_return = False), [], is_virtual = True)
    add_method(clsDistType, 'get_alias', retval('const std::string'), [], is_const = True)
    add_method(clsDistType, 'set_alias', None, [param('const std::string&', 'alias')])
    add_function(dcfile, 'read_dcfile', retval('bamboo::Module *', caller_owns_return = False),
                 [param('const std::string&', 'filename')])
    add_function(dcfile, 'parse_dcfile', retval('bool'),
                 [param('bamboo::Module *', 'module', transfer_ownership = False),
                  param('const std::string&', 'filename')])
    add_function(dcfile, 'parse_dcvalue', retval('std::string'),
                 [param('const bamboo::DistributedType *', 'type', transfer_ownership = False),
                  param('const std::string&', 'formattedValue'), param('bool&', 'isError')])

    bamboo.generate(file_)

def add_method(cls, name, ret, params, **kwargs):
    names = altnames[name]
    doc = docstrings[name]
    for n in names: cls.add_method(name, ret, params, custom_name = n, docstring = doc, **kwargs)

def add_function(mod, name, ret, params, **kwargs):
    names = altnames[name]
    doc = docstrings[name]
    for n in names: mod.add_function(name, ret, params, custom_name = n, docstring = doc, **kwargs)
