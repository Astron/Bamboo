#ifndef _CUSTOM_PARSE_VALUE_TUPLE
#define _CUSTOM_PARSE_VALUE_TUPLE
bool _custom_parse_char_value(PyObject *value_tuple, PyObject **return_exception, char &value)
{
    if(PyArg_ParseTuple(value_tuple, "c", &value)) {
        return true;
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return false;
    }
}

bool _custom_parse_int8_value(PyObject *value_tuple, PyObject **return_exception, int8_t &value)
{
    // Uses "B" (ignores overflow checking) because no option exists for signed 8-bit integers
    if(PyArg_ParseTuple(value_tuple, "B", &value)) {
        return true;
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return false;
    }
}

bool _custom_parse_int16_value(PyObject *value_tuple, PyObject **return_exception, int16_t &value)
{
    if(PyArg_ParseTuple(value_tuple, "h", &value)) {
        return true;
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return false;
    }
}

bool _custom_parse_int32_value(PyObject *value_tuple, PyObject **return_exception, int32_t &value)
{
    if(PyArg_ParseTuple(value_tuple, "i", &value)) {
        return true;
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return false;
    }
}

bool _custom_parse_int64_value(PyObject *value_tuple, PyObject **return_exception, int64_t &value)
{
    if(PyArg_ParseTuple(value_tuple, "L", &value)) { // "L" => "signed long long"
        return true;
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return false;
    }
}

bool _custom_parse_uint8_value(PyObject *value_tuple, PyObject **return_exception, uint8_t &value)
{
    if(PyArg_ParseTuple(value_tuple, "b", &value)) { // "b" => "unsigned char"
        return true;
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return false;
    }
}

bool _custom_parse_uint16_value(PyObject *value_tuple, PyObject **return_exception, uint16_t &value)
{
    if(PyArg_ParseTuple(value_tuple, "H", &value)) {
        return true;
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return false;
    }
}

bool _custom_parse_uint32_value(PyObject *value_tuple, PyObject **return_exception, uint32_t &value)
{
    if(PyArg_ParseTuple(value_tuple, "I", &value)) {
        return true;
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return false;
    }
}

bool _custom_parse_uint64_value(PyObject *value_tuple, PyObject **return_exception, uint64_t &value)
{
    if(PyArg_ParseTuple(value_tuple, "K", &value)) { // "K" => "unsigned long long"
        return true;
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return false;
    }
}

bool _custom_parse_float32_value(PyObject *value_tuple, PyObject **return_exception, float &value)
{
    if(PyArg_ParseTuple(value_tuple, "f", &value)) {
        return true;
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return false;
    }
}

bool _custom_parse_float64_value(PyObject *value_tuple, PyObject **return_exception, double &value)
{
    if(PyArg_ParseTuple(value_tuple, "d", &value)) {
        return true;
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return false;
    }
}

bool _custom_parse_buffer_value(PyObject *value_tuple, PyObject **return_exception,
                                const uint8_t *& value_ptr, Py_ssize_t& value_len)
{
    if(PyArg_ParseTuple(value_tuple, "s#", &value_ptr, &value_len)) {
        return true;
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return false;
    }
}
#endif // _CUSTOM_PARSE_VALUE_TUPLE

