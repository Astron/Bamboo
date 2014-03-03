altnames = {
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

    # dcfile/parse.h
    # The sluggified versions of these names are temporarily disabled because of a pybindgen bug
    # We could probably circumvent the bug with some kind of patching system for 'pythonBindings.cpp'
    'read_dcfile':   ['read_dcfile'],   #readDCFile
    'parse_dcfile':  ['parse_dcfile'],  #parseDCFile
    'parse_dcvalue': ['parse_dcvalue'], #parseDCValue
}
docstrings = {
    # Module
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
'Adds the keyword to the list of keywords accepted by the module.',
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
'Returns false if the range is not valid for this type.',

    # DistributedType
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
'Gives this type an alternate name <alias>.',

    # dcfile/parse
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

}
