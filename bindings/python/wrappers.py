import os

wrapperDir = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'wrappers')
def read_wrapper(filename):
    filepath = os.path.join(wrapperDir, filename)
    with open(filepath, 'r') as fd:
        wrapper = fd.read()
    return wrapper

wrapperNames = {
    'Datagram': '_custom_wrap_PyBambooDatagram_',
    'DatagramIterator': '_custom_wrap_PyBambooDatagramIterator_',
}

wrapperBodies = {
    'add_data': read_wrapper('datagram_add_data.cpp'),
    'addData': '/* addData */',
    'add_value': read_wrapper('pydatagram_add_value.cpp'),
    'addValue': '/* add_value */',
    'data': read_wrapper('datagram_data.cpp'),
    'read_value': read_wrapper('pydatagramiterator_read_value.cpp'),
    'readValue': '/* read_value */',
}
