#include <limits> // std::numeric_limits
#include <bytesobject.h>  // python2.6+ compat with BYTES_*

static char _custom_py_float_is_not_integer[] = "integer argument expected, got float";
static char _custom_numeric_has_bad_bytesize[] = "can't pack value for numeric with invalid bytesize";
static char *_custom_py_pack_typerr(const char *expected, PyObject *py_value, char *msgbuf, size_t bufsize)
{
    assert(expected != NULL);
    assert(py_value != NULL);
    PyOS_snprintf(msgbuf, bufsize,
                  "must be %.50s, not %.50s", expected,
                  py_value == Py_None ? "None" : py_value->ob_type->tp_name);
    return msgbuf;
}

static char *_custom_PyBambooDatagram_py_pack(bamboo::Datagram *dg, const bamboo::Type *type,
                                              PyObject *py_value, char *msgbuf, size_t bufsize)
{

#define PY_PACK_ERROR msgbuf

    switch(type->subtype()) {
    case bamboo::kTypeChar:
        if(!PyBytes_Check(py_value) && PyBytes_Size(py_value) == 1) {
            return _custom_py_pack_typerr("a byte string of length 1", py_value, msgbuf, bufsize);
        }
        dg->add_char(PyBytes_AS_STRING(py_value)[0]);
        break;
    case bamboo::kTypeInt8:
        if(!PyFloat_Check(py_value)) {
            long value = PyLong_AsLong(py_value);
            if(PyErr_Occurred()) { return PY_PACK_ERROR; }
            if(value < std::numeric_limits<int8_t>::lowest()) {
                PyErr_SetString(PyExc_OverflowError, "int8 is less than minimum");
                return PY_PACK_ERROR;
            } else if(value > std::numeric_limits<int8_t>::max()) {
                PyErr_SetString(PyExc_OverflowError, "int8 is greater than maximum");
                return PY_PACK_ERROR;
            }
            dg->add_int8((int8_t)value);
        } else {
            return _custom_py_float_is_not_integer;
        }
        break;
    case bamboo::kTypeInt16:
        if(!PyFloat_Check(py_value)) {
            long value = PyLong_AsLong(py_value);
            if(PyErr_Occurred()) { return PY_PACK_ERROR; }
            if(value < std::numeric_limits<int16_t>::lowest()) {
                PyErr_SetString(PyExc_OverflowError, "int16 is less than minimum");
                return PY_PACK_ERROR;
            } else if(value > std::numeric_limits<int16_t>::max()) {
                PyErr_SetString(PyExc_OverflowError, "int16 is greater than maximum");
                return PY_PACK_ERROR;
            }
            dg->add_int16((int16_t)value);
        } else {
            return _custom_py_float_is_not_integer;
        }
        break;
    case bamboo::kTypeInt32:
        if(!PyFloat_Check(py_value)) {
            long value = PyLong_AsLong(py_value);
            if(PyErr_Occurred()) { return PY_PACK_ERROR; }
            if(value < std::numeric_limits<int32_t>::lowest()) {
                PyErr_SetString(PyExc_OverflowError, "int32 is less than minimum");
                return PY_PACK_ERROR;
            } else if(value > std::numeric_limits<int32_t>::max()) {
                PyErr_SetString(PyExc_OverflowError, "int32 is greater than maximum");
                return PY_PACK_ERROR;
            }
            dg->add_int32((int32_t)value);
        } else {
            return _custom_py_float_is_not_integer;
        }
        break;
    case bamboo::kTypeInt64:
        if(!PyFloat_Check(py_value)) {
            long long value = PyLong_AsLongLong(py_value);
            if(PyErr_Occurred()) { return PY_PACK_ERROR; }
            dg->add_int64((int64_t)value);
        } else {
            return _custom_py_float_is_not_integer;
        }
        break;
    case bamboo::kTypeUint8:
        if(!PyFloat_Check(py_value)) {
            unsigned long value = PyLong_AsUnsignedLong(py_value);
            if(PyErr_Occurred()) { return PY_PACK_ERROR; }
            if(value < std::numeric_limits<uint8_t>::lowest()) {
                PyErr_SetString(PyExc_OverflowError, "uint8 is less than minimum");
                return PY_PACK_ERROR;
            } else if(value > std::numeric_limits<uint8_t>::max()) {
                PyErr_SetString(PyExc_OverflowError, "uint8 is greater than maximum");
                return PY_PACK_ERROR;
            }
            dg->add_uint8((uint8_t)value);
        } else {
            return _custom_py_float_is_not_integer;
        }
        break;
    case bamboo::kTypeUint16:
        if(!PyFloat_Check(py_value)) {
            unsigned long value = PyLong_AsUnsignedLong(py_value);
            if(PyErr_Occurred()) { return PY_PACK_ERROR; }
            if(value < std::numeric_limits<uint16_t>::lowest()) {
                PyErr_SetString(PyExc_OverflowError, "uint16 is less than minimum");
                return PY_PACK_ERROR;
            } else if(value > std::numeric_limits<uint16_t>::max()) {
                PyErr_SetString(PyExc_OverflowError, "uint16 is greater than maximum");
                return PY_PACK_ERROR;
            }
            dg->add_uint16((uint16_t)value);
        } else {
            return _custom_py_float_is_not_integer;
        }
        break;
    case bamboo::kTypeUint32:
        if(!PyFloat_Check(py_value)) {
            unsigned long value = PyLong_AsUnsignedLong(py_value);
            if(PyErr_Occurred()) { return PY_PACK_ERROR; }
            if(value < std::numeric_limits<uint32_t>::lowest()) {
                PyErr_SetString(PyExc_OverflowError, "uint32 is less than minimum");
                return PY_PACK_ERROR;
            } else if(value > std::numeric_limits<uint32_t>::max()) {
                PyErr_SetString(PyExc_OverflowError, "uint32 is greater than maximum");
                return PY_PACK_ERROR;
            }
            dg->add_uint32((uint32_t)value);
        } else {
            return _custom_py_float_is_not_integer;
        }
        break;
    case bamboo::kTypeUint64:
        if(!PyFloat_Check(py_value)) {
            unsigned long long value = PyLong_AsUnsignedLongLong(py_value);
            if(PyErr_Occurred()) { return PY_PACK_ERROR; }
            dg->add_uint64((uint64_t)value);
        } else {
            return _custom_py_float_is_not_integer;
        }
        break;
    case bamboo::kTypeFloat32:
        {
            double value = PyFloat_AsDouble(py_value);
            if(PyErr_Occurred()) { return PY_PACK_ERROR; }
            dg->add_float32((float)value);
        }
        break;
    case bamboo::kTypeFloat64:
        {
            double value = PyFloat_AsDouble(py_value);
            if(PyErr_Occurred()) { return PY_PACK_ERROR; }
            dg->add_float64((double)value);
        }
        break;
    case bamboo::kTypeFixed:
        {
            const bamboo::Numeric *numeric = type->as_numeric();
            double value = PyFloat_AsDouble(py_value);
            if(PyErr_Occurred()) { return PY_PACK_ERROR; }

            if(numeric->is_signed()) {
                switch(numeric->fixed_size()) {
                case 1:
                    dg->add_int8(numeric->to_fixed_s8(value));
                    break;
                case 2:
                    dg->add_int16(numeric->to_fixed_s16(value));
                    break;
                case 4:
                    dg->add_int32(numeric->to_fixed_s32(value));
                    break;
                case 8:
                    dg->add_int64(numeric->to_fixed_s64(value));
                    break;
                default:
                    return _custom_numeric_has_bad_bytesize;
                }
            } else {
                switch(numeric->fixed_size()) {
                case 1:
                    dg->add_int8(numeric->to_fixed_u8(value));
                    break;
                case 2:
                    dg->add_int16(numeric->to_fixed_u16(value));
                    break;
                case 4:
                    dg->add_int32(numeric->to_fixed_u32(value));
                    break;
                case 8:
                    dg->add_int64(numeric->to_fixed_u64(value));
                    break;
                default:
                    return _custom_numeric_has_bad_bytesize;
                }
            }
        }
        break;
    case bamboo::kTypeString:
    case bamboo::kTypeBlob:
        {
            const char *value_ptr;
            Py_ssize_t value_length;
            if(PyBytes_Check(py_value)) {
                value_length = PyBytes_Size(py_value);
                value_ptr = PyBytes_AS_STRING(py_value);
            } else if(PyUnicode_Check(py_value)) {
                value_length = PyUnicode_GET_DATA_SIZE(py_value);
                value_ptr = PyUnicode_AS_DATA(py_value);
            } else {
                return _custom_py_pack_typerr("unicode, string(py2), or bytes(py3)", py_value, msgbuf, bufsize);
            }

            if(type->has_fixed_size()) {
                bamboo::sizetag_t length = type->fixed_size();
                if(Py_ssize_t(length) != value_length) {
                    PyOS_snprintf(msgbuf, bufsize,
                        "Datagram tried to add string/blob value, but value had length %d"
                        " for a string/blob of fixed length %d", (int)value_length, length);
                    PyErr_SetString(PyExc_ValueError, msgbuf);
                    return PY_PACK_ERROR;
                }

                dg->add_data(value_ptr, value_length);
            } else {
                dg->add_blob(value_ptr, value_length);
            }
        }
        break;
    case bamboo::kTypeArray:
        if(PyList_Check(py_value)) {
            const bamboo::Array *arr = type->as_array();
            const bamboo::Type *element_type = arr->element_type();

            if(type->has_fixed_size()) {
                bamboo::sizetag_t size = arr->array_size();
                Py_ssize_t value_size = PyList_Size(py_value);
                if(Py_ssize_t(size) != value_size) {
                    PyOS_snprintf(msgbuf, bufsize,
                        "Datagram tried to add array value, but was given %d"
                        " elements for an array of fixed size %d", (int)value_size, (int)size);
                    PyErr_SetString(PyExc_ValueError, msgbuf);
                    return PY_PACK_ERROR;
                }

                for(int i = 0; i < value_size; ++i) {
                    PyObject *element_value = PyList_GetItem(py_value, i);
                    char *msg = _custom_PyBambooDatagram_py_pack(dg, element_type, element_value, msgbuf, bufsize);
                    if(msg != nullptr) { return msg; }
                }
            } else {
                const bamboo::Array *arr = type->as_array();
                const bamboo::Type *element_type = arr->element_type();

                bamboo::Datagram packed;
                Py_ssize_t num_elements = PyList_Size(py_value);
                for(int i = 0; i < num_elements; ++i) {
                    PyObject *element_value = PyList_GetItem(py_value, i);
                    char *msg = _custom_PyBambooDatagram_py_pack(&packed, element_type, element_value, msgbuf, bufsize);
                    if(msg != nullptr) { return msg; }
                }

                dg->add_size(packed.size());
                dg->add_data(packed);
            }
        } else {
            return _custom_py_pack_typerr("list", py_value, msgbuf, bufsize);
        }
        break;
    case bamboo::kTypeStruct:
        if(PyTuple_Check(py_value)) {
            const bamboo::Struct *struct_ = type->as_struct();
            size_t num_fields = struct_->num_fields();

            Py_ssize_t value_num_fields = PyTuple_Size(py_value);
            if(Py_ssize_t(num_fields) != value_num_fields) {
                PyOS_snprintf(msgbuf, bufsize,
                    "Datagram tried to add struct value, but was given %d"
                    " fields for a struct with %d fields", (int)value_num_fields, (int)num_fields);
                PyErr_SetString(PyExc_ValueError, msgbuf);
                return PY_PACK_ERROR;
            }

            for(int i = 0; i < value_num_fields; ++i) {
                const bamboo::Type *field_type = struct_->nth_field((unsigned int)i)->type();
                PyObject *field_value = PyTuple_GetItem(py_value, i);
                char *msg = _custom_PyBambooDatagram_py_pack(dg, field_type, field_value, msgbuf, bufsize);
                if(msg != nullptr) { return msg; }
            }
        } else {
            return _custom_py_pack_typerr("tuple", py_value, msgbuf, bufsize);
        }
        break;
    case bamboo::kTypeMethod:
        if(PyTuple_Check(py_value)) {
            const bamboo::Method *method = type->as_method();
            size_t num_params = method->num_params();

            Py_ssize_t value_num_params = PyTuple_Size(py_value);
            if(Py_ssize_t(num_params) != value_num_params) {
                PyOS_snprintf(msgbuf, bufsize,
                    "Datagram tried to add method value, but was given %d"
                    "  params for a method with %d params", (int)value_num_params, (int)num_params);
                PyErr_SetString(PyExc_ValueError, msgbuf);
                return PY_PACK_ERROR;
            }

            for(int i = 0; i < value_num_params; ++i) {
                const bamboo::Type *param_type = method->nth_param((unsigned int)i)->type();
                PyObject *param_value = PyTuple_GetItem(py_value, i);
                char *msg = _custom_PyBambooDatagram_py_pack(dg, param_type, param_value, msgbuf, bufsize);
                if(msg != nullptr) { return msg; }
            }
        } else {
            return _custom_py_pack_typerr("tuple", py_value, msgbuf, bufsize);
        }
        break;
    case bamboo::kTypeNone:
        return (char *)"Can't pack value for type with subtype 'none'";
    }

    return nullptr;

#undef PY_PACK_ERROR

}

