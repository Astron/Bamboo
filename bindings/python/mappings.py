altnames = {
    # Common alternate names
    'copy': ['copy', '__deepcopy__'],
    'seek': ['seek'],
    'tell': ['tell'],
    'size': ['size'],

    # Module
    'get_num_classes':   ['get_num_classes',   'getNumClasses'],
    'get_num_structs':   ['get_num_structs',   'getNumStructs'],
    'get_class':         ['get_class',         'getClass'],
    'get_struct':        ['get_struct',        'getStruct'],
    'get_class_by_id':   ['get_class_by_id',   'getClassById'],
    'get_class_by_name': ['get_class_by_name', 'getClassByName'],
    'get_num_types':     ['get_num_types',     'getNumTypes'],
    'get_type_by_id':    ['get_type_by_id',    'getTypeById'],
    'get_type_by_name':  ['get_type_by_name',  'getTypeByName'],
    'get_field_by_id':   ['get_field_by_id',   'getFieldById'],
    'get_num_imports':   ['get_num_imports',   'getNumImports'],
    'get_import':        ['get_import',        'getImport'],
    'has_keyword':       ['has_keyword',       'hasKeyword'],
    'get_num_keywords':  ['get_num_keywords',  'getNumKeywords'],
    'get_keyword':       ['get_keyword',       'getKeyword'],
    'add_class':         ['add_class',         'addClass'],
    'add_struct':        ['add_struct',        'addStruct'],
    'add_import':        ['add_import',        'addImport'],
    'add_typedef':       ['add_typedef',       'addTypedef'],
    'add_keyword':       ['add_keyword',       'addKeyword'],

    # DistributedType
    'get_subtype':    ['get_subtype',    'getSubtype'],
    'has_fixed_size': ['has_fixed_size', 'hasFixedSize'],
    'get_size':       ['get_size',       'getSize'],
    'has_alias':      ['has_alias',      'hasAlias'],
    'as_numeric':     ['as_numeric',     'asNumeric'],
    'as_array':       ['as_array',       'asArray'],
    'as_struct':      ['as_struct',      'asStruct'],
    'as_method':      ['as_method',      'asMethod'],
    'get_alias':      ['get_alias',      'getAlias'],
    'set_alias':      ['set_alias',      'setAlias'],

    # NumericType
    'get_divisor': ['get_divisor', 'getDivisor'],
    'has_modulus': ['has_modulus', 'hasModulus'],
    'get_modulus': ['get_modulus', 'getModulus'],
    'has_range':   ['has_range',   'hasRange'],
    'get_range':   ['get_range',   'getRange'],
    'set_divisor': ['set_divisor', 'setDivisor'],
    'set_modulus': ['set_modulus', 'setModulus'],
    'set_range':   ['set_range',   'setRange'],

    # ArrayType
    'get_element_type': ['get_element_type', 'getElementType'],
    'get_array_size':   ['get_array_size',   'getArraySize'],
    #'has_range':       ['has_range',        'hasRange'],
    #'get_range':       ['get_range',        'getRange'],

    # Method
    'get_num_parameters':    ['get_num_parameters',    'getNumParameters'],
    'get_parameter':         ['get_parameter',         'getParameter'],
    'get_parameter_by_name': ['get_parameter_by_name', 'getParameterByName'],
    'add_parameter':         ['add_parameter',         'addParameter'],

    # Struct
    'get_id':            ['get_id',            'getId'],
    'get_name':          ['get_name',          'getName'],
    'get_module':        ['get_module',        'getModule'],
    'get_num_fields':    ['get_num_fields',    'getNumFields'],
    'get_field':         ['get_field',         'getField'],
    'get_field_by_id':   ['get_field_by_id',   'getFieldById'],
    'get_field_by_name': ['get_field_by_name', 'getFieldByName'],
    'add_field':         ['add_field',         'addField'],

    # Buffer
    'read_bool':    ['read_bool',    'readBool'],
    'read_char':    ['read_char',    'readChar'],
    'read_int8':    ['read_int8',    'readInt8'],
    'read_int16':   ['read_int16',   'readInt16'],
    'read_int32':   ['read_int32',   'readInt32'],
    'read_int64':   ['read_int64',   'readInt64'],
    'read_uint8':   ['read_uint8',   'readUint8'],
    'read_uint16':  ['read_uint16',  'readUint16'],
    'read_uint32':  ['read_uint32',  'readUint32'],
    'read_uint64':  ['read_uint64',  'readUint64'],
    'read_float32': ['read_float32', 'readFloat32'],
    'read_float64': ['read_float64', 'readFloat64'],
    'pack_bool':    ['pack_bool',    'packBool'],
    'pack_char':    ['pack_char',    'packChar'],
    'pack_int8':    ['pack_int8',    'packInt8'],
    'pack_int16':   ['pack_int16',   'packInt16'],
    'pack_int32':   ['pack_int32',   'packInt32'],
    'pack_int64':   ['pack_int64',   'packInt64'],
    'pack_uint8':   ['pack_uint8',   'packUint8'],
    'pack_uint16':  ['pack_uint16',  'packUint16'],
    'pack_uint32':  ['pack_uint32',  'packUint32'],
    'pack_uint64':  ['pack_uint64',  'packUint64'],
    'pack_float32': ['pack_float32', 'packFloat32'],
    'pack_float64': ['pack_float64', 'packFloat64'],
    'pack_string':  ['pack_string',  'packString'],
    'pack_buffer':  ['pack_buffer',  'packBuffer'],
    'from_bool':    ['from_bool',    'fromBool'],
    'from_char':    ['from_char',    'fromChar'],
    'from_int8':    ['from_int8',    'fromInt8'],
    'from_int16':   ['from_int16',   'fromInt16'],
    'from_int32':   ['from_int32',   'fromInt32'],
    'from_int64':   ['from_int64',   'fromInt64'],
    'from_uint8':   ['from_uint8',   'fromUint8'],
    'from_uint16':  ['from_uint16',  'fromUint16'],
    'from_uint32':  ['from_uint32',  'fromUint32'],
    'from_uint64':  ['from_uint64',  'fromUint64'],
    'from_float32': ['from_float32', 'fromFloat32'],
    'from_float64': ['from_float64', 'fromFloat64'],
    'from_string':  ['from_string',  'fromString'],
    # These methods are for python array subscription
    'get_byte': ['__getitem__'],
    'set_byte': ['__setitem__'],

    # dcfile/parse.h
    # The camel versions of these names are temporarily disabled because of a pybindgen bug
    # We could probably circumvent the bug with some kind of patching system for 'pythonBindings.cpp'
    'read_dcfile':   ['read_dcfile'],   #readDCFile
    'parse_dcfile':  ['parse_dcfile'],  #parseDCFile
    'parse_dcvalue': ['parse_dcvalue'], #parseDCValue
}

