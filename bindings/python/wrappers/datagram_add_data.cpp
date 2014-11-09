PyObject *_custom_wrap_PyBambooDatagram_add_data(PyBambooDatagram *self, PyObject *args,
                                                 PyObject *kwargs, PyObject **return_exception)
{
    const char *keywords[] = {"value", nullptr};
    const char *value;
    Py_ssize_t value_len;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "s#", (char **) keywords, &value, &value_len)) {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return nullptr;
    }

    try {
        self->obj->add_data((const uint8_t *)value, value_len);
    } catch (bamboo::DatagramOverflow const &exc) {
        PyErr_SetString((PyObject *) Pybamboo__DatagramOverflow_Type, exc.what());
        return nullptr;
    }

    PyObject *py_retval;
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}

PyObject *_custom_wrap_PyBambooDatagram_addData(PyBambooDatagram *self, PyObject *args,
                                                 PyObject *kwargs, PyObject **return_exception)
{
    const char *keywords[] = {"value", nullptr};
    const char *value;
    Py_ssize_t value_len;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, (char *) "s#", (char **) keywords, &value, &value_len)) {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return nullptr;
    }

    try {
        self->obj->add_data((const uint8_t *)value, value_len);
    } catch (bamboo::DatagramOverflow const &exc) {
        PyErr_SetString((PyObject *) Pybamboo__DatagramOverflow_Type, exc.what());
        return nullptr;
    }

    PyObject *py_retval;
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}
