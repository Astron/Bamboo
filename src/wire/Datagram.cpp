#include "Datagram.h"
#include "module/ArrayType.h"
#include "module/Struct.h"
#include "module/Method.h"
#include "module/Field.h"
#include "module/Parameter.h"
using namespace std;
namespace bamboo {  // close namespace bamboo


// add_field adds a packed field value to the datagram, converting byte-order
// from native-endianess to wire-endianess (if necessary).
// Returns the number of bytes read from the packed.
sizetag_t Datagram::add_field(const Field* field, const vector<uint8_t>& packed) {
    return add_dtype(field->get_type(), packed);
}

// add_dtype adds a packed value with the given type to the datagram, converting
// byte-order from native-endianess to wire-endianess (if necessary).
// Returns the number of bytes read from the packed.
sizetag_t Datagram::add_dtype(const DistributedType *dtype,
                              const vector<uint8_t>& packed,
                              sizetag_t offset) {
    switch(dtype->get_type()) {
        case T_CHAR:
        {
            check_read_length(packed, offset, sizeof(char));
            char r = *(char*)(&packed[offset]);
            offset += sizeof(char);
            add_char(r);
            return offset;
        }
        case T_INT8:
        {
            check_read_length(packed, offset, sizeof(int8_t));
            int8_t r = *(int8_t*)(&packed[offset]);
            offset += sizeof(int8_t);
            add_int8(r);
            return offset;
        }
        case T_INT16:
        {
            check_read_length(packed, offset, sizeof(int16_t));
            int16_t r = *(int16_t*)(&packed[offset]);
            offset += sizeof(int16_t);
            add_int16(r);
            return offset;
        }
        case T_INT32:
        {
            check_read_length(packed, offset, sizeof(int32_t));
            int32_t r = *(int32_t*)(&packed[offset]);
            offset += sizeof(int32_t);
            add_int32(r);
            return offset;
        }
        case T_INT64:
        {
            check_read_length(packed, offset, sizeof(int64_t));
            int64_t r = *(int64_t*)(&packed[offset]);
            offset += sizeof(int64_t);
            add_int64(r);
            return offset;
        }
        case T_UINT8:
        {
            check_read_length(packed, offset, sizeof(uint8_t));
            uint8_t r = *(uint8_t*)(&packed[offset]);
            offset += sizeof(uint8_t);
            add_uint8(r);
            return offset;
        }
        case T_UINT16:
        {
            check_read_length(packed, offset, sizeof(uint16_t));
            uint16_t r = *(uint16_t*)(&packed[offset]);
            offset += sizeof(uint16_t);
            add_uint16(r);
            return offset;
        }
        case T_UINT32:
        {
            check_read_length(packed, offset, sizeof(uint32_t));
            uint32_t r = *(uint32_t*)(&packed[offset]);
            offset += sizeof(uint32_t);
            add_uint32(r);
            return offset;
        }
        case T_UINT64:
        {
            check_read_length(packed, offset, sizeof(uint64_t));
            uint64_t r = *(uint64_t*)(&packed[offset]);
            offset += sizeof(uint64_t);
            add_uint64(r);
            return offset;
        }
        case T_FLOAT32:
        {
            check_read_length(packed, offset, sizeof(float));
            float r = *(float*)(&packed[offset]);
            offset += sizeof(float);
            add_float32(r);
            return offset;
        }
        case T_FLOAT64:
        {
            check_read_length(packed, offset, sizeof(double));
            double r = *(double*)(&packed[offset]);
            offset += sizeof(double);
            add_float64(r);
            return offset;
        }
        case T_STRING:
        case T_BLOB:
        {
            sizetag_t len = dtype->get_size();
            check_read_length(packed, offset, len);
            add_blob(&packed[offset], len);
            offset += len;
            return offset;
        }
        case T_ARRAY:
        {
            sizetag_t len = dtype->get_size();
            check_read_length(packed, offset, len);

            const ArrayType *arr = dtype->as_array();
            if(arr->get_element_type()->get_size() == 1) {
                // If the element size is 1, we don't have to worry about endianness
                add_blob(&packed[offset], len);
                offset += len;
                return offset;
            }

            // Copy elements from the array till we reach the expected size
            sizetag_t array_end = offset + len;
            while(offset < array_end) {
                offset = add_dtype(arr->get_element_type(), packed, offset);
            }
            if(offset > array_end) {
                stringstream error;
                error << "Datagram tried to copy array data, but array data"
                         " exceeded the expected array length of " << len << ".\n";
                throw BufferEOF(error.str());
            }
            return offset;
        }
        case T_VARSTRING:
        case T_VARBLOB:
        {
            check_read_length(packed, offset, sizeof(sizetag_t));
            sizetag_t len = *(sizetag_t*)(&packed[offset]);
            offset += sizeof(sizetag_t);
            check_read_length(packed, offset, len);

            add_blob(&packed[offset], len);
            offset += len;
            return offset;
        }
        case T_VARARRAY:
        {
            check_read_length(packed, offset, sizeof(sizetag_t));
            sizetag_t len = *(sizetag_t*)(&packed[offset]);
            offset += sizeof(sizetag_t);
            check_read_length(packed, offset, len);

            const ArrayType *arr = dtype->as_array();
            if(arr->get_element_type()->get_size() == 1) {
                // If the element size is 1, we don't have to worry about endianness
                add_blob(&packed[offset], len);
                offset += len;
                return offset;
            }

            add_size(packed.size());

            // Copy elements from the array till we reach the expected size
            sizetag_t array_end = offset + len;
            while(offset < array_end) {
                offset = add_dtype(arr->get_element_type(), packed, offset);
            }
            if(offset > array_end) {
                stringstream error;
                error << "Datagram tried to copy array data, but array data"
                         " exceeded the expected array length of " << len << ".\n";
                throw BufferEOF(error.str());
            }
            return offset;
        }
        case T_STRUCT:
        {
            const Struct *dstruct = dtype->as_struct();
            size_t num_fields = dstruct->get_num_fields();
            for(unsigned int i = 0; i < num_fields; ++i) {
                offset = add_dtype(dstruct->get_field(i)->get_type(), packed, offset);
            }
            return offset;
        }
        case T_METHOD:
        {
            const Method *dmethod = dtype->as_method();
            size_t num_params = dmethod->get_num_parameters();
            for(unsigned int i = 0; i < num_params; ++i) {
                offset = add_dtype(dmethod->get_parameter(i)->get_type(), packed, offset);
            }
            return offset;
        }
        case T_INVALID:
        {
            return offset;
        }
    }
    return offset;
}


} // close namespace bamboo
