#include <bytesobject.h>  // python2.6+ compat with BYTES_*

static PyObject *_custom_PyBambooDatagramIterator_py_unpack(bamboo::DatagramIterator *dgi,
                                                     const bamboo::Type *type,
                                                     PyObject **return_exception)
{
    switch(type->subtype()) {
    case bamboo::kTypeChar:
        return Py_BuildValue("c", dgi->read_char()); // Py_BuildValue simplifies Py2/3 support
    case bamboo::kTypeInt8:
        return PyLong_FromLong(dgi->read_int8());
    case bamboo::kTypeInt16:
        return PyLong_FromLong(dgi->read_int16());
    case bamboo::kTypeInt32:
        return PyLong_FromLong(dgi->read_int32());
    case bamboo::kTypeInt64:
        return PyLong_FromLongLong(dgi->read_int64());
    case bamboo::kTypeUint8:
        return PyLong_FromUnsignedLong(dgi->read_uint8());
    case bamboo::kTypeUint16:
        return PyLong_FromUnsignedLong(dgi->read_uint16());
    case bamboo::kTypeUint32:
        return PyLong_FromUnsignedLong(dgi->read_uint32());
    case bamboo::kTypeUint64:
        return PyLong_FromUnsignedLongLong(dgi->read_uint64());
    case bamboo::kTypeFloat32:
        return PyFloat_FromDouble(dgi->read_float32());
    case bamboo::kTypeFloat64:
        return PyFloat_FromDouble(dgi->read_float64());
    case bamboo::kTypeString:
        {
            std::string value = dgi->read_string(type->fixed_size());
            return PyUnicode_FromStringAndSize(value.c_str(), value.length());
        }
    case bamboo::kTypeVarstring:
        {
            std::string value = dgi->read_string();
            return PyUnicode_FromStringAndSize(value.c_str(), value.length());
        }
    case bamboo::kTypeBlob:
        {
            std::string value = dgi->read_string(type->fixed_size());
            return PyBytes_FromStringAndSize(value.c_str(), value.length());
        }
    case bamboo::kTypeVarblob:
        {
            std::string value = dgi->read_string();
            return PyBytes_FromStringAndSize(value.c_str(), value.length());
        }
    case bamboo::kTypeArray:
        {
            const bamboo::Array *arr = type->as_array();
            const bamboo::Type *element_type = arr->element_type();

            bamboo::sizetag_t size = arr->array_size();
            PyObject *value = PyList_New(size);
            if(value == nullptr) { return nullptr; }
            for(unsigned int i = 0; i < size; ++i) {
                PyObject *element = _custom_PyBambooDatagramIterator_py_unpack(dgi, element_type, return_exception);
                if(element == nullptr) { return nullptr; }
                PyList_SetItem(value, i, element);
            }

            return value;
        }
    case bamboo::kTypeVararray:
        {
            const bamboo::Array *arr = type->as_array();
            const bamboo::Type *element_type = arr->element_type();

            std::vector<PyObject *> elements;

            // Read elements from the array till we reach the expected size
            bamboo::sizetag_t length = dgi->read_size();
            bamboo::sizetag_t end = dgi->tell() + length;
            while(dgi->tell() < end) {
                PyObject *element = _custom_PyBambooDatagramIterator_py_unpack(dgi, element_type, return_exception);
                if(element == nullptr) { return nullptr; }
                elements.push_back(element);
            }
            if(dgi->tell() > end) {
                bamboo::sizetag_t bytes_read = length + (dgi->tell() - end);
                std::stringstream error;
                error << "Datagram tried to read array value for " << length << " byte long array"
                      " but read " << bytes_read << " bytes without finding the end.\n";
                PyErr_SetString((PyObject *) PyExc_RuntimeError, error.str().c_str());
                return NULL;
            }

            size_t size = elements.size();
            PyObject *value = PyTuple_New(size);
            if(value == nullptr) { return nullptr; }
            for(unsigned int i = 0; i < size; ++i) {
                PyList_SetItem(value, i, elements[i]);
            }

            return value;
        }
    case bamboo::kTypeStruct:
        {
            const bamboo::Struct *struct_ = type->as_struct();
            size_t num_fields = struct_->num_fields();

            PyObject *value = PyTuple_New(num_fields);
            if(value == nullptr) { return nullptr; }
            for(unsigned int i = 0; i < num_fields; ++i) {
                const bamboo::Type *field_type = struct_->get_field((unsigned int)i)->type();
                PyObject *field = _custom_PyBambooDatagramIterator_py_unpack(dgi, field_type, return_exception);
                if(field == nullptr) { return nullptr; }
                PyTuple_SetItem(value, i, field);
            }

            return value;
        }
    case bamboo::kTypeMethod:
        {
            const bamboo::Method *method = type->as_method();
            size_t num_params = method->num_parameters();

            PyObject *value = PyTuple_New(num_params);
            if(value == nullptr) { return nullptr; }
            for(unsigned int i = 0; i < num_params; ++i) {
                const bamboo::Type *param_type = method->get_parameter((unsigned int)i)->type();
                PyObject *param = _custom_PyBambooDatagramIterator_py_unpack(dgi, param_type, return_exception);
                if(param == nullptr) { return nullptr; }
                PyTuple_SetItem(value, i, param);
            }

            return value;
        }
    case bamboo::kTypeInvalid:
        PyErr_SetString((PyObject *) PyExc_RuntimeError, "Can't unpack value for type with subtype 'invalid'");
        return nullptr;
    }

    PyErr_SetString((PyObject *) PyExc_AssertionError,
        "Reached end of 'read_value' method of 'bamboo.wire.DatagramIterator' without returning a value.");
    return nullptr;
}

PyObject *_custom_wrap_PyBambooDatagramIterator_read_value(PyBambooDatagramIterator *self, PyObject *args,
                                                           PyObject *kwargs, PyObject **return_exception)
{
    const char *keywords[] = {"type", nullptr};

    PyBambooType *type;
    bamboo::Type *type_ptr;

    if(PyArg_ParseTupleAndKeywords(args, kwargs, "O!", (char **) keywords, &PyBambooType_Type, &type)) {
        type_ptr = (type ? type->obj : nullptr);
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return nullptr;
    }

    try {
        return _custom_PyBambooDatagramIterator_py_unpack(self->obj, type_ptr, return_exception);
    } catch (bamboo::DatagramIteratorEOF const &exc) {
        PyErr_SetString((PyObject *) Pybamboo__DatagramIteratorEOF_Type, exc.what());
        return nullptr;
    }
}

PyObject *_custom_wrap_PyBambooDatagramIterator_readValue(PyBambooDatagramIterator *self, PyObject *args,
                                                          PyObject *kwargs, PyObject **return_exception)
{
    const char *keywords[] = {"type", nullptr};

    PyBambooType *type;
    bamboo::Type *type_ptr;

    if(PyArg_ParseTupleAndKeywords(args, kwargs, "O!", (char **) keywords, &PyBambooType_Type, &type)) {
        type_ptr = (type ? type->obj : nullptr);
    } else {
        PyObject *exc_type, *traceback;
        PyErr_Fetch(&exc_type, return_exception, &traceback);
        Py_XDECREF(exc_type);
        Py_XDECREF(traceback);
        return nullptr;
    }

    try {
        return _custom_PyBambooDatagramIterator_py_unpack(self->obj, type_ptr, return_exception);
    } catch (bamboo::DatagramIteratorEOF const &exc) {
        PyErr_SetString((PyObject *) Pybamboo__DatagramIteratorEOF_Type, exc.what());
        return nullptr;
    }
}
