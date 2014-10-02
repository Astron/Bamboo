#!/usr/bin/env python3

from bamboo import dcfile, module

astron_keywords = ['clsend', 'ownsend', 'clrecv', 'ownrecv',
                   'airecv', 'broadcast', 'ram', 'required', 'db']

mod = module.Module()
for word in astron_keywords:
    mod.add_keyword(word)
dcfile.parse_dcfile(mod, 'simple_example.dc')

print() # Newline after prompt

# Inspect python imports
print('Imports')
for import_num in range(0, mod.num_imports()):
    # python module path
    pyimport = mod.get_import(import_num)
    print('  ' + pyimport.module)
    for symbol in pyimport.symbols:
        # Figure out the names of used classes
        fragments = str(symbol).split('/')
        base_class = fragments[0]
        suffixes = fragments[1:] + ['']
        for suffix in suffixes:
            print('    ' + base_class + suffix)

print() # Newline after imports

# Inspect module keywords
print('Keywords')
for keyword_num in range(0, mod.num_keywords()):
    print('  ' + mod.get_keyword(keyword_num))

print() # Newline after keywords

# Helper functions for printing complex types
def print_type(typ, indent):
    tab = '  ' * indent
    subtype = typ.subtype()
    if subtype == module.kTypeMethod:
        method = typ.as_method()
        for param_num in range(0, method.num_parameters()):
            param = method.get_parameter(param_num)
            if not param.name():
                print(tab + param.type().to_string())
            else:
                print(tab + param.name() + ' : ' + param.type().to_string())
            print_type(param.type(), indent + 1)

    elif subtype == module.kTypeStruct:
        struct = typ.as_struct()
        for field_num in range(0, struct.num_fields()):
            field = struct.get_field(field_num)
            if not field.name():
                print(tab + field.type().to_string())
            else:
                print(tab + field.name() + ' : ' + field.type().to_string())
            print_type(field.type(), indent + 1)

# Inspect module classes
print('Classes')
for class_num in range(0, mod.num_classes()):
    cls = mod.get_class(class_num)
    print('  ' + cls.name())
    for field_num in range(0, cls.num_fields()):
        field = cls.get_field(field_num)
        print('    ' + field.name() + ' : ' + field.type().to_string())
        print_type(field.type(), 3)

print() # Newline after classes

print('Fields-By-Id')
for i in range(0, 6):
    print('  %s - %s' % (i, mod.field_by_id(i).name()))

print() # Newline before prompt
