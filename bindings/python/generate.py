#!/usr/bin/python
from pybindgen.module import Module, SubModule
from pybindgen import retval, param
from mappings import *
from wrappers import *

# FIXME: https://github.com/Astron/Bamboo/issues/1

def generate(file_):
    # Declare modules
    bamboo = Module('bamboo', cpp_namespace="::bamboo")
    bits = SubModule('bits', bamboo)
    module = SubModule('module', bamboo)
    traits = SubModule('traits', bamboo)
    dcfile = SubModule('dcfile', bamboo)
    wire = SubModule('wire', bamboo)

    # Declare includes
    bits.add_include('"bits/byteorder.h"')
    bits.add_include('"bits/sizetag.h"')
    module.add_include('"module/Array.h"')
    module.add_include('"module/Class.h"')
    module.add_include('"module/Type.h"')
    module.add_include('"module/Field.h"')
    module.add_include('"module/KeywordList.h"')
    module.add_include('"module/Method.h"')
    module.add_include('"module/Module.h"')
    module.add_include('"module/MolecularField.h"')
    module.add_include('"module/NumericRange.h"')
    module.add_include('"module/Numeric.h"')
    module.add_include('"module/Parameter.h"')
    module.add_include('"module/Struct.h"')
    module.add_include('"module/Value.h"')
    traits.add_include('"traits/hashes.h"')
    dcfile.add_include('"dcfile/format.h"')
    dcfile.add_include('"dcfile/parse.h"')
    dcfile.add_include('"dcfile/write.h"')
    wire.add_include('"wire/Datagram.h"')
    wire.add_include('"wire/DatagramIterator.h"')

    # Declare classes
    indexError = bamboo.add_exception('out_of_range',
        custom_name = 'IndexError',
        foreign_cpp_namespace = 'std',
        message_rvalue = 'exc.what()',
        is_standard_error = True)
    typeError = bamboo.add_exception('invalid_argument',
        custom_name = 'TypeError',
        foreign_cpp_namespace = 'std',
        message_rvalue = 'exc.what()',
        is_standard_error = True)
    clsModule = module.add_class('Module',
        docstring = classDocstrings['Module'])
    clsType = module.add_class('Type',
        docstring = classDocstrings['Type'])
    clsNumType = module.add_class('Numeric',parent = clsType,
        docstring = classDocstrings['Numeric'])
    clsArrType = module.add_class('Array', parent = clsType,
        docstring = classDocstrings['Array'])
    clsMethod = module.add_class('Method', parent = clsType,
        docstring = classDocstrings['Method'])
    clsStruct = module.add_class('Struct', parent = clsType,
        docstring = classDocstrings['Struct'])
    clsClass = module.add_class('Class', parent = clsStruct,
        docstring = classDocstrings['Class'])
    clsParam = module.add_class('Parameter',
        docstring = classDocstrings['Parameter'])
    clsField = module.add_class('Field',
        docstring = classDocstrings['Field'])
    clsMolecular = module.add_class('MolecularField', parent = [clsField, clsStruct])
    structImport = module.add_struct('Import')
    structNumber = module.add_struct('Number')
    structNumericRange = module.add_struct('NumericRange')
    #structValue = module.add_struct('Value',
    #    docstring = classDocstrings['Value'])
    clsDatagram = wire.add_class('Datagram',
        docstring = classDocstrings['Datagram'])
    clsDgIter = wire.add_class('DatagramIterator',
        docstring = classDocstrings['DatagramIterator'])

    # Declare enums
    enumSubtype = module.add_enum('Subtype', [
        'kTypeInt8', 'kTypeInt16', 'kTypeInt32', 'kTypeInt64', 'kTypeUint8', 'kTypeUint16',
        'kTypeUint32', 'kTypeUint64', 'kTypeChar', 'kTypeFloat32', 'kTypeFloat64', 'kTypeString',
        'kTypeVarstring', 'kTypeBlob', 'kTypeVarblob', 'kTypeStruct', 'kTypeMethod', 'kTypeInvalid'])
    enumNumtype = structNumber.add_enum('Type', ['kNaN', 'kInt', 'kUint', 'kFloat'])

    # Wrap STL containers
    module.add_container('std::vector<std::string>', 'std::string', 'vector')

    # Declare member variables
    structImport.add_copy_constructor()
    structImport.add_constructor([param('const std::string &', 'moduleName')])
    structImport.add_instance_attribute('module', 'std::string')
    structImport.add_instance_attribute('symbols', 'std::vector<std::string>')
    structNumber.add_copy_constructor()
    structNumber.add_instance_attribute('type', 'bamboo::Number::Type')
    structNumber.add_instance_attribute('integer', 'int64_t')
    structNumber.add_instance_attribute('uinteger', 'uint64_t')
    structNumber.add_instance_attribute('floating', 'double')
    structNumericRange.add_copy_constructor()
    structNumericRange.add_instance_attribute('type', 'bamboo::Number::Type')
    structNumericRange.add_instance_attribute('min', 'Number')
    structNumericRange.add_instance_attribute('max', 'Number')

    # Declare functions/methods
    #structValue.add_copy_constructor()
    #add_method(structValue, 'from_type', retval('bamboo::Value'),
    #           [param('const bamboo::Type *', 'type', transfer_ownership = False)],
    #           is_static = True, throw = [typeError])
    #add_custom_method(structValue, 'from_packed', ('METH_KEYWORDS', 'METH_VARARGS', 'METH_STATIC'))
    #add_method(structValue, 'pack', retval('bamboo::Buffer'),
    #           [param('const bamboo::Type *', 'type', transfer_ownership = False)],
    #           is_const = True, throw = [typeError])
    #add_method(structValue, 'size', retval('size_t'), [], is_const = True)
    #add_method(structValue, '_getitem_', retval('bamboo::Value'),
    #           [param('unsigned int', 'index')], throw = [indexError])
    #add_method(structValue, '_getitem_', retval('bamboo::Value'),
    #           [param('const std::string&', 'item')], throw = [indexError])
    #add_method(structValue, '_setitem_', None,
    #           [param('unsigned int', 'index'), param('const Value', 'value')], throw = [indexError])
    #add_method(structValue, '_setitem_', None,
    #           [param('const std::string&', 'item'), param('const Value', 'value')], throw = [indexError])
    clsModule.add_constructor([])
    add_method(clsModule, 'num_classes', retval('size_t'), [], is_const = True)
    add_method(clsModule, 'num_structs', retval('size_t'), [], is_const = True)
    add_method(clsModule, 'num_tyoes', retval('size_t'), [], is_const = True)
    add_method(clsModule, 'get_class',
               retval_child('bamboo::Class *'),
               [param('unsigned int', 'n')])
    add_method(clsModule, 'get_struct',
               retval_child('bamboo::Struct *'),
               [param('unsigned int', 'n')])
    add_method(clsModule, 'class_by_id',
               retval_child('bamboo::Class *'),
               [param('unsigned int', 'id')])
    add_method(clsModule, 'class_by_name',
               retval_child('bamboo::Class *'),
               [param('const std::string&', 'name')])
    add_method(clsModule, 'type_by_id',
               retval_child('bamboo::Type *'),
               [param('unsigned int', 'id')])
    add_method(clsModule, 'type_by_name',
               retval_child('bamboo::Type *'),
               [param('const std::string&', 'name')])
    add_method(clsModule, 'field_by_id',
               retval_child('bamboo::Field *'),
               [param('unsigned int', 'id')])
    add_method(clsModule, 'num_imports', retval('size_t'), [], is_const = True)
    add_method(clsModule, 'get_import',
               retval_child('bamboo::Import *'),
               [param('unsigned int', 'n')])
    add_method(clsModule, 'has_keyword', retval('bool'),
               [param('const std::string&', 'keyword')], is_const = True)
    add_method(clsModule, 'num_keywords', retval('size_t'), [], is_const = True)
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
                param('Type *', 'type', transfer_ownership = False)])
    add_method(clsModule, 'add_keyword', None, [param('const std::string&', 'keyword')])
    clsType.add_constructor([], visibility='protected')
    add_method(clsType, 'subtype', retval('bamboo::Subtype'), [], is_const = True)
    add_method(clsType, 'has_fixed_size', retval('bool'), [], is_const = True)
    add_method(clsType, 'fixed_size', retval('size_t'), [], is_const = True)
    add_method(clsType, 'has_alias', retval('bool'), [], is_const = True)
    add_method(clsType, 'as_numeric', retval_self('bamboo::Numeric *'), [], is_virtual = True)
    add_method(clsType, 'as_array', retval_self('bamboo::Array *'), [], is_virtual = True)
    add_method(clsType, 'as_struct', retval_self('bamboo::Struct*'), [], is_virtual = True)
    add_method(clsType, 'as_method', retval_self('bamboo::Method *'), [], is_virtual = True)
    add_method(clsType, 'alias', retval('const std::string'), [], is_const = True)
    add_method(clsType, 'set_alias', None, [param('const std::string&', 'alias')])
    clsNumType.add_constructor([param('bamboo::Subtype', 'subtype')])
    add_method(clsNumType, 'divisor', retval('unsigned int'), [], is_const = True)
    add_method(clsNumType, 'has_modulus', retval('bool'), [], is_const = True)
    add_method(clsNumType, 'modulus', retval('double'), [], is_const = True)
    add_method(clsNumType, 'has_range', retval('bool'), [], is_const = True)
    add_method(clsNumType, 'range', retval('bamboo::NumericRange'), [], is_const = True)
    add_method(clsNumType, 'set_divisor', retval('bool'), [param('unsigned int', 'divisor')])
    add_method(clsNumType, 'set_modulus', retval('bool'), [param('double', 'modulus')])
    add_method(clsNumType, 'set_range', retval('bool'), [param('const NumericRange&', 'range')])
    clsArrType.add_constructor([
               param('bamboo::Type *', 'elementType', transfer_ownership = False),
               param('const bamboo::NumericRange&', 'arraySize', default_value = 'bamboo::NumericRange()')])
    add_method(clsArrType, 'element_type', retval_child('const bamboo::Type *'), [], is_const = True)
    add_method(clsArrType, 'array_size', retval('unsigned int'), [], is_const = True)
    add_method(clsArrType, 'has_range', retval('bool'), [], is_const = True)
    add_method(clsArrType, 'range', retval('bamboo::NumericRange'), [], is_const = True)
    clsMethod.add_constructor([])
    add_method(clsMethod, 'num_parameters', retval('size_t'), [], is_const = True)
    add_method(clsMethod, 'get_parameter', retval_child('bamboo::Parameter *'),
               [param('unsigned int', 'n')])
    add_method(clsMethod, 'parameter_by_name', retval_child('bamboo::Parameter *'),
               [param('const std::string&', 'name')]),
    add_method(clsMethod, 'add_parameter', retval('bool'),
               [param('bamboo::Parameter *', 'param', transfer_ownership = True)])
    clsStruct.add_constructor([
               param('bamboo::Module *', 'module', transfer_ownership = False),
               param('const std::string&', 'name')])
    add_method(clsStruct, 'id', retval('unsigned int'), [], is_const = True)
    add_method(clsStruct, 'name', retval('std::string'), [], is_const = True)
    add_method(clsStruct, 'module', retval_parent('bamboo::Module *'), [])
    add_method(clsStruct, 'num_fields', retval('size_t'), [], is_const = True)
    add_method(clsStruct, 'get_field', retval_child('bamboo::Field *'),
               [param('unsigned int', 'n')])
    add_method(clsStruct, 'field_by_id', retval_child('bamboo::Field *'),
               [param('unsigned int', 'id')])
    add_method(clsStruct, 'field_by_name', retval_child('bamboo::Field *'),
               [param('const std::string&', 'name')])
    add_method(clsStruct, 'add_field', retval('bool'),
               [param('bamboo::Field *', 'field', transfer_ownership = True)])
    clsClass.add_constructor([
               param('bamboo::Module *', 'module', transfer_ownership = False),
               param('const std::string&', 'name')])
    add_method(clsClass, 'num_parents', retval('size_t'), [], is_const = True)
    add_method(clsClass, 'get_parent', retval_child('bamboo::Class *'), [param('unsigned int', 'n')])
    add_method(clsClass, 'num_children', retval('size_t'), [], is_const = True)
    add_method(clsClass, 'get_child', retval_child('bamboo::Class *'), [param('unsigned int', 'n')])
    add_method(clsClass, 'has_constructor', retval('bool'), [], is_const = True)
    add_method(clsClass, 'constructor', retval_child('bamboo::Field *'), [])
    add_method(clsClass, 'num_base_fields', retval('size_t'), [], is_const = True)
    add_method(clsClass, 'get_base_field', retval_child('bamboo::Field *'), [param('unsigned int', 'n')])
    add_method(clsClass, 'add_parent', None,
               [param('bamboo::Class *', 'parent', transfer_ownership = False)])
    clsParam.add_constructor([
               param('bamboo::Type *', 'type', transfer_ownership = False),
               param('const std::string&', 'name', default_value = '""')])
    add_method(clsParam, 'name', retval('std::string'), [], is_const = False)
    add_method(clsParam, 'type', retval_child('bamboo::Type *'), [])
    add_method(clsParam, 'get_method', retval_parent('bamboo::Method *'), [])
    add_method(clsParam, 'has_default_value', retval('bool'), [], is_const = True),
    #add_method(clsParam, 'default_value', retval('const bamboo::Value'), [], is_const = True)
    add_method(clsParam, 'set_name', retval('bool'), [param('const std::string&', 'name')])
    add_method(clsParam, 'set_type', retval('bool'),
               [param('bamboo::Type *', 'type', transfer_ownership = False)]),
    #add_method(clsParam, 'set_default_value', retval('bool'), [param('const bamboo::Value', 'value')])
    #add_method(clsParam, 'set_default_value', retval('bool'), [param('const bamboo::Buffer&', 'value')])
    clsField.add_constructor([
               param('bamboo::Type *', 'type', transfer_ownership = False),
               param('const std::string&', 'name', default_value = '""')])
    add_method(clsField, 'id', retval('unsigned int'), [], is_const = True)
    add_method(clsField, 'name', retval('std::string'), [], is_const = False)
    add_method(clsField, 'type', retval_child('bamboo::Type *'), [])
    add_method(clsField, 'record', retval_parent('bamboo::Struct *'), [])
    add_method(clsField, 'has_default_value', retval('bool'), [], is_const = True),
    #add_method(clsField, 'default_value', retval('const bamboo::Value'), [], is_const = True)
    add_method(clsField, 'set_name', retval('bool'), [param('const std::string&', 'name')])
    add_method(clsField, 'set_type', None,
               [param('bamboo::Type *', 'type', transfer_ownership = False)]),
    #add_method(clsField, 'set_default_value', retval('bool'), [param('const bamboo::Value', 'value')])
    #add_method(clsField, 'set_default_value', retval('bool'), [param('const bamboo::Buffer&', 'value')])
    clsMolecular.add_constructor([
               param('bamboo::Class *', 'cls', transfer_ownership = False),
               param('const std::string&', 'name')])
    clsDatagram.add_constructor([])
    clsDatagram.add_copy_constructor()
    add_custom_method(clsDatagram, 'data', ('METH_NOARGS',))
    add_method(clsDatagram, 'size', retval('size_t'), [], is_const = True)
    add_method(clsDatagram, 'cap', retval('size_t'), [], is_const = True)
    add_method(clsDatagram, 'add_bool', None, [param('bool', 'value')])
    add_method(clsDatagram, 'add_char', None, [param('char', 'value')])
    add_method(clsDatagram, 'add_int8', None, [param('int8_t', 'value')])
    add_method(clsDatagram, 'add_int16', None, [param('int16_t', 'value')])
    add_method(clsDatagram, 'add_int32', None, [param('int32_t', 'value')])
    add_method(clsDatagram, 'add_int64', None, [param('int64_t', 'value')])
    add_method(clsDatagram, 'add_uint8', None, [param('uint8_t', 'value')])
    add_method(clsDatagram, 'add_uint16', None, [param('uint16_t', 'value')])
    add_method(clsDatagram, 'add_uint32', None, [param('uint32_t', 'value')])
    add_method(clsDatagram, 'add_uint64', None, [param('uint64_t', 'value')])
    add_method(clsDatagram, 'add_float32', None, [param('float', 'value')])
    add_method(clsDatagram, 'add_float64', None, [param('double', 'value')])
    add_custom_method(clsDatagram, 'add_data')
    #add_method(clsDatagram, 'add_data', None, [param('const bamboo::Buffer&', 'value')])
    add_method(clsDatagram, 'add_string', None, [param('const std::string&', 'value')])
    #add_method(clsDatagram, 'add_blob', None, [param('const bamboo::Buffer&', 'value')])
    #add_method(clsDatagram, 'add_value', None,
    #           [param('const bamboo::Type *', 'type', transfer_ownership = False),
    #            param('const Value', 'value')])
    #add_method(clsDatagram, 'add_packed', None,
    #           [param('const bamboo::Type *', 'type', transfer_ownership = False),
    #            param('const bamboo::Buffer&', 'packed'),
    #            param('size_t', 'offset', default_value = '0')])
    clsDgIter.add_constructor([
            param('const bamboo::Datagram&', 'dg'),
            param('size_t', 'offset', default_value = '0')])
    clsDgIter.add_copy_constructor()
    add_method(clsDgIter, 'tell', retval('size_t'), [], is_const = True)
    add_method(clsDgIter, 'seek', None, [param('size_t', 'offset')])
    add_method(clsDgIter, 'skip', None, [param('size_t', 'length')])
    add_method(clsDgIter, 'skip_type', None,
               [param('const bamboo::Type *', 'type', transfer_ownership = False)])
    add_method(clsDgIter, 'remaining', retval('size_t'), [], is_const = True)
    #add_method(clsDgIter, 'read_remainder', retval('bamboo::Buffer'), [])
    add_method(clsDgIter, 'read_bool', retval('bool'), [])
    add_method(clsDgIter, 'read_char', retval('char'), [])
    add_method(clsDgIter, 'read_int8', retval('int8_t'), [])
    add_method(clsDgIter, 'read_int16', retval('int16_t'), [])
    add_method(clsDgIter, 'read_int32', retval('int32_t'), [])
    add_method(clsDgIter, 'read_int64', retval('int64_t'), [])
    add_method(clsDgIter, 'read_uint8', retval('uint8_t'), [])
    add_method(clsDgIter, 'read_uint16', retval('uint16_t'), [])
    add_method(clsDgIter, 'read_uint32', retval('uint32_t'), [])
    add_method(clsDgIter, 'read_uint64', retval('uint64_t'), [])
    add_method(clsDgIter, 'read_float32', retval('float'), [])
    add_method(clsDgIter, 'read_float64', retval('double'), [])
    add_method(clsDgIter, 'read_size', retval('size_t'), [])
    add_method(clsDgIter, 'read_string', retval('std::string'), [])
    #add_method(clsDgIter, 'read_blob', retval('bamboo::Buffer'), [])
    add_method(clsDgIter, 'read_datagram', retval('bamboo::Datagram'), [])
    #add_method(clsDgIter, 'read_data', retval('bamboo::Buffer'), [param('size_t', 'length')])
    #add_method(clsDgIter, 'read_value', retval('bamboo::Value'),
    #           [param('const bamboo::Type *', 'type', transfer_ownership = False)])
    #add_method(clsDgIter, 'read_packed', retval('bamboo::Buffer'),
    #           [param('const bamboo::Type *', 'type', transfer_ownership = False)])
    add_function(traits, 'legacy_hash', retval('uint32_t'),
                 [param('const bamboo::Module *', 'module', transfer_ownership = False)])
    add_function(dcfile, 'read_dcfile', retval('bamboo::Module *', caller_owns_return = True),
                 [param('const std::string&', 'filename')])
    add_function(dcfile, 'parse_dcfile', retval('bool'),
                 [param('bamboo::Module *', 'module', transfer_ownership = False),
                  param('const std::string&', 'filename')])
    #add_function(dcfile, 'parse_dcvalue', retval('bamboo::Buffer'),
    #             [param('const bamboo::Type *', 'type', transfer_ownership = False),
    #              param('const std::string&', 'formattedValue'), param('bool&', 'isError')])

    bamboo.generate(file_)

