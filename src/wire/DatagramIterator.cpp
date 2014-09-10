#include "DatagramIterator.h"
#include "../bits/buffers.h"
#include "../values/Value.h"
#include "../module/ArrayType.h"
#include "../module/Struct.h"
#include "../module/Method.h"
#include "../module/Field.h"
#include "../module/Parameter.h"
using namespace std;
namespace bamboo {  // close namespace bamboo


// read_value interprets the following data as a
// value for the DistributedType in native endianness.
Value DatagramIterator::read_value(const DistributedType *type) {
    // Note: We can probably make this a lot more efficient by constructing the value manually
    //       from the wire-endian data instead of unpacking it into native endianess first.
    //       Though that would probably cause some code duplication with Value::from_packed.
    vector<uint8_t> packed = read_packed(type);
    return Value::from_packed(type, packed);
}

vector<uint8_t> DatagramIterator::read_packed(const DistributedType *type) {
#ifndef PLATFORM_BIG_ENDIAN
    // We're little endian so we don't have to worry about byte-swapping the data
    const uint8_t *start = m_dg->data() + m_offset;
    skip_type(type); // note: this will advanced m_offset
    return vector<uint8_t>(start, m_dg->data() + m_offset);
#else
    // Lets go ahead and unpack that manually
    vector<uint8_t> buf;
    read_packed(type, buf);
    return buf;
#endif

}

void DatagramIterator::read_packed(const DistributedType *dtype, vector<uint8_t>& buffer) {
#ifndef PLATFORM_BIG_ENDIAN
    if(dtype->has_fixed_size()) {
        // If we're a fixed-sized type like uint, int, float, etc
        // Also any other type lucky enough to be fixed size will be faster.
        vector<uint8_t> data = read_data(dtype->get_size());
        pack_value(data, buffer);
        return;
    }
#endif

    // For the unlucky types/machines, we have to figure out their size manually
    switch(dtype->get_subtype()) {
        case kTypeChar:
            pack_value(read_char(), buffer);
            break;
        case kTypeInt8:
            pack_value(read_int8(), buffer);
            break;
        case kTypeInt16:
            pack_value(read_int16(), buffer);
            break;
        case kTypeInt32:
            pack_value(read_int32(), buffer);
            break;
        case kTypeInt64:
            pack_value(read_int64(), buffer);
            break;
        case kTypeUint8:
            pack_value(read_uint8(), buffer);
            break;
        case kTypeUint16:
            pack_value(read_uint16(), buffer);
            break;
        case kTypeUint32:
            pack_value(read_uint32(), buffer);
            break;
        case kTypeUint64:
            pack_value(read_uint64(), buffer);
            break;
        case kTypeFloat32:
            pack_value(read_float32(), buffer);
            break;
        case kTypeFloat64:
            pack_value(read_float64(), buffer);
            break;
        case kTypeString:
        case kTypeBlob:
            pack_value(read_data(dtype->get_size()), buffer);
            break;
        case kTypeArray: {
            const ArrayType *arr = dtype->as_array();
            if(arr->get_element_type()->get_size() == 1) {
                // If the element size is 1, we don't have to worry about endianness
                pack_value(read_data(dtype->get_size()), buffer);
                break;
            }

            // Read elements from the array till we reach the expected size
            sizetag_t len = dtype->get_size();
            sizetag_t array_end = m_offset + len;
            while(m_offset < array_end) {
                read_packed(arr->get_element_type(), buffer);
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
        case kTypeVarblob: {
            sizetag_t len = read_size();
            pack_value(len, buffer);
            pack_value(read_data(len), buffer);
            break;
        }
        case kTypeVararray: {
            sizetag_t len = read_size();
            pack_value(len, buffer);

            const ArrayType *arr = dtype->as_array();
            if(arr->get_element_type()->get_size() == 1) {
                // If the element size is 1, we don't have to worry about endianness
                pack_value(read_data(len), buffer);
                break;
            }

            // Read elements from the array till we reach the expected size
            sizetag_t array_end = m_offset + len;
            while(m_offset < array_end) {
                read_packed(arr->get_element_type(), buffer);
            }
            if(m_offset > array_end) {
                stringstream error;
                error << "Datagram iterator tried to read array data, but array data"
                      " exceeded the expected array length of " << len << ".\n";
                throw DatagramIteratorEOF(error.str());
            }
            break;
        }
        case kTypeStruct: {
            const Struct *dstruct = dtype->as_struct();
            size_t num_fields = dstruct->get_num_fields();
            for(unsigned int i = 0; i < num_fields; ++i) {
                read_packed(dstruct->get_field(i)->get_type(), buffer);
            }
            break;
        }
        case kTypeMethod: {
            const Method *dmethod = dtype->as_method();
            size_t num_params = dmethod->get_num_parameters();
            for(unsigned int i = 0; i < num_params; ++i) {
                read_packed(dmethod->get_parameter(i)->get_type(), buffer);
            }
            break;
        }
        case kTypeInvalid: {
            break;
        }
    }
}

// skip_type can be used to seek past the packed data for a DistributedType.
//     Throws DatagramIteratorEOF if it skips past the end of the datagram.
void DatagramIterator::skip_type(const DistributedType *dtype) {
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
                skip_type(dstruct->get_field(i)->get_type());
            }
            break;
        }
        case kTypeMethod: {
            const Method *dmethod = dtype->as_method();
            size_t num_params = dmethod->get_num_parameters();
            for(unsigned int i = 0; i < num_params; ++i) {
                skip_type(dmethod->get_parameter(i)->get_type());
            }
            break;
        }
        default: {
            // This case should be impossible, but a default is required by compilers
            break;
        }
    }
}


} // close namespace bamboo