#ifndef _CUSTOM_PYDATAGRAM_PACK_TUPLE
#define _CUSTOM_PYDATAGRAM_PACK_TUPLE
bool _custom_PyBambooDatagram_pack_tuple(bamboo::Datagram *dg, const bamboo::Type *type,
                                         PyObject *value_tuple, PyObject **return_exception)
{
    std::cerr << bamboo::format_subtype(type->subtype()) << "\n";
    switch(type->subtype()) {
    case bamboo::kTypeChar: {
        char value;
        bool ok = _custom_parse_char_value(value_tuple, return_exception, value);
        if(ok) { dg->add_char(value); }
        else { return false; }
    }
    break;
    case bamboo::kTypeInt8: {
        int8_t value;
        bool ok = _custom_parse_int8_value(value_tuple, return_exception, value);
        if(ok) { dg->add_int8(value); }
        else { return false; }
    }
    break;
    case bamboo::kTypeInt16: {
        int16_t value;
        bool ok = _custom_parse_int16_value(value_tuple, return_exception, value);
        if(ok) { dg->add_int16(value); }
        else { return false; }
    }
    break;
    case bamboo::kTypeInt32: {
        int32_t value;
        bool ok = _custom_parse_int32_value(value_tuple, return_exception, value);
        if(ok) { dg->add_int32(value); }
        else { return false; }
    }
    break;
    case bamboo::kTypeInt64: {
        int64_t value;
        bool ok = _custom_parse_int64_value(value_tuple, return_exception, value);
        if(ok) { dg->add_int64(value); }
        else { return false; }
    }
    break;
    case bamboo::kTypeUint8: {
        uint8_t value;
        bool ok = _custom_parse_uint8_value(value_tuple, return_exception, value);
        if(ok) { dg->add_uint8(value); }
        else { return false; }
    }
    break;
    case bamboo::kTypeUint16: {
        uint16_t value;
        bool ok = _custom_parse_uint16_value(value_tuple, return_exception, value);
        if(ok) { dg->add_uint16(value); }
        else { return false; }
    }
    break;
    case bamboo::kTypeUint32: {
        uint32_t value;
        bool ok = _custom_parse_uint32_value(value_tuple, return_exception, value);
        if(ok) { dg->add_uint32(value); }
        else { return false; }
    }
    break;
    case bamboo::kTypeUint64: {
        uint64_t value;
        bool ok = _custom_parse_uint64_value(value_tuple, return_exception, value);
        if(ok) { dg->add_uint64(value); }
        else { return false; }
    }
    break;
    case bamboo::kTypeFloat32: {
        float value;
        bool ok = _custom_parse_float32_value(value_tuple, return_exception, value);
        if(ok) { dg->add_float32(value); }
        else { return false; }
    }
    break;
    case bamboo::kTypeFloat64: {
        double value;
        bool ok = _custom_parse_float64_value(value_tuple, return_exception, value);
        if(ok) { dg->add_float64(value); }
        else { return false; }
    }
    break;
    case bamboo::kTypeString:
    case bamboo::kTypeBlob: {
        const uint8_t *value_ptr;
        Py_ssize_t value_len;
        bool ok = _custom_parse_buffer_value(value_tuple, return_exception, value_ptr, value_len);
        if(ok) { dg->add_data(value_ptr, value_len); }
        else { return false; }
    }
    break;
    case bamboo::kTypeVarstring:
    case bamboo::kTypeVarblob: {
        const uint8_t *value_ptr;
        Py_ssize_t value_len;
        bool ok = _custom_parse_buffer_value(value_tuple, return_exception, value_ptr, value_len);
        if(ok) { dg->add_blob(value_ptr, value_len); }
        else { return false; }
    }
    break;
    case bamboo::kTypeArray: {
        bamboo::sizetag_t len = type->fixed_size();
        const bamboo::Array *arr = type->as_array();
        const bamboo::Type *element_type = arr->element_type();

        Py_ssize_t value_len = PyTuple_Size(value_tuple);
        if(Py_ssize_t(len) != value_len) {
            std::stringstream error;
            error << "Datagram tried to add array data, but was given " << value_len
                  << " elements for a fixed array of length " << len << ".\n";
            PyErr_SetString((PyObject *) PyExc_RuntimeError, error.str().c_str());
            return false;
        }

        for(int i = 0; i < value_len; ++i) {
            PyObject *element_tuple = PyTuple_GetSlice(value_tuple, i, i+1);
            bool ok = _custom_PyBambooDatagram_pack_tuple(dg, element_type, element_tuple, return_exception);
            if(!ok) { return false; }
        }
    }
    break;
    case bamboo::kTypeVararray: {
        const bamboo::Array *arr = type->as_array();
        const bamboo::Type *element_type = arr->element_type();

        Py_ssize_t value_len = PyTuple_Size(value_tuple);
        for(int i = 0; i < value_len; ++i) {
            PyObject *element_tuple = PyTuple_GetSlice(value_tuple, i, i+1);
            bool ok = _custom_PyBambooDatagram_pack_tuple(dg, element_type, element_tuple, return_exception);
            if(!ok) { return false; }
        }
    }
    break;
    case bamboo::kTypeStruct: {
        const bamboo::Struct *struct_ = type->as_struct();
        size_t num_fields = struct_->num_fields();

        Py_ssize_t value_num_fields = PyTuple_Size(value_tuple);
        if(Py_ssize_t(num_fields) != value_num_fields) {
            std::stringstream error;
            error << "Datagram tried to add struct data, but was given " << value_num_fields
                  << " fields for a struct with " << num_fields << " fields.\n";
            PyErr_SetString((PyObject *) PyExc_RuntimeError, error.str().c_str());
            return false;
        }

        for(int i = 0; i < value_num_fields; ++i) {
            const bamboo::Type *field_type = struct_->get_field((unsigned int)i)->type();
            PyObject *field_tuple = PyTuple_GetSlice(value_tuple, i, i+1);
            bool ok = _custom_PyBambooDatagram_pack_tuple(dg, field_type, field_tuple, return_exception);
            if(!ok) { return false; }
        }
    }
    break;
    case bamboo::kTypeMethod: {
        const bamboo::Method *method = type->as_method();
        size_t num_params = method->num_parameters();

        Py_ssize_t value_num_params = PyTuple_Size(value_tuple);
        if(Py_ssize_t(num_params) != value_num_params) {
            std::stringstream error;
            error << "Datagram tried to add method data, but was given " << value_num_params
                  << " params for a method with " << num_params << " params.\n";
            PyErr_SetString((PyObject *) PyExc_RuntimeError, error.str().c_str());
            return false;
        }

        for(int i = 0; i < value_num_params; ++i) {
            const bamboo::Type *param_type = method->get_parameter((unsigned int)i)->type();
            PyObject *param_tuple = PyTuple_GetSlice(value_tuple, i, i+1);
            bool ok = _custom_PyBambooDatagram_pack_tuple(dg, param_type, param_tuple, return_exception);
            if(!ok) { return false; }
        }
    }
    break;
    case bamboo::kTypeInvalid:
        PyErr_SetString((PyObject *) PyExc_RuntimeError, "Can't pack value for type with subtype 'invalid'");
        return false;
    }

    return true;
}
#endif // _CUSTOM_PYDATAGRAM_PACK_TUPLE

