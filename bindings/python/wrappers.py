wrapperNames = {
    'Value':    '_custom_wrap_PyBambooValue_',
    'Datagram': '_custom_wrap_PyBambooDatagram_'
}

wrapperBodies = {
    'from_packed':
'''
PyObject *
_custom_wrap_PyBambooValue_%s(PyBambooValue *PYBINDGEN_UNUSED(dummy), PyObject *args,
                              PyObject *kwargs, PyObject **return_exception)
{
    PyObject *py_retval;
    bamboo::Value retval;
    PyBambooDistributedType *type;
    bamboo::DistributedType *type_ptr;
    const char *packed;
    Py_ssize_t packed_len;
    std::vector<uint8_t> packed_std;
    const char *keywords[] = {"type", "packed", NULL};
    PyBambooValue *py_Value;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!s#", (char **) keywords, &PyBambooDistributedType_Type, &type, &packed, &packed_len)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return NULL;
    }
    type_ptr = (type ? type->obj : NULL);
    packed_std = std::vector<uint8_t>(&packed[0], &packed[packed_len]);
    try
    {
        retval = bamboo::Value::from_packed(type_ptr, packed_std);
    } catch (std::invalid_argument const &exc) {
        PyErr_SetString((PyObject *) Pystd__invalid_argument_Type, exc.what());
        return NULL;
    }
    py_Value = PyObject_New(PyBambooValue, &PyBambooValue_Type);
    py_Value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_Value->obj = new bamboo::Value(retval);
    py_retval = Py_BuildValue((char *) "N", py_Value);
    return py_retval;
}
''',
    'data':
'''
PyObject *
_custom_wrap_PyBambooDatagram_%s(PyBambooDatagram *self, PyObject **return_exception)
{
    PyObject *py_retval;
    const uint8_t *retval;
    Py_ssize_t retval_len;

    retval = self->obj->data();
    retval_len = self->obj->size();
    py_retval = Py_BuildValue((char *) "s#", retval, retval_len);
    return py_retval;
}
'''
}
