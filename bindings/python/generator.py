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
    wire = SubModule('wire', bamboo)

    # Declare includes
    traits.add_include('"traits/byteorder.h"')
    traits.add_include('"traits/hashes.h"')
    traits.add_include('"traits/sizetag.h"')
    traits.add_include('"traits/Buffer.h"')
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
    wire.add_include('"wire/Datagram.h"')
    wire.add_include('"wire/DatagramIterator.h"')

    # Declare classes
    structBuffer = traits.add_struct('Buffer')
    indexError = traits.add_exception('range_error', custom_name = 'IndexError',
        foreign_cpp_namespace = 'std', message_rvalue = 'exc.what()')
    clsModule = module.add_class('Module')
    clsDistType = module.add_class('DistributedType')
    clsNumType = module.add_class('NumericType', parent = clsDistType)
    clsArrType = module.add_class('ArrayType', parent = clsDistType)
    clsMethod = module.add_class('Method', parent = clsDistType)
    clsStruct = module.add_class('Struct', parent = clsDistType)
    clsClass = module.add_class('Class', parent = clsStruct)
    clsParameter = module.add_class('Parameter')
    clsField = module.add_class('Field')
    structImport = module.add_struct('Import')
    structNumber = module.add_struct('Number')
    structNumericRange = module.add_class('NumericRange')
    clsDatagram = wire.add_class('Datagram')
    clsDgIter = wire.add_class('DatagramIterator')

    # Declare enums
    enumSubtype = module.add_enum('Subtype', [
        'kTypeInt8', 'kTypeInt16', 'kTypeInt32', 'kTypeInt64', 'kTypeUint8', 'kTypeUint16',
        'kTypeUint32', 'kTypeUint64', 'kTypeChar', 'kTypeFloat32', 'kTypeFloat64', 'kTypeString',
        'kTypeVarstring', 'kTypeBlob', 'kTypeVarblob', 'kTypeStruct', 'kTypeMethod', 'kTypeInvalid'])
    enumNumtype = structNumber.add_enum('Type', ['kNan', 'kInt', 'kUint', 'kFloat'])

    # Wrap STL containers
    module.add_container('std::vector<std::string>', 'std::string', 'vector')

    # Declare member variables
    structImport.add_constructor([param('const std::string &', 'moduleName')])
    structImport.add_instance_attribute('module', 'std::string')
    structImport.add_instance_attribute('symbols', 'std::vector<std::string>')
    structNumber.add_instance_attribute('type', 'Type')
    structNumber.add_instance_attribute('integer', 'int64_t')
    structNumber.add_instance_attribute('uinteger', 'uint64_t')
    structNumber.add_instance_attribute('floating', 'double')
    structNumericRange.add_instance_attribute('type', 'Type')
    structNumericRange.add_instance_attribute('min', 'Number')
    structNumericRange.add_instance_attribute('max', 'Number')

    # Declare functions/methods
    clsModule.add_constructor([])
    add_method(clsModule, 'get_num_classes', retval('size_t'), [], is_const = True)
    add_method(clsModule, 'get_num_structs', retval('size_t'), [], is_const = True)
    add_method(clsModule, 'get_num_types', retval('size_t'), [], is_const = True)
    add_method(clsModule, 'get_class',
               retval('bamboo::Class *', caller_owns_return = False),
               [param('unsigned int', 'n')])
    add_method(clsModule, 'get_struct',
               retval('bamboo::Struct *', caller_owns_return = False),
               [param('unsigned int', 'n')])
    add_method(clsModule, 'get_class_by_id',
               retval('bamboo::Class *', caller_owns_return = False),
               [param('unsigned int', 'id')])
    add_method(clsModule, 'get_class_by_name',
               retval('bamboo::Class *', caller_owns_return = False),
               [param('const std::string&', 'name')])
    add_method(clsModule, 'get_type_by_id',
               retval('bamboo::DistributedType *', caller_owns_return = False),
               [param('unsigned int', 'id')])
    add_method(clsModule, 'get_type_by_name',
               retval('bamboo::DistributedType *', caller_owns_return = False),
               [param('const std::string&', 'name')])
    add_method(clsModule, 'get_field_by_id',
               retval('bamboo::Field *', caller_owns_return = False),
               [param('unsigned int', 'id')])
    add_method(clsModule, 'get_num_imports', retval('size_t'), [], is_const = True)
    add_method(clsModule, 'get_import',
               retval('bamboo::Import *', caller_owns_return = False),
               [param('unsigned int', 'n')])
    add_method(clsModule, 'has_keyword', retval('bool'),
               [param('const std::string&', 'keyword')], is_const = True)
    add_method(clsModule, 'get_num_keywords', retval('size_t'), [], is_const = True)
    add_method(clsModule, 'get_keyword', retval('const std::string'),
               [param('unsigned int', 'n')], is_const = True)
    add_method(clsModule, 'add_class', retval('bool'),
               [param('bamboo::Class *', 'dclass', transfer_ownership = True)])
    add_method(clsModule, 'add_struct', retval('bool'),
               [param('bamboo::Struct *', 'dstruct', transfer_ownership = True)])
    add_method(clsModule, 'add_import', None,
               [param('bamboo::Import *', 'import', transfer_ownership = True)])
    add_method(clsModule, 'add_typedef', retval('bool'),
               [param('const std::string&', 'name'),
                param('DistributedType *', 'type', transfer_ownership = False)])
    add_method(clsModule, 'add_keyword', None, [param('const std::string&', 'keyword')])
    add_method(clsDistType, 'get_subtype', retval('bamboo::Subtype'), [], is_const = True)
    add_method(clsDistType, 'has_fixed_size', retval('bool'), [], is_const = True)
    add_method(clsDistType, 'get_size', retval('size_t'), [], is_const = True)
    add_method(clsDistType, 'has_alias', retval('bool'), [], is_const = True)
    add_method(clsDistType, 'as_numeric',
               retval('bamboo::NumericType *', caller_owns_return = False),
               [], is_virtual = True)
    add_method(clsDistType, 'as_array',
               retval('bamboo::ArrayType *', caller_owns_return = False),
               [], is_virtual = True)
    add_method(clsDistType, 'as_struct',
               retval('bamboo::Struct*', caller_owns_return = False),
               [], is_virtual = True)
    add_method(clsDistType, 'as_method',
               retval('bamboo::Method *', caller_owns_return = False),
               [], is_virtual = True)
    add_method(clsDistType, 'get_alias',
               retval('const std::string'),
               [], is_const = True)
    add_method(clsDistType, 'set_alias', None, [param('const std::string&', 'alias')])
    add_method(clsNumType, 'get_divisor', retval('unsigned int'), [], is_const = True)
    add_method(clsNumType, 'has_modulus', retval('bool'), [], is_const = True)
    add_method(clsNumType, 'get_modulus', retval('double'), [], is_const = True)
    add_method(clsNumType, 'has_range', retval('bool'), [], is_const = True)
    add_method(clsNumType, 'get_range', retval('bamboo::NumericRange'), [], is_const = True)
    add_method(clsNumType, 'set_divisor', retval('bool'), [param('unsigned int', 'divisor')])
    add_method(clsNumType, 'set_modulus', retval('bool'), [param('double', 'modulus')])
    add_method(clsNumType, 'set_range', retval('bool'), [param('const NumericRange&', 'range')])
    structBuffer.add_constructor([])
    structBuffer.add_copy_constructor()
    add_method(structBuffer, 'copy', retval('bamboo::Buffer'), [])
    add_method(structBuffer, 'seek', None, [param('unsigned int')])
    add_method(structBuffer, 'tell', retval('unsigned int'), [])
    add_method(structBuffer, 'size', retval('unsigned int'), [])
    add_method(structBuffer, 'read_bool', retval('bool'), [])
    add_method(structBuffer, 'read_char', retval('char'), [])
    add_method(structBuffer, 'read_int8', retval('int8_t'), [])
    add_method(structBuffer, 'read_int16', retval('int16_t'), [])
    add_method(structBuffer, 'read_int32', retval('int32_t'), [])
    add_method(structBuffer, 'read_int64', retval('int64_t'), [])
    add_method(structBuffer, 'read_uint8', retval('uint8_t'), [])
    add_method(structBuffer, 'read_uint16', retval('uint16_t'), [])
    add_method(structBuffer, 'read_uint32', retval('uint32_t'), [])
    add_method(structBuffer, 'read_uint64', retval('uint64_t'), [])
    add_method(structBuffer, 'read_float32', retval('float'), [])
    add_method(structBuffer, 'read_float64', retval('double'), [])
    add_method(structBuffer, 'pack_bool', None, [param('bool', 'val')])
    add_method(structBuffer, 'pack_char', None, [param('char', 'val')])
    add_method(structBuffer, 'pack_int8', None, [param('int8_t', 'val')])
    add_method(structBuffer, 'pack_int16', None, [param('int16_t', 'val')])
    add_method(structBuffer, 'pack_int32', None, [param('int32_t', 'val')])
    add_method(structBuffer, 'pack_int64', None, [param('int64_t', 'val')])
    add_method(structBuffer, 'pack_uint8', None, [param('uint8_t', 'val')])
    add_method(structBuffer, 'pack_uint16', None, [param('uint16_t', 'val')])
    add_method(structBuffer, 'pack_uint32', None, [param('uint32_t', 'val')])
    add_method(structBuffer, 'pack_uint64', None, [param('uint64_t', 'val')])
    add_method(structBuffer, 'pack_float32', None, [param('float', 'val')])
    add_method(structBuffer, 'pack_float64', None, [param('double', 'val')])
    add_method(structBuffer, 'pack_string', None, [param('const std::string&', 'val')])
    add_method(structBuffer, 'pack_buffer', None, [param('const bamboo::Buffer&', 'val')])
    add_method(structBuffer, 'from_bool', retval('bamboo::Buffer'),
               [param('bool', 'val')], is_static = True)
    add_method(structBuffer, 'from_char', retval('bamboo::Buffer'),
               [param('char', 'val')], is_static = True)
    add_method(structBuffer, 'from_int8', retval('bamboo::Buffer'),
               [param('int8_t', 'val')], is_static = True)
    add_method(structBuffer, 'from_int16', retval('bamboo::Buffer'),
               [param('int16_t', 'val')], is_static = True)
    add_method(structBuffer, 'from_int32', retval('bamboo::Buffer'),
               [param('int32_t', 'val')], is_static = True)
    add_method(structBuffer, 'from_int64', retval('bamboo::Buffer'),
               [param('int64_t', 'val')], is_static = True)
    add_method(structBuffer, 'from_uint8', retval('bamboo::Buffer'),
               [param('uint8_t', 'val')], is_static = True)
    add_method(structBuffer, 'from_uint16', retval('bamboo::Buffer'),
               [param('uint16_t', 'val')], is_static = True)
    add_method(structBuffer, 'from_uint32', retval('bamboo::Buffer'),
               [param('uint32_t', 'val')], is_static = True)
    add_method(structBuffer, 'from_uint64', retval('bamboo::Buffer'),
               [param('uint64_t', 'val')], is_static = True)
    add_method(structBuffer, 'from_float32', retval('bamboo::Buffer'),
               [param('float', 'val')], is_static = True)
    add_method(structBuffer, 'from_float64', retval('bamboo::Buffer'),
               [param('double', 'val')], is_static = True)
    add_method(structBuffer, 'from_string', retval('bamboo::Buffer'),
               [param('const std::string&', 'val')], is_static = True)
    add_method(structBuffer, 'get_byte', retval('uint8_t'),
               [param('unsigned int', 'index')], throw = [indexError])
    add_method(structBuffer, 'set_byte', None,
               [param('unsigned int', 'index'), param('char', 'item')], throw = [indexError])
    add_function(dcfile, 'read_dcfile', retval('bamboo::Module *', caller_owns_return = False),
                 [param('const std::string&', 'filename')])
    add_function(dcfile, 'parse_dcfile', retval('bool'),
                 [param('bamboo::Module *', 'module', transfer_ownership = False),
                  param('const std::string&', 'filename')])
    add_function(dcfile, 'parse_dcvalue', retval('bamboo::Buffer'),
                 [param('const bamboo::DistributedType *', 'type', transfer_ownership = False),
                  param('const std::string&', 'formattedValue'), param('bool&', 'isError')])

    bamboo.generate(file_)

def add_method(cls, name, ret, params, **kwargs):
    names = altnames[name]
    if name in docstrings:
      kwargs['docstring'] = docstrings[name]
    for n in names: cls.add_method(name, ret, params, custom_name = n, **kwargs)

def add_function(mod, name, ret, params, **kwargs):
    names = altnames[name]
    doc = docstrings[name]
    for n in names: mod.add_function(name, ret, params, custom_name = n, **kwargs)