PyObject *_custom_wrap_PyBambooDatagram_%s(PyBambooDatagram *self, PyObject *args, PyObject **return_exception)
{
    PyObject *py_retval;
    PyBambooType *type;
    bamboo::Type *type_ptr;
    PyObject *value_tuple;

    /* parse arguments */
    {
        // We are mixing positional args and variadic args, so we can't parse the arguments with
        // a predefined format string.  Instead we slice the required args out of the args tuple.
        //
        // The desired python prototype:
        //     def add_value(type, *args)

        if(PyTuple_Size(args) < 1) {
            PyObject *exc_type, *traceback;
            PyArg_ParseTuple(args, "O!", &PyBambooType_Type, &type);
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
            return NULL;
        }

        PyObject *req_args = PyTuple_GetSlice(args, 0, 1);
        if(PyArg_ParseTuple(req_args, "O!", &PyBambooType_Type, &type)) {
            type_ptr = (type ? type->obj : NULL);
            value_tuple = PyTuple_GetSlice(args, 1, PyTuple_Size(args));
        } else {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
            return NULL;
        }
    }

    /* decompose tuple value */
    try {
        bool ok = _custom_PyBambooDatagram_pack_tuple(self->obj, type_ptr, value_tuple, return_exception);
        if(!ok) { return NULL; }
    } catch (bamboo::DatagramOverflow const &exc) {
        PyErr_SetString((PyObject *) Pybamboo__DatagramOverflow_Type, exc.what());
        return NULL;
    }

    /* no return value */
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}