def add_method(cls, name, ret, params, **kwargs):
    names = altnames[name]
    if cls.get_python_name() in methodDocstrings and \
       name in methodDocstrings[cls.get_python_name()]:
      kwargs['docstring'] = methodDocstrings[cls.get_python_name()][name]
    for n in names: cls.add_method(name, ret, params, custom_name = n, **kwargs)

def add_custom_method(cls, name, *args, **kwargs):
    names = altnames[name]
    if cls.get_python_name() in methodDocstrings and \
       name in methodDocstrings[cls.get_python_name()]:
      kwargs['docstring'] = methodDocstrings[cls.get_python_name()][name]
    for n in names:
      cls.add_custom_method_wrapper(n, wrapperNames[cls.name] + n, wrapperBodies[name] % n, *args, **kwargs)

def add_function(mod, name, ret, params, **kwargs):
    names = altnames[name]
    doc = functionDocstrings[mod.name][name]
    for n in names: mod.add_function(name, ret, params, custom_name = n, **kwargs)

def retval_self(typestr, **kwargs):
    # N.B. Currently this is an alias for retval_child, but we may consider
    #      changing it to have different behavior to make memory management cleaner.
    #      For now its ok, because usage of this pattern tends to be cleaned-up fairly quickly.
    return retval_child(typestr, **kwargs)

def retval_parent(typestr, **kwargs):
    # FIXME: Currently this is an alias for retval_child, but it needs to be
    #        changed to have the returned object be the custodian of the child
    #        to reduce the amount of reference-loops that are generated
    return retval_child(typestr, **kwargs)

def retval_child(typestr, **kwargs):
    return retval(typestr, custodian = -1, caller_owns_return = False, return_internal_reference = True, **kwargs)

if __name__ == "__main__":
    import sys, os

    scriptDir = os.path.dirname(os.path.realpath(__file__))
    buildDir = os.path.join(scriptDir, '../../build')

    try:
        os.mkdir(buildDir)
    except OSError:
        pass

    filename = os.path.join(buildDir, sys.argv[1])
    with open(filename, 'wt') as file_:
        print('Generating file {}'.format(filename))
        generate(file_)
