altnames = {
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
    # The sluggified versions of these names are temporarily disabled because of a pybindgen bug
    # We could probably circumvent the bug with some kind of patching system for 'pythonBindings.cpp'
    'read_dcfile':    ['read_dcfile'],   #readDCFile
    'parse_dcfile':   ['parse_dcfile'],  #parseDCFile
    'parse_dcvalue':  ['parse_dcvalue'], #parseDCValue
}
docstrings = {
    'get_subtype':
'Returns the type\'s fundamental type as an integer constant.',

    'has_fixed_size':
'Returns true if the DistributedType has a fixed size in bytes.',

    'get_size':
'Returns the size of the DistributedType in bytes or 0 if it is variable.',

    'has_alias':
'Returns true if this type was defined the an aliased name.',

    'as_numeric':
'Returns this as a NumericType if it is numeric, or nullptr otherwise.',

    'as_array':
'Returns this as an ArrayType if it is an array, or nullptr otherwise.',

    'as_struct':
'Returns this as a Struct if it is a struct, or nullptr otherwise.',

    'as_method':
'Returns this as a Method if it is a method, or nullptr otherwise.',

    'get_alias':
'Returns the name used to define the type, or the empty string.',

    'set_alias':
'Gives this type an alternate name <alias>.',

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
