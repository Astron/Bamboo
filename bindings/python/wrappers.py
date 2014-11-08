import os

wrapperDir = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'wrappers')
def read_wrapper(filename):
    filepath = os.path.join(wrapperDir, filename)
    with open(filepath, 'r') as fd:
        wrapper = fd.read()
    return wrapper

wrapperNames = {
    'Datagram': '_custom_wrap_PyBambooDatagram_',
}

wrapperBodies = {
    'add_data': read_wrapper('datagram_add_data.cpp'),
    'data': read_wrapper('datagram_data.cpp'),
    'add_value': read_wrapper('pydatagram_add_value.cpp')
}
