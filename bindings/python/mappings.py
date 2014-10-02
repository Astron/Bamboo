altnames = {
    # Common alternate names
    'cap':  ['cap'],
    'size': ['size', '__len__'],
    'copy': ['copy', '__deepcopy__'],
    'tell': ['tell'],
    'seek': ['seek'],
    'skip': ['skip'],
    'type': ['type'],
    'data': ['data'],
    'next': ['next'],
    'prev': ['prev'],
    'to_string': ['to_string', 'toString', '__str__', '__repr__'],

    # Explicit Builtins
    '_add_':      ['__add__'],
    '_sub_':      ['__sub__'],
    '_radd_':     ['__radd__'],
    '_rsub_':     ['__rsub__'],
    '_len_':      ['__len__'],
    '_iter_':     ['__iter__'],
    '_getitem_':  ['__getitem__'],
    '_setitem_':  ['__setitem__'],
    '_getslice_': ['__getslice__'],
    '_setslice_': ['__setslice__'],

    # Value
    'from_type':   ['from_type',   'fromType'],
    'from_packed': ['from_packed', 'fromPacked'],
    'pack':        ['pack'],

    # Module
    'num_classes':   ['num_classes',   'numClasses'],
    'num_structs':   ['num_structs',   'numStructs'],
    'get_class':     ['get_class',     'getClass'],
    'get_struct':    ['get_struct',    'getStruct'],
    'class_by_id':   ['class_by_id',   'classById'],
    'class_by_name': ['class_by_name', 'classByName'],
    'num_types':     ['num_types',     'numTypes'],
    'type_by_id':    ['type_by_id',    'typeById'],
    'type_by_name':  ['type_by_name',  'typeByName'],
    'field_by_id':   ['field_by_id',   'fieldById'],
    'num_imports':   ['num_imports',   'numImports'],
    'get_import':    ['get_import',    'getImport'],
    'has_keyword':   ['has_keyword',   'hasKeyword'],
    'num_keywords':  ['num_keywords',  'numKeywords'],
    'get_keyword':   ['get_keyword',   'getKeyword'],
    'add_class':     ['add_class',     'addClass'],
    'add_struct':    ['add_struct',    'addStruct'],
    'add_import':    ['add_import',    'addImport'],
    'add_typedef':   ['add_typedef',   'addTypedef'],
    'add_keyword':   ['add_keyword',   'addKeyword'],

    # Type
    'subtype':        ['subtype'],
    'has_fixed_size': ['has_fixed_size', 'hasFixedSize'],
    'fixed_size':     ['fixed_size'],
    'has_alias':      ['has_alias',      'hasAlias'],
    'as_numeric':     ['as_numeric',     'asNumeric'],
    'as_array':       ['as_array',       'asArray'],
    'as_struct':      ['as_struct',      'asStruct'],
    'as_method':      ['as_method',      'asMethod'],
    'alias':          ['alias'],
    'set_alias':      ['set_alias',      'setAlias'],

    # Numeric
    'divisor':     ['divisor'],
    'has_modulus': ['has_modulus', 'hasModulus'],
    'modulus':     ['modulus'],
    'has_range':   ['has_range',   'hasRange'],
    'range':       ['range'],
    'set_divisor': ['set_divisor', 'setDivisor'],
    'set_modulus': ['set_modulus', 'setModulus'],
    'set_range':   ['set_range',   'setRange'],

    # Array
    'element_type': ['element_type', 'elementType'],
    'array_size':   ['array_size',   'arraySize'],
    #'has_range':   ['has_range',    'hasRange'],
    #'range':       ['range'],

    # Method
    'num_parameters':    ['num_parameters',    'numParameters'],
    'get_parameter':     ['get_parameter',     'getParameter'],
    'parameter_by_name': ['parameter_by_name', 'parameterByName'],
    'add_parameter':     ['add_parameter',     'addParameter'],

    # Struct
    'id':            ['id'],
    'name':          ['name'],
    'module':        ['module'],
    'num_fields':    ['num_fields',    'numFields'],
    'get_field':     ['get_field',     'getField'],
    'field_by_id':   ['field_by_id',   'fieldById'],
    'field_by_name': ['field_by_name', 'fieldByName'],
    'add_field':     ['add_field',     'addField'],
    'as_class':      ['as_class'],

    # Class
    'num_parents':     ['num_parents',     'numParents'],
    'get_parent':      ['get_parent',      'getParent'],
    'num_children':    ['num_children',    'numChildren'],
    'get_child':       ['get_child',       'getChild'],
    'has_constructor': ['has_constructor', 'hasConstructor'],
    'constructor':     ['constructor'],
    'num_base_fields': ['num_base_fields', 'numBaseFields'],
    'get_base_field':  ['get_base_field',  'getBaseField'],
    'add_parent':      ['add_parent',      'addParent'],

    # Parameter
    #'name':             ['name'],
    'type':              ['type'],
    'get_method':        ['get_method',        'getMethod'],
    'has_default_value': ['has_default_value', 'hasDefaultValue'],
    'default_value':     ['default_value',     'defaultValue'],
    'set_name':          ['set_name',          'setName'],
    'set_type':          ['set_type',          'setType'],
    'set_default_value': ['set_default_value', 'setDefaultValue'],

    # Field
    #'id':                ['id'],
    #'name':              ['name'],
    #'type':              ['type'],
    'record':            ['record'],
    #'has_default_value': ['has_default_value', 'hasDefaultValue'],
    #'default_value':     ['default_value',     'defaultValue'],
    #'set_name':          ['set_name',          'setName'],
    #'set_type':          ['set_type',          'setType'],
    #'set_default_value': ['set_default_value', 'setDefaultValue'],

    # Datagram
    'add_bool':    ['add_bool',    'addBool'],
    'add_char':    ['add_char',    'addChar'],
    'add_int8':    ['add_int8',    'addInt8'],
    'add_int16':   ['add_int16',   'addInt16'],
    'add_int32':   ['add_int32',   'addInt32'],
    'add_int64':   ['add_int64',   'addInt64'],
    'add_uint8':   ['add_uint8',   'addUint8'],
    'add_uint16':  ['add_uint16',  'addUint16'],
    'add_uint32':  ['add_uint32',  'addUint32'],
    'add_uint64':  ['add_uint64',  'addUint64'],
    'add_float32': ['add_float32', 'addFloat32'],
    'add_float64': ['add_float64', 'addFloat64'],
    'add_size':    ['add_size',    'addSize'],
    'add_data':    ['add_data',    'addData'],
    'add_string':  ['add_string',  'addString'],
    'add_blob':    ['add_blob',    'addBlob'],
    'add_value':   ['add_value',   'addValue'],
    'add_packed':  ['add_packed',  'addPacked'],

    # Datagram Iterator
    'read_bool':      ['read_bool',      'readBool'],
    'read_char':      ['read_char',      'readChar'],
    'read_int8':      ['read_int8',      'readInt8'],
    'read_int16':     ['read_int16',     'readInt16'],
    'read_int32':     ['read_int32',     'readInt32'],
    'read_int64':     ['read_int64',     'readInt64'],
    'read_uint8':     ['read_uint8',     'readUint8'],
    'read_uint16':    ['read_uint16',    'readUint16'],
    'read_uint32':    ['read_uint32',    'readUint32'],
    'read_uint64':    ['read_uint64',    'readUint64'],
    'read_float32':   ['read_float32',   'readFloat32'],
    'read_float64':   ['read_float64',   'readFloat64'],
    'read_size':      ['read_size',      'readSize'],
    'read_string':    ['read_string',    'readString'],
    'read_blob':      ['read_blob',      'readBlob'],
    'read_datagram':  ['read_datagram',  'readDatagram'],
    'read_data':      ['read_data',      'readData'],
    'read_remainder': ['read_remainder', 'readRemainder'],
    'read_value':     ['read_value',     'readValue'],
    'read_packed':    ['read_packed',    'readPacked'],
    'remaining':      ['remaining'],
    'skip_type':      ['skip_type',      'skipType'],

    # traits/hashes.h
    'legacy_hash': ['legacy_hash', 'legacyHash'],

    # dcfile/parse.h
    'read_dcfile':   ['read_dcfile',   'readDCFile'],
    'parse_dcfile':  ['parse_dcfile',  'parseDCFile'],
    'parse_dcvalue': ['parse_dcvalue', 'parseDCValue'],
}

