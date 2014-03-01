#include "DatagramIterator.h"
#include "module/ArrayType.h"
#include "module/Struct.h"
#include "module/Method.h"
#include "module/Field.h"
#include "module/Parameter.h"
using namespace std;
namespace bamboo {  // close namespace bamboo


// read_field interprets the following data as a
// value for the Field in native endianness.
vector<uint8_t> DatagramIterator::read_field(const Field *field) {
    vector<uint8_t> buffer;
    read_field(field, buffer);
    return buffer;
}

// read_field can also be called to read into an existing buffer.
void DatagramIterator::read_field(const Field *field, vector<uint8_t>& buffer) {
    read_dtype(field->get_type(), buffer);
}

// read_dtype interprets the following data as a
// value for the DistributedType in native endianness.
vector<uint8_t> DatagramIterator::read_dtype(const DistributedType *dtype) {
    vector<uint8_t> buffer;
    read_dtype(dtype, buffer);
    return buffer;
}

// read_dtype can also be called to read into an existing buffer.
void DatagramIterator::read_dtype(const DistributedType *dtype, vector<uint8_t>& buffer) {

#ifndef PLATFORM_BIG_ENDIAN
    // If object has fixed size, and we're little endian, we don't have to interpret it
    if(dtype->has_fixed_size()) {
        // If field is a fixed-sized type like uint, int, float, etc
        // Also any other type lucky enough to be fixed size will be computed faster
        vector<uint8_t> data = read_data(dtype->get_size());
        buffer.insert(buffer.end(), data.begin(), data.end());
        return;
    }
#endif

    // For the unlucky types/machines, we have to figure out their size manually
    switch(dtype->get_subtype()) {
        case kTypeChar:
        {
            char r = read_char();
            buffer.insert(buffer.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
            break;
        }
        case kTypeInt8:
        {
            int8_t r = read_int8();
            buffer.insert(buffer.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
            break;
        }
        case kTypeInt16:
        {
            int16_t r = read_int16();
            buffer.insert(buffer.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
            break;
        }
        case kTypeInt32:
        {
            int32_t r = read_int32();
            buffer.insert(buffer.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
            break;
        }
        case kTypeInt64:
        {
            int64_t r = read_int64();
            buffer.insert(buffer.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
            break;
        }
        case kTypeUint8:
        {
            uint8_t r = read_uint8();
            buffer.insert(buffer.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
            break;
        }
        case kTypeUint16:
        {
            uint16_t r = read_uint16();
            buffer.insert(buffer.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
            break;
        }
        case kTypeUint32:
        {
            uint32_t r = read_uint32();
            buffer.insert(buffer.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
            break;
        }
        case kTypeUint64:
        {
            uint64_t r = read_uint64();
            buffer.insert(buffer.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
            break;
        }
        case kTypeFloat32:
        {
            float r = read_float32();
            buffer.insert(buffer.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
            break;
        }
        case kTypeFloat64:
        {
            double r = read_float64();
            buffer.insert(buffer.end(), (uint8_t *)&r, (uint8_t *)(&r + 1));
            break;
        }
        case kTypeString:
        case kTypeBlob:
        {
            vector<uint8_t> blob = read_data(dtype->get_size());
            buffer.insert(buffer.end(), blob.begin(), blob.end());
            break;
        }
        case kTypeArray:
        {
            const ArrayType *arr = dtype->as_array();
            if(arr->get_element_type()->get_size() == 1) {
                // If the element size is 1, we don't have to worry about endianness
                vector<uint8_t> blob = read_data(dtype->get_size());
                buffer.insert(buffer.end(), blob.begin(), blob.end());
                break;
            }

            // Read elements from the array till we reach the expected size
            sizetag_t len = dtype->get_size();
            sizetag_t array_end = m_offset + len;
            while(m_offset < array_end) {
                read_dtype(arr->get_element_type(), buffer);
            }
            if(m_offset > array_end) {
                stringstream error;
                error << "Datagram iterator tried to read array data, but array data"
                         " exceeded the expected array length of " << len << ".\n";
                throw DatagramIteratorEOF(error.str());
            }
            break;
        }
        case kTypeVarstring:
        case kTypeVarblob:
        {
            sizetag_t len = read_size();
            buffer.insert(buffer.end(), (uint8_t *)&len, (uint8_t *)&len + sizeof(sizetag_t));

            vector<uint8_t> blob = read_data(len);
            buffer.insert(buffer.end(), blob.begin(), blob.end());
            break;
        }
        case kTypeVararray:
        {
            sizetag_t len = read_size();
            buffer.insert(buffer.end(), (uint8_t *)&len, (uint8_t *)&len + sizeof(sizetag_t));

            const ArrayType* arr = dtype->as_array();
            if(arr->get_element_type()->get_size() == 1) {
                // If the element size is 1, we don't have to worry about endianness
                vector<uint8_t> blob = read_data(len);
                buffer.insert(buffer.end(), blob.begin(), blob.end());
                break;
            }

            // Read elements from the array till we reach the expected size
            sizetag_t array_end = m_offset + len;
            while(m_offset < array_end) {
                read_dtype(arr->get_element_type(), buffer);
            }
            if(m_offset > array_end) {
                stringstream error;
                error << "Datagram iterator tried to read array data, but array data"
                         " exceeded the expected array length of " << len << ".\n";
                throw DatagramIteratorEOF(error.str());
            }
            break;
        }
        case kTypeStruct:
        {
            const Struct *dstruct = dtype->as_struct();
            size_t num_fields = dstruct->get_num_fields();
            for(unsigned int i = 0; i < num_fields; ++i) {
                read_dtype(dstruct->get_field(i)->get_type(), buffer);
            }
            break;
        }
        case kTypeMethod:
        {
            const Method *dmethod = dtype->as_method();
            size_t num_params = dmethod->get_num_parameters();
            for(unsigned int i = 0; i < num_params; ++i) {
                read_dtype(dmethod->get_parameter(i)->get_type(), buffer);
            }
            break;
        }
        case kTypeInvalid:
        {
            break;
        }
    }
}

// skip_field can be used to seek past the packed field data for a Field.
//     Throws DatagramIteratorEOF if it skips past the end of the datagram.
void DatagramIterator::skip_field(const Field *field) {
    skip_dtype(field->get_type());
}

// skip_dtype can be used to seek past the packed data for a DistributedType.
//     Throws DatagramIteratorEOF if it skips past the end of the datagram.
void DatagramIterator::skip_dtype(const DistributedType *dtype) {
    if(dtype->has_fixed_size()) {
        sizetag_t length = dtype->get_size();
        check_read_length(length);
        m_offset += length;
        return;
    }

    switch(dtype->get_subtype()) {
        case kTypeVarstring:
        case kTypeVarblob:
        case kTypeVararray: {
            sizetag_t length = read_size();
            check_read_length(length);
            m_offset += length;
            break;
        }
        case kTypeStruct: {
            const Struct *dstruct = dtype->as_struct();
            size_t num_fields = dstruct->get_num_fields();
            for(unsigned int i = 0; i < num_fields; ++i) {
                skip_dtype(dstruct->get_field(i)->get_type());
            }
            break;
        }
        case kTypeMethod: {
            const Method *dmethod = dtype->as_method();
            size_t num_params = dmethod->get_num_parameters();
            for(unsigned int i = 0; i < num_params; ++i) {
                skip_dtype(dmethod->get_parameter(i)->get_type());
            }
            break;
        }
        default:
        {
            // This case should be impossible, but a default is required by compilers
            break;
        }
    }
}


} // close namespace bamboo