classDocstrings = {
    'Module':
'A Module represents the complete list of Distributed Class definitions that define a particular ' +
'protocol. Typically, parsed from one or more \\".dc\\" or other bamboo protocol description files.',
    'DistributedType':
'A DistributedType represents any bamboo type. DistributedTypes have a defined layout of data.',
    'NumericType':
'A NumericType can represent any of the basic number types (ie. integers, floats, etc).  ' +
'A NumericType may also have a range and/or modulus to limit its possible values, '        +
'and/or a divisor representing a fixed-point numeric convention.  '                        +
'A divisor scales up any range or modulus to constrain up to (constraint * divisor).',
    'ArrayType':
'An ArrayType represents an array of some other kind of object, meaning ' +
'this parameter type accepts an arbitrary (or possibly fixed) number of ' +
' nested fields, all of which are of the same type.  '                    +
'Strings and blobs are arrays with char and uint8 elements respectively.',
    'Method':
'A Method is a field for a distributed Class that typically represents a remote procedure call.',
    'Struct':
'A Struct provides type composition by combining multiple Fields each with their own type.  ' +
'Structs may have both anonymous and named Fields.',
#    'MolecularField':
#'A MolecularField is an abstract field which provides an interface that can ' +
#'be used to access multiple other fields at the same time.',
}

functionDocstrings = {
    'dcfile' : {
    # parse.h
        'read_dcfile':
'Opens the given file or stream and parses it as a .dc file.  Classes defined in '       +
'the file are added to a new Module object, and a pointer to that object is returned.  ' +
'When reading from a stream, a filename is optional only used when reporting errors.',
       'parse_dcfile':
'Opens the given file or stream and parses it as a .dc file.  The distributed '              +
'classes defined in the file are added to the list of classes associated with the Module.  ' +
'When appending from a stream, a filename is optional only used to report errors.',
        'parse_dcvalue':
'Reads a .dc-formatted parameter value and outputs the data in wire form matching ' +
'the appropriate DistributedType and suitable for a default parameter value.  '     +
'If an error occurs, the error reason is returned instead of the parsed value.',
    },
}