classDocstrings = {
    'Module':
'A Module represents the complete list of Distributed Class definitions that define a particular ' +
'protocol. Typically, parsed from one or more \\".dc\\" or other bamboo protocol description files.',
    'Type':
'A Type represents any bamboo type. Types have a defined layout of data.',
    'Numeric':
'A Numeric can represent any of the basic number types (ie. integers, floats, etc).  ' +
'A Numeric may also have a range and/or modulus to limit its possible values, '        +
'and/or a divisor representing a fixed-point numeric convention.  '                        +
'A divisor scales up any range or modulus to constrain up to (constraint * divisor).',
    'Array':
'An Array represents an array of some other kind of object, meaning ' +
'this parameter type accepts an arbitrary (or possibly fixed) number of ' +
' nested fields, all of which are of the same type.  '                    +
'Strings and blobs are arrays with char and uint8 elements respectively.',
    'Method':
'A Method is a field for a distributed Class that typically represents a remote procedure call.',
    'Struct':
'A Struct provides type composition by combining multiple Fields each with their own type.  ' +
'Structs may have both anonymous and named Fields.',
    'Class':
'A Class is a special type of struct that have a couple advanced object-oriented features; ' +
'Classes can inherit from other classes (ie. have super-/sub-classes), '                     +
'Classes can have methods including a special constructor method, '                          +
'Unlike other structs, classes cannot have anonymous fields.',
    'Parameter':
'A Parameter is a single argument/parameter of a Method.',
    'Field':
'A Field is a member of a class or struct.',
    'MolecularField':
'A MolecularField is an abstract field which provides an interface that can ' +
'be used to access multiple other fields at the same time.',
    'Value':
'A Value is a local representation of data.  '                                +
'The library uses the Value class to do some magic for us that allows us to ' +
'manipulate dynamically-typed data similar freely in a loosely-typed language.',
    'Datagram':
'A Datagram is a buffer of binary data in network-endianness.',
    'DatagramIterator':
'A DatagramIterator lets you step trough a datagram by reading a single value at a time.',
}