PyObject *_custom_wrap_PyBambooDatagram_add_value(PyBambooDatagram *self, PyObject *args,
                                                  PyObject *kwargs, PyObject **return_exception)
{
    const char *keywords[] = {"type", "value", nullptr};

    PyBambooType *type;
    bamboo::Type *type_ptr;
    PyObject *py_value;

    if(PyArg_ParseTupleAndKeywords(args, kwargs, "O!O", (char **) keywords, &PyBambooType_Type, &type, &py_value)) {
        type_ptr = (type ? type->obj : nullptr);
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return nullptr;
    }

    char msgbuf[256];
    try {
        char * msg = _custom_PyBambooDatagram_py_pack(self->obj, type_ptr, py_value, msgbuf, sizeof(msgbuf));
        if(msg != nullptr) {
            if(!PyErr_Occurred()) { PyErr_SetString(PyExc_TypeError, msg); }
            return nullptr;
        }
    } catch (bamboo::DatagramOverflow const &exc) {
        PyErr_SetString((PyObject *) Pybamboo__DatagramOverflow_Type, exc.what());
        return nullptr;
    }

    PyObject *py_retval;
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}

PyObject *_custom_wrap_PyBambooDatagram_addValue(PyBambooDatagram *self, PyObject *args,
                                                 PyObject *kwargs, PyObject **return_exception)
{
    const char *keywords[] = {"type", "value", nullptr};

    PyBambooType *type;
    bamboo::Type *type_ptr;
    PyObject *py_value;

    if(PyArg_ParseTupleAndKeywords(args, kwargs, "O!O", (char **) keywords, &PyBambooType_Type, &type, &py_value)) {
        type_ptr = (type ? type->obj : nullptr);
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return nullptr;
    }

    char msgbuf[256];
    try {
        char * msg = _custom_PyBambooDatagram_py_pack(self->obj, type_ptr, py_value, msgbuf, sizeof(msgbuf));
        if(msg != nullptr) {
            if(!PyErr_Occurred()) { PyErr_SetString(PyExc_TypeError, msg); }
            return nullptr;
        }
    } catch (bamboo::DatagramOverflow const &exc) {
        PyErr_SetString((PyObject *) Pybamboo__DatagramOverflow_Type, exc.what());
        return nullptr;
    }

    PyObject *py_retval;
    Py_INCREF(Py_None);
    py_retval = Py_None;
    return py_retval;
}