methodDocstrings = {
    'Module': {
        'get_num_classes':
'Returns the number of classes in the module.',
        'get_num_structs':
'Returns the number of structs in the module.',
        'get_class':
'Returns the <n>th Class in the module.',
        'get_struct':
'Returns the <n>th Struct in the module.',
        'get_class_by_id':
'Returns the requested Class or None if there is no such class.',
        'get_class_by_name':
'Returns the requested Class or None if there is no such class.',
        'get_num_types':
'Returns the number of types in the module (classes, structs, typedefs).',
        'get_type_by_id':
'Returns the requested DistributedType or None if there is no such type.',
        'get_type_by_name':
'Returns the requested DistributedType or None if there is no such type.',
        'get_field_by_id':
'Returns the requested Field or None if there is no such type.',
        'get_num_imports':
'Returns the number of imports in the module.',
        'get_import':
'Returns the <n>th import in the module.',
        'has_keyword':
'Returns true if a keyword with the name <keyword> is declared in the module.',
        'get_num_keywords':
'Returns the number of keywords declared in the module.',
        'get_keyword':
'Returns the <n>th keyword declared in the module.',
        'add_class':
'Gives ownership of the Class to the Module returning false if there is a name conflict.',
        'add_struct':
'Gives ownership of the Struct to the Module returning false if there is a name conflict.',
        'add_import':
'Gives ownership of the Import to the Module, merging it with any duplicate modules.',
        'add_typedef':
'Creates a new alias in the module for a given type.',
        'add_keyword':
'Adds the keyword to the list of keywords accepted by the module.'
    },

    'DistributedType': {
        'get_subtype':
'Returns the type\'s fundamental type as an integer constant.',
        'has_fixed_size':
'Returns true if the DistributedType has a fixed size in bytes.',
        'get_size':
'Returns the size of the DistributedType in bytes or 0 if it is variable.',
        'has_alias':
'Returns true if this type was defined the an aliased name.',
        'as_numeric':
'Returns this as a NumericType if it is numeric, or None otherwise.',
        'as_array':
'Returns this as an ArrayType if it is an array, or None otherwise.',
        'as_struct':
'Returns this as a Struct if it is a struct, or None otherwise.',
        'as_method':
'Returns this as a Method if it is a method, or None otherwise.',
        'get_alias':
'Returns the name used to define the type, or the empty string.',
        'set_alias':
'Gives this type an alternate name <alias>.'
    },

    'NumericType': {
        'get_divisor':
'Retuns the divisior of the numeric, with a default value of one.',
       'has_modulus':
'Returns true if the numeric is constrained by a modulus.',
        'get_modulus':
'Returns a double precision floating-point representation of the modulus value.',
       'has_range':
'Returns true if the numeric is constrained by a range.',
        'get_range':
'Returns the NumericRange that constrains this type\'s values.',
        'set_divisor':
'Sets a divisor for the numeric type, typically to represent fixed point.  ' +
'Returns false if the divisor is not valid for this type.',
        'set_modulus':
'Sets a modulus for the numeric type, typically to represent fixed point.  ' +
'Returns false if the modulus is not valid for this type.',
        'set_range':
'Sets a range for the numeric type, typically to represent fixed point.  ' +
'Returns false if the range is not valid for this type.'
    },

    'ArrayType': {
        'get_element_type':
'Returns the type of the individual elements of this array.',
        'get_array_size':
'Returns the fixed number of elements in this array, or 0 ' +
'if the array may contain a variable number of elements.',
        'has_range':
'Returns true if there is a constraint on the range of valid array sizes.  ' +
'This is always true for fixed-size arrays.',
        'get_range':
'Returns the range of sizes that the array may have.'
    },

    'Method': {
        'get_num_parameters':
'Returns the number of parameters/arguments of the method.',
        'get_parameter':
'Returns the <n>th parameter of the method.',
        'get_parameter_by_name':
'Returns the requested parameter or None if there is no such param.',
        'add_parameter':
'Gives ownership of the Parameter to the Method returning false if there is a name conflict.',
    },

    'Struct': {
        'get_id':
'Returns a unique index number associated with this struct.',
        'get_name':
'Returns the name of this struct.',
        'get_module':
'Returns the Module object that contains the struct.',
        'get_num_fields':
'Returns the number of fields in the struct.',
        'get_field':
'Returns the <n>th field of the struct.',
        'get_field_by_id':
'Returns the field with the index <id>, or None if no such field exists.',
        'get_field_by_name':
'Returns the field with <name>, or None if no such field exists.',
        'add_field':
'Gives ownership of the Field to the Struct returning false if there is a name conflict.',
    },
}