functionDocstrings = {
    'traits': {
    # hashes.h
        'legacy_hash':
'Produces a hash which matches that of the legacy dcparser in Panda3D.',
    },
    'dcfile': {
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
'the appropriate Type and suitable for a default parameter value.  '     +
'If an error occurs, the error reason is returned instead of the parsed value.',
    },
}

methodDocstrings = {
    'Value': {
        'from_type':
'Returns the default Value for the provided type.',
        'from_packed':
'Returns the Value for the provided type unpacked from a buffer.',
        'pack':
'Returns a buffer containing the Value in packed representation for the provided type.',
        'size':
'Returns the size (the number of items) in a value.  ' +
'Size may be called on an array, string, blob, struct, or function value.',

    },

    'Module': {
        'num_classes':
'Returns the number of classes in the module.',
        'num_structs':
'Returns the number of structs in the module.',
        'get_class':
'Returns the <n>th Class in the module.',
        'get_struct':
'Returns the <n>th Struct in the module.',
        'class_by_id':
'Returns the requested Class or None if there is no such class.',
        'class_by_name':
'Returns the requested Class or None if there is no such class.',
        'num_types':
'Returns the number of types in the module (classes, structs, typedefs).',
        'type_by_id':
'Returns the requested Type or None if there is no such type.',
        'type_by_name':
'Returns the requested Type or None if there is no such type.',
        'field_by_id':
'Returns the requested Field or None if there is no such type.',
        'num_imports':
'Returns the number of imports in the module.',
        'get_import':
'Returns the <n>th import in the module.',
        'has_keyword':
'Returns true if a keyword with the name <keyword> is declared in the module.',
        'num_keywords':
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

    'Type': {
        'subtype':
'Returns the type\'s fundamental type as an integer constant.',
        'has_fixed_size':
'Returns true if the Type has a fixed size in bytes.',
        'fixed_size':
'Returns the size of the Type in bytes or 0 if it is variable.',
        'has_alias':
'Returns true if this type was defined the an aliased name.',
        'as_numeric':
'Returns this as a Numeric if it is numeric, or None otherwise.',
        'as_array':
'Returns this as an Array if it is an array, or None otherwise.',
        'as_struct':
'Returns this as a Struct if it is a struct, or None otherwise.',
        'as_method':
'Returns this as a Method if it is a method, or None otherwise.',
        'alias':
'Returns the name used to define the type, or the empty string.',
        'set_alias':
'Gives this type an alternate name <alias>.'
    },

    'Numeric': {
        'divisor':
'Retuns the divisior of the numeric, with a default value of one.',
       'has_modulus':
'Returns true if the numeric is constrained by a modulus.',
        'modulus':
'Returns a double precision floating-point representation of the modulus value.',
       'has_range':
'Returns true if the numeric is constrained by a range.',
        'range':
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

    'Array': {
        'element_type':
'Returns the type of the individual elements of this array.',
        'array_size':
'Returns the fixed number of elements in this array, or 0 ' +
'if the array may contain a variable number of elements.',
        'has_range':
'Returns true if there is a constraint on the range of valid array sizes.  ' +
'This is always true for fixed-size arrays.',
        'range':
'Returns the range of sizes that the array may have.'
    },

    'Method': {
        'num_parameters':
'Returns the number of parameters/arguments of the method.',
        'get_parameter':
'Returns the <n>th parameter of the method.',
        'parameter_by_name':
'Returns the requested parameter or None if there is no such param.',
        'add_parameter':
'Gives ownership of the Parameter to the Method returning false if there is a name conflict.',
    },

    'Struct': {
        'id':
'Returns a unique index number associated with this struct.',
        'name':
'Returns the name of this struct.',
        'module':
'Returns the Module object that contains the struct.',
        'num_fields':
'Returns the number of fields in the struct.',
        'get_field':
'Returns the <n>th field of the struct.',
        'field_by_id':
'Returns the field with the index <id>, or None if no such field exists.',
        'field_by_name':
'Returns the field with <name>, or None if no such field exists.',
        'add_field':
'Gives ownership of the Field to the Struct returning false if there is a name conflict.',
    },

    'Class': {
        'num_parents':
'Returns the number of superclasses this class inherits from.',
        'get_parent':
'Returns the <n>th parent-/super-class this class inherits from.',
        'num_children':
'Returns the number of subclasses that inherit from this class.',
        'get_child':
'Returns the <n>th child-/sub-class that inherits this class.',
        'has_constructor':
'Returns true if this class has a constructor method, ' +
'or false if it just uses the default constructor.',
        'constructor':
'Returns the constructor method for this class if it is defined, ' +
'or nullptr if the class uses the default constructor.',
        'num_base_fields':
'Returns the number of fields declared directly in this class.',
        'get_base_field':
'Returns the <n>th field from the class excluding any inherited fields.',
        'add_parent':
'Set this class as a subclass to target parent.',
    },

    'Parameter': {
        'name':
'Returns the parameter\'s name.  An unnamed parameter returns the empty string.',
        'type':
'Returns the Type of the Parameter.',
        'get_method':
'Returns the Method that contains the Parameter.',
        'has_default_value':
'Returns true if a default value was defined for this parameter.',
        'default_value':
'Returns the default value for this parameter.  ' +
'If a default value hasn\'t been set, returns an implicit default.',
        'set_name':
'Sets the name of this parameter.  Returns false if a parameter with ' +
'the same name already exists in the containing method.',
        'set_type':
'Sets the Type of the parameter and clear\'s the default value.  ' +
'Returns false if a parameter cannot represent <type>.',
        'set_default_value':
'Defines a default value for this parameter.  ' +
'Returns false if the value is invalid for the parameter\'s type.',
    },

    'Field': {
        'id':
'Returns a unique index number associated with this field.',
        'name':
'Returns the field\'s name.  An unnamed field returns the empty string.',
        'type':
'Returns the Type of the field.',
        'record':
'Returns the Class or Struct that contains this field.',
        'has_default_value':
'Returns true if a default value was defined for this field.',
        'default_value':
'Returns the default value for this field.  ' +
'If a default value hasn\'t been set, returns an implicit default.',
        'set_name':
'Sets the name of this field.  Returns false if a field with ' +
'the same name already exists in the containing method.',
        'set_type':
'Sets the Type of the field and clear\'s the default value.  ' +
'Returns false if a field cannot represent <type>.',
        'set_default_value':
'Defines a default value for this field.  ' +
'Returns false if the value is invalid for the field\'s type.',
    },

    'Datagram': {
        'add_bool':
'Adds an 8-bit integer to the datagram that is guaranteed ' +
'to be one of the values 0x00 (false) or 0x01 (true).',
        'add_char':
'Adds an 8-bit ascii-character value to the datagram.',
        'add_int8':
'Adds a signed 8-bit integer value to the datagram.',
        'add_int16':
'Adds a signed 16-bit integer value to the datagram arranged in little-endian.',
        'add_int32':
'Adds a signed 32-bit integer value to the datagram arranged in little-endian.',
        'add_int64':
'Adds a signed 64-bit integer value to the datagram arranged in little-endian.',
        'add_uint8':
'Adds an unsigned 8-bit integer value to the datagram.',
        'add_uint16':
'Adds an unsigned 16-bit integer value to the datagram arranged in little-endian.',
        'add_uint32':
'Adds an unsigned 32-bit integer value to the datagram arranged in little-endian.',
        'add_uint64':
'Adds an unsigned 64-bit integer value to the datagram arranged in little-endian.',
        'add_float32':
'Adds a float (32-bit IEEE 754 floating point) value to the datagram',
        'add_float64':
'Adds a double (64-bit IEEE 754 floating point) value to the datagram.',
        'add_size':
'Adds a length-tag to the datagram.',
        'add_data':
'Adds bytes directly to the end of the datagram.',
        'add_string':
'Adds a string to the datagram; a length tag (typically a uint16_t) ' +
'is prepended to the string before it is added.',
        'add_blob':
'Adds a blob to the datagram from bytes; a length tag (typically a uint16_t) ' +
'is prepended to the blob before it is added.',
        'add_value':
'Adds a Value with the given type packed into the datagram, converting ' +
'byte-order from native-endianess to wire-endianess (if necessary).',
        'add_packed':
'Adds data from a packed value, returning the number of bytes read from the packed data.',
        'size':
'Returns the amount of data added to the datagram in bytes.',
        'cap':
'Returns the currently allocated size of the datagram in memory (ie. capacity).',
        'data':
'Returns a copy of the bytes in the Datagram.',
    },

    'DatagramIterator': {
        'read_bool':
'Reads the next byte from the datagram and returns either false or true.',
        'read_char':
'Reads a byte from the datagram, returning an 8-bit ascii character.',
        'read_int8':
'Reads a byte from the datagram, returning a signed 8-bit integer.',
        'read_int16':
'Reads 2 bytes from the datagram, returning a signed 16-bit integer in native endianness.',
        'read_int32':
'Reads 4 bytes from the datagram, returning a signed 32-bit integer in native endianness.',
        'read_int64':
'Reads 8 bytes from the datagram, returning a signed 64-bit integer in native endianness.',
        'read_uint8':
'Reads a byte from the datagram, returning an usigned 8-bit integer.',
        'read_uint16':
'Reads 2 bytes from the datagram, returning an usigned 16-bit integer in native endianness.',
        'read_uint32':
'Reads 4 bytes from the datagram, returning an usigned 32-bit integer in native endianness.',
        'read_uint64':
'Reads 8 bytes from the datagram, returning an usigned 64-bit integer in native endianness.',
        'read_size':
'Reads a length-tag from the datagram.',
        'read_float32':
'Reads reads 4 bytes from the datagram, returning a 32-bit float in native endianness.',
        'read_float64':
'Reads reads 8 bytes from the datagram, returning a 64-bit float in native endianness.',
        'read_string':
'Reads a length, then reads a string of that length from the datagram.',
        'read_blob':
'Reads a length, then reads a blob of that length from the datagram.',
        'read_datagram':
'Reads a blob from the datagram and returns it as another datagram.',
        'read_data':
'Reads the next N bytes in the datagram..',
        'read_remainder':
'Reads the remainder of the datagram.',
        'read_value':
'Reads the Value for a Type from the datagram.',
        'read_packed':
'Reads a Value for a Type and returns it as a buffer converted to native endianness.',
        'remaining':
'Returns the number of unread bytes left in the Datagram.',
        'tell':
'Returns the current offset in bytes of the DatagramIterator into the datagram,.',
        'seek':
'Sets the current offset in bytes of the DatagramIterator into the datagram.',
        'skip':
'Increments the current offset into the datagram by N.  ' +
'Throws DatagramIteratorEOF if it skips past the end of the datagram.',
        'skip_type':
'Seeks past the packed data for a Type.  ' +
'Throws DatagramIteratorEOF if it skips past the end of the datagram.',
    },
}
