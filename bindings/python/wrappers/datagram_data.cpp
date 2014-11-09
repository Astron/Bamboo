PyObject *_custom_wrap_PyBambooDatagram_data(PyBambooDatagram *self, PyObject **return_exception)
{
    PyObject *py_retval;
    const uint8_t *retval;
    Py_ssize_t retval_len;

    retval = self->obj->data();
    retval_len = self->obj->size();
    py_retval = Py_BuildValue((char *) "s#", retval, retval_len);
    return py_retval;
}
