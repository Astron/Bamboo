#!/usr/bin/python
from pybindgen.module import Module, SubModule

from pybindgen import retval, param
from mappings import *
from wrappers import *

# FIXME: https://github.com/Astron/Bamboo/issues/1
def generate(file_):
    # Declare modules
    bamboo = Module('bamboo', cpp_namespace = "::bamboo")
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
    clsKeywordList = module.add_class('KeywordList',
        docstring = classDocstrings['KeywordList'])
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
    clsField = module.add_class('Field', parent = clsKeywordList,
        docstring = classDocstrings['Field'])
    clsMolecular = module.add_class('MolecularField', parent = [clsField, clsStruct])
    structImport = module.add_struct('Import')
    structNumber = module.add_struct('Number')
    structNumericRange = module.add_struct('NumericRange')
    dgOverflowError = wire.add_exception('DatagramOverflow', message_rvalue = 'exc.what()')
    dgiEOFError = wire.add_exception('DatagramIteratorEOF', message_rvalue = 'exc.what()')
    clsDatagram = wire.add_class('Datagram',
        docstring = classDocstrings['Datagram'])
    clsDgIter = wire.add_class('DatagramIterator',
        docstring = classDocstrings['DatagramIterator'])

    # Declare enums
    enumSubtype = module.add_enum('Subtype', [
        'kTypeInt8', 'kTypeInt16', 'kTypeInt32', 'kTypeInt64', 'kTypeUint8', 'kTypeUint16',
        'kTypeUint32', 'kTypeUint64', 'Subtype_Char', 'kTypeFloat32', 'kTypeFloat64', 'Subtype_String',
        'Subtype_Blob', 'Subtype_Array', 'Subtype_Struct', 'Subtype_Method', 'Subtype_None'])
    enumNumtype = structNumber.add_enum('Type', ['Number_NotANumber', 'Number_Signed', 'Number_Unsigned', 'Number_Floating'])

    # Wrap STL containers
    bits.add_container('std::vector<std::string>', 'std::string', 'vector', custom_name = 'StringCollection')

    # Declare member variables
    structImport.add_copy_constructor()
    structImport.add_constructor([param('const std::string &', 'moduleName')])
    structImport.add_instance_attribute('module', 'std::string')
    structImport.add_instance_attribute('symbols', 'std::vector<std::string>')
    structNumber.add_copy_constructor()
    structNumber.add_instance_attribute('type', 'bamboo::NumberType')
    structNumber.add_instance_attribute('integer', 'int64_t')
    structNumber.add_instance_attribute('uinteger', 'uint64_t')
    structNumber.add_instance_attribute('floating', 'double')
    structNumericRange.add_copy_constructor()
    structNumericRange.add_instance_attribute('type', 'bamboo::NumberType')
    structNumericRange.add_instance_attribute('min', 'Number')
    structNumericRange.add_instance_attribute('max', 'Number')

    # Declare functions/methods
    clsKeywordList.add_constructor([])
    clsKeywordList.add_copy_constructor()
    add_method(clsKeywordList, 'has_keyword', retval('bool'),
               [param('std::string', 'keyword')], is_const = True)
    add_method(clsKeywordList, 'has_matching_keywords', retval('bool'),
               [param('const bamboo::KeywordList&', 'other')], is_const = True)
    add_method(clsKeywordList, 'num_keywords', retval('size_t'), [], is_const = True)
    add_method(clsKeywordList, 'nth_keyword', retval('std::string'),
               [param('unsigned int', 'n')],
               is_const = True, throw = [indexError])
    add_method(clsKeywordList, 'add_keyword', retval('bool'), [param('std::string', 'keyword')])
    add_method(clsKeywordList, 'copy_keywords', None, [param('const bamboo::KeywordList&', 'other')])
    clsModule.add_constructor([])
    add_method(clsModule, 'num_classes', retval('size_t'), [], is_const = True)
    add_method(clsModule, 'num_structs', retval('size_t'), [], is_const = True)
    add_method(clsModule, 'num_types', retval('size_t'), [], is_const = True)
    add_method(clsModule, 'nth_class',
               retval_ref('bamboo::Class *'),
               [param('unsigned int', 'n')],
               throw = [indexError])
    add_method(clsModule, 'nth_struct',
               retval_ref('bamboo::Struct *'),
               [param('unsigned int', 'n')],
               throw = [indexError])
    add_method(clsModule, 'class_by_id',
               retval_ref('bamboo::Class *'),
               [param('unsigned int', 'id')])
    add_method(clsModule, 'class_by_name',
               retval_ref('bamboo::Class *'),
               [param('std::string', 'name')])
    add_method(clsModule, 'type_by_id',
               retval_ref('bamboo::Type *'),
               [param('unsigned int', 'id')])
    add_method(clsModule, 'type_by_name',
               retval_ref('bamboo::Type *'),
               [param('std::string', 'name')])
    add_method(clsModule, 'field_by_id',
               retval_ref('bamboo::Field *'),
               [param('unsigned int', 'id')])
    add_method(clsModule, 'num_imports', retval('size_t'), [], is_const = True)
    add_method(clsModule, 'nth_import',
               retval_ref('bamboo::Import *'),
               [param('unsigned int', 'n')],
               throw = [indexError])
    add_method(clsModule, 'has_keyword', retval('bool'),
               [param('std::string', 'keyword')], is_const = True)
    add_method(clsModule, 'num_keywords', retval('size_t'), [], is_const = True)
    add_method(clsModule, 'nth_keyword', retval('std::string'),
               [param('unsigned int', 'n')],
               is_const = True, throw = [indexError])
    add_method(clsModule, 'add_class', retval('bool'),
               [param('std::unique_ptr<Class>', 'dclass')])
    add_method(clsModule, 'add_struct', retval('bool'),
               [param('std::unique_ptr<Struct>', 'dstruct')])
    add_method(clsModule, 'add_import', None,
               [param('std::unique_ptr<Import>', 'import')])
    add_method(clsModule, 'add_typedef', retval('bool'),
               [param('std::string', 'name'),
                param('Type *', 'type', transfer_ownership = False)])
    add_method(clsModule, 'add_keyword', None, [param('std::string', 'keyword')])
    clsType.add_constructor([], visibility='protected')
    add_method(clsType, 'subtype', retval('bamboo::Subtype'), [], is_const = True)
    add_method(clsType, 'has_fixed_size', retval('bool'), [], is_const = True)
    add_method(clsType, 'fixed_size', retval('size_t'), [], is_const = True)
    add_method(clsType, 'has_alias', retval('bool'), [], is_const = True)
    add_method(clsType, 'alias', retval('std::string'), [], is_const = True)
    add_method(clsType, 'set_alias', None, [param('std::string', 'alias')])
    add_method(clsType, 'as_struct', retval_ref('bamboo::Struct *'), [])
    add_method(clsType, 'as_method', retval_ref('bamboo::Method *'), [])
    add_method(clsType, 'as_array', retval_ref('bamboo::Array *'), [])
    add_method(clsType, 'as_numeric', retval_ref('bamboo::Numeric *'), [])
    add_method(clsType, 'to_string', retval('std::string'), [])
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
               param('bamboo::NumericRange', 'arraySize', default_value = 'bamboo::NumericRange()')])
    add_method(clsArrType, 'element_type', retval_ref('const bamboo::Type *'), [], is_const = True)
    add_method(clsArrType, 'array_size', retval('unsigned int'), [], is_const = True)
    add_method(clsArrType, 'has_range', retval('bool'), [], is_const = True)
    add_method(clsArrType, 'range', retval('bamboo::NumericRange'), [], is_const = True)
    clsMethod.add_constructor([])
    add_method(clsMethod, 'num_params', retval('size_t'), [], is_const = True)
    add_method(clsMethod, 'nth_param', retval_ref('bamboo::Parameter *'),
               [param('unsigned int', 'n')],
               throw = [indexError])
    add_method(clsMethod, 'param_by_name', retval_ref('bamboo::Parameter *'),
               [param('std::string', 'name')]),
    add_method(clsMethod, 'add_param', retval('bool'), [param('std::unique_ptr<Parameter>', 'param')])
    clsStruct.add_constructor([
               param('bamboo::Module *', 'module', transfer_ownership = False),
               param('std::string', 'name')])
    add_method(clsStruct, 'id', retval('unsigned int'), [], is_const = True)
    add_method(clsStruct, 'name', retval('std::string'), [], is_const = True)
    add_method(clsStruct, 'module', retval_ref('bamboo::Module *'), [])
    add_method(clsStruct, 'num_fields', retval('size_t'), [], is_const = True)
    add_method(clsStruct, 'nth_field', retval_ref('bamboo::Field *'),
               [param('unsigned int', 'n')],
               throw = [indexError])
    add_method(clsStruct, 'field_by_id', retval_ref('bamboo::Field *'),
               [param('unsigned int', 'id')])
    add_method(clsStruct, 'field_by_name', retval_ref('bamboo::Field *'),
               [param('std::string', 'name')])
    add_method(clsStruct, 'add_field', retval('bool'), [param('std::unique_ptr<Field>', 'field')])
    add_method(clsStruct, 'as_class', retval_ref('bamboo::Class *'), [])
    clsClass.add_constructor([
               param('bamboo::Module *', 'module', transfer_ownership = False),
               param('std::string', 'name')])
    add_method(clsClass, 'num_parents', retval('size_t'), [], is_const = True)
    add_method(clsClass, 'nth_parent', retval_ref('bamboo::Class *'),
               [param('unsigned int', 'n')],
               throw = [indexError])
    add_method(clsClass, 'num_children', retval('size_t'), [], is_const = True)
    add_method(clsClass, 'nth_child', retval_ref('bamboo::Class *'),
               [param('unsigned int', 'n')],
               throw = [indexError])
    add_method(clsClass, 'num_declared_fields', retval('size_t'), [], is_const = True)
    add_method(clsClass, 'nth_declared_field', retval_ref('bamboo::Field *'),
               [param('unsigned int', 'n')],
               throw = [indexError])
    add_method(clsClass, 'add_parent', retval('bool'),
               [param('bamboo::Class *', 'parent', transfer_ownership = False)])
    clsParam.add_constructor([
               param('bamboo::Type *', 'type', transfer_ownership = False),
               param('std::string', 'name', default_value = '""')])
    add_method(clsParam, 'name', retval('std::string'), [], is_const = False)
    add_method(clsParam, 'type', retval_ref('bamboo::Type *'), [])
    add_method(clsParam, 'method', retval_ref('bamboo::Method *'), [])
    add_method(clsParam, 'has_default_value', retval('bool'), [], is_const = True),
    #add_method(clsParam, 'default_value', retval('const bamboo::Value'), [], is_const = True)
    add_method(clsParam, 'set_name', retval('bool'), [param('std::string', 'name')])
    add_method(clsParam, 'set_type', retval('bool'),
               [param('bamboo::Type *', 'type', transfer_ownership = False)]),
    #add_method(clsParam, 'set_default_value', retval('bool'), [param('const bamboo::Value', 'value')])
    #add_method(clsParam, 'set_default_value', retval('bool'), [param('const bamboo::Buffer&', 'value')])
    clsField.add_constructor([
               param('bamboo::Type *', 'type', transfer_ownership = False),
               param('std::string', 'name', default_value = '""')])
    add_method(clsField, 'id', retval('unsigned int'), [], is_const = True)
    add_method(clsField, 'name', retval('std::string'), [], is_const = False)
    add_method(clsField, 'type', retval_ref('bamboo::Type *'), [])
    add_method(clsField, 'container', retval_ref('bamboo::Struct *'), [])
    add_method(clsField, 'has_default_value', retval('bool'), [], is_const = True),
    #add_method(clsField, 'default_value', retval('const bamboo::Value'), [], is_const = True)
    add_method(clsField, 'set_name', retval('bool'), [param('std::string', 'name')])
    add_method(clsField, 'set_type', None,
               [param('bamboo::Type *', 'type', transfer_ownership = False)]),
    #add_method(clsField, 'set_default_value', retval('bool'), [param('const bamboo::Value', 'value')])
    #add_method(clsField, 'set_default_value', retval('bool'), [param('const bamboo::Buffer&', 'value')])
    clsMolecular.add_constructor([
               param('bamboo::Class *', 'cls', transfer_ownership = False),
               param('std::string', 'name')])
    clsDatagram.add_constructor([])
    clsDatagram.add_copy_constructor()
    add_method(clsDatagram, 'size', retval('size_t'), [], is_const = True)
    add_method(clsDatagram, 'cap', retval('size_t'), [], is_const = True)
    add_method(clsDatagram, 'add_bool', None, [param('bool', 'value')], throw = [dgOverflowError])
    add_method(clsDatagram, 'add_char', None, [param('char', 'value')], throw = [dgOverflowError])
    add_method(clsDatagram, 'add_int8', None, [param('int8_t', 'value')], throw = [dgOverflowError])
    add_method(clsDatagram, 'add_int16', None, [param('int16_t', 'value')], throw = [dgOverflowError])
    add_method(clsDatagram, 'add_int32', None, [param('int32_t', 'value')], throw = [dgOverflowError])
    add_method(clsDatagram, 'add_int64', None, [param('int64_t', 'value')], throw = [dgOverflowError])
    add_method(clsDatagram, 'add_uint8', None, [param('uint8_t', 'value')], throw = [dgOverflowError])
    add_method(clsDatagram, 'add_uint16', None, [param('uint16_t', 'value')], throw = [dgOverflowError])
    add_method(clsDatagram, 'add_uint32', None, [param('uint32_t', 'value')], throw = [dgOverflowError])
    add_method(clsDatagram, 'add_uint64', None, [param('uint64_t', 'value')], throw = [dgOverflowError])
    add_method(clsDatagram, 'add_float32', None, [param('float', 'value')], throw = [dgOverflowError])
    add_method(clsDatagram, 'add_float64', None, [param('double', 'value')], throw = [dgOverflowError])
    add_custom_method(clsDatagram, 'add_data')
    add_custom_method(clsDatagram, 'add_value')
    add_custom_method(clsDatagram, 'data', ('METH_NOARGS',))
    # add_string also moonlights as add_blob and addBlob
    add_method(clsDatagram, 'add_string', None, [param('std::string', 'value')], throw = [dgOverflowError])
    clsDgIter.add_constructor([
            param('const bamboo::Datagram&', 'dg'),
            param('size_t', 'offset', default_value = '0')])
    clsDgIter.add_copy_constructor()
    add_method(clsDgIter, 'tell', retval('size_t'), [], is_const = True)
    add_method(clsDgIter, 'seek', None, [param('size_t', 'offset')])
    add_method(clsDgIter, 'skip', None, [param('size_t', 'length')], throw = [dgiEOFError])
    add_method(clsDgIter, 'skip_type', None,
               [param('const bamboo::Type *', 'type', transfer_ownership = False)], throw = [dgiEOFError])
    add_method(clsDgIter, 'remaining', retval('size_t'), [], is_const = True)
    add_method(clsDgIter, 'read_bool', retval('bool'), [], throw = [dgiEOFError])
    add_method(clsDgIter, 'read_char', retval('char'), [], throw = [dgiEOFError])
    add_method(clsDgIter, 'read_int8', retval('int8_t'), [], throw = [dgiEOFError])
    add_method(clsDgIter, 'read_int16', retval('int16_t'), [], throw = [dgiEOFError])
    add_method(clsDgIter, 'read_int32', retval('int32_t'), [], throw = [dgiEOFError])
    add_method(clsDgIter, 'read_int64', retval('int64_t'), [], throw = [dgiEOFError])
    add_method(clsDgIter, 'read_uint8', retval('uint8_t'), [], throw = [dgiEOFError])
    add_method(clsDgIter, 'read_uint16', retval('uint16_t'), [], throw = [dgiEOFError])
    add_method(clsDgIter, 'read_uint32', retval('uint32_t'), [], throw = [dgiEOFError])
    add_method(clsDgIter, 'read_uint64', retval('uint64_t'), [], throw = [dgiEOFError])
    add_method(clsDgIter, 'read_float32', retval('float'), [], throw = [dgiEOFError])
    add_method(clsDgIter, 'read_float64', retval('double'), [], throw = [dgiEOFError])
    add_method(clsDgIter, 'read_size', retval('size_t'), [], throw = [dgiEOFError])
    # read_string also moonlights as read_blob and readBlob
    add_method(clsDgIter, 'read_string', retval('std::string'), [], throw = [dgiEOFError])
    add_method(clsDgIter, 'read_datagram', retval('bamboo::Datagram'), [], throw = [dgiEOFError])
    add_custom_method(clsDgIter, 'read_value')
    #add_method(clsDgIter, 'read_value', retval('bamboo::Value'),
    #           [param('const bamboo::Type *', 'type', transfer_ownership = False)])
    #add_method(clsDgIter, 'read_data', retval('std::string'), [param('size_t', 'length')])
    #add_method(clsDgIter, 'read_remainder', retval('std::string'), [])
    add_function(traits, 'legacy_hash', retval('uint32_t'),
                 [param('const bamboo::Module *', 'module', transfer_ownership = False)])
    add_function(dcfile, 'read_dcfile', retval('bamboo::Module *', caller_owns_return = True),
                 [param('std::string', 'filename')])
    add_function(dcfile, 'parse_dcfile', retval('bool'),
                 [param('bamboo::Module *', 'module', transfer_ownership = False),
                  param('std::string', 'filename')])
    #add_function(dcfile, 'parse_dcvalue', retval('bamboo::Buffer'),
    #             [param('const bamboo::Type *', 'type', transfer_ownership = False),
    #              param('std::string', 'formattedValue'), param('bool&', 'isError')])

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
      cls.add_custom_method_wrapper(n, wrapperNames[cls.name] + n, wrapperBodies[n], *args, **kwargs)

def add_function(mod, name, ret, params, **kwargs):
    names = altnames[name]
    doc = functionDocstrings[mod.name][name]
    for n in names: mod.add_function(name, ret, params, custom_name = n, **kwargs)

def retval_ref(typestr, **kwargs):
    return retval(typestr, reference_existing_object = True, **kwargs)

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
