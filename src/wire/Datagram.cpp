#include "Datagram.h"
#include "values/Value.h"
#include "module/ArrayType.h"
#include "module/Struct.h"
#include "module/Method.h"
#include "module/Field.h"
#include "module/Parameter.h"
using namespace std;
namespace bamboo {  // close namespace bamboo


// add_value adds a Value with the given type to the datagram, converting
// byte-order from native-endianess to wire-endianess (if necessary).
void Datagram::add_value(const DistributedType *type, const Value value) {
    vector<uint8_t> packed = value.pack(type);

#ifdef PLATFORM_BIG_ENDIAN
    add_packed(type, packed); // endian-swap all values
#else
    add_data(packed);
#endif
};

// add_packed adds data from a packed value, returning the number of bytes read from the buffer.
sizetag_t Datagram::add_packed(const DistributedType *type,
                               const vector<uint8_t>& packed,
                               sizetag_t offset) {
    switch(type->get_subtype()) {
        case kTypeChar:
        {
            check_read_length(packed, offset, sizeof(char));
            char r = *(char*)(&packed[offset]);
            offset += sizeof(char);
            add_char(r);
            return offset;
        }
        case kTypeInt8:
        {
            check_read_length(packed, offset, sizeof(int8_t));
            int8_t r = *(int8_t*)(&packed[offset]);
            offset += sizeof(int8_t);
            add_int8(r);
            return offset;
        }
        case kTypeInt16:
        {
            check_read_length(packed, offset, sizeof(int16_t));
            int16_t r = *(int16_t*)(&packed[offset]);
            offset += sizeof(int16_t);
            add_int16(r);
            return offset;
        }
        case kTypeInt32:
        {
            check_read_length(packed, offset, sizeof(int32_t));
            int32_t r = *(int32_t*)(&packed[offset]);
            offset += sizeof(int32_t);
            add_int32(r);
            return offset;
        }
        case kTypeInt64:
        {
            check_read_length(packed, offset, sizeof(int64_t));
            int64_t r = *(int64_t*)(&packed[offset]);
            offset += sizeof(int64_t);
            add_int64(r);
            return offset;
        }
        case kTypeUint8:
        {
            check_read_length(packed, offset, sizeof(uint8_t));
            uint8_t r = *(uint8_t*)(&packed[offset]);
            offset += sizeof(uint8_t);
            add_uint8(r);
            return offset;
        }
        case kTypeUint16:
        {
            check_read_length(packed, offset, sizeof(uint16_t));
            uint16_t r = *(uint16_t*)(&packed[offset]);
            offset += sizeof(uint16_t);
            add_uint16(r);
            return offset;
        }
        case kTypeUint32:
        {
            check_read_length(packed, offset, sizeof(uint32_t));
            uint32_t r = *(uint32_t*)(&packed[offset]);
            offset += sizeof(uint32_t);
            add_uint32(r);
            return offset;
        }
        case kTypeUint64:
        {
            check_read_length(packed, offset, sizeof(uint64_t));
            uint64_t r = *(uint64_t*)(&packed[offset]);
            offset += sizeof(uint64_t);
            add_uint64(r);
            return offset;
        }
        case kTypeFloat32:
        {
            check_read_length(packed, offset, sizeof(float));
            float r = *(float*)(&packed[offset]);
            offset += sizeof(float);
            add_float32(r);
            return offset;
        }
        case kTypeFloat64:
        {
            check_read_length(packed, offset, sizeof(double));
            double r = *(double*)(&packed[offset]);
            offset += sizeof(double);
            add_float64(r);
            return offset;
        }
        case kTypeString:
        case kTypeBlob:
        {
            sizetag_t len = type->get_size();
            check_read_length(packed, offset, len);
            add_blob(&packed[offset], len);
            offset += len;
            return offset;
        }
        case kTypeArray:
        {
            sizetag_t len = type->get_size();
            check_read_length(packed, offset, len);

            const ArrayType *arr = type->as_array();
            if(arr->get_element_type()->get_size() == 1) {
                // If the element size is 1, we don't have to worry about endianness
                add_blob(&packed[offset], len);
                offset += len;
                return offset;
            }

            // Copy elements from the array till we reach the expected size
            sizetag_t array_end = offset + len;
            while(offset < array_end) {
                offset = add_packed(arr->get_element_type(), packed, offset);
            }
            if(offset > array_end) {
                stringstream error;
                error << "Datagram tried to copy array data, but array data"
                         " exceeded the expected array length of " << len << ".\n";
                throw BufferEOF(error.str());
            }
            return offset;
        }
        case kTypeVarstring:
        case kTypeVarblob:
        {
            check_read_length(packed, offset, sizeof(sizetag_t));
            sizetag_t len = *(sizetag_t*)(&packed[offset]);
            offset += sizeof(sizetag_t);
            check_read_length(packed, offset, len);

            add_blob(&packed[offset], len);
            offset += len;
            return offset;
        }
        case kTypeVararray:
        {
            check_read_length(packed, offset, sizeof(sizetag_t));
            sizetag_t len = *(sizetag_t*)(&packed[offset]);
            offset += sizeof(sizetag_t);
            check_read_length(packed, offset, len);

            const ArrayType *arr = type->as_array();
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
                offset = add_packed(arr->get_element_type(), packed, offset);
            }
            if(offset > array_end) {
                stringstream error;
                error << "Datagram tried to copy array data, but array data"
                         " exceeded the expected array length of " << len << ".\n";
                throw BufferEOF(error.str());
            }
            return offset;
        }
        case kTypeStruct:
        {
            const Struct *dstruct = type->as_struct();
            size_t num_fields = dstruct->get_num_fields();
            for(unsigned int i = 0; i < num_fields; ++i) {
                offset = add_packed(dstruct->get_field(i)->get_type(), packed, offset);
            }
            return offset;
        }
        case kTypeMethod:
        {
            const Method *dmethod = type->as_method();
            size_t num_params = dmethod->get_num_parameters();
            for(unsigned int i = 0; i < num_params; ++i) {
                offset = add_packed(dmethod->get_parameter(i)->get_type(), packed, offset);
            }
            return offset;
        }
        case kTypeInvalid:
        {
            return offset;
        }
    }
    return offset;
}


} // close namespace bamboo
