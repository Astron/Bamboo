PyObject *
_build_value_from_PyBambooValue(PyBambooValue *self)
{
    PyObject *py_retval;
    if self->obj
}


PyObject *
_custom_wrap_PyBambooValue_%s(PyBambooValue *PYBINDGEN_UNUSED(dummy), PyObject *args,
                              PyObject *kwargs, PyObject **return_exception)
{
    PyObject *py_retval;
    bamboo::Value retval;
    PyBambooType *type;
    bamboo::Type *type_ptr;
    const char *packed;
    Py_ssize_t packed_len;
    std::vector<uint8_t> packed_std;
    const char *keywords[] = {"type", "packed", NULL};
    PyBambooValue *py_Value;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "O!s#", (char **) keywords, &PyBambooType_Type, &type, &packed, &packed_len)) {
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
        PyErr_SetString((PyObject *) PyExc_TypeError, exc.what());
        return NULL;
    }
    py_Value = PyObject_New(PyBambooValue, &PyBambooValue_Type);
    py_Value->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
    py_Value->obj = new bamboo::Value(retval);
    py_retval = Py_BuildValue((char *) "N", py_Value);
    return py_retval;
}
