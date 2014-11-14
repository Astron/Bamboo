#include "Datagram.h"
#include "../module/Numeric.h"
#include "../module/Array.h"
#include "../module/Struct.h"
#include "../module/Method.h"
#include "../module/Field.h"
#include "../module/Parameter.h"
#include "../module/Value.h"
using namespace std;
namespace bamboo
{


void Datagram::add_value(const Value& value)
{
    add_value(&value);
};

void Datagram::add_value(const Value *value)
{
    // FIXME: Need to pack the values directly into little-endian for better performance.

    vector<uint8_t> packed = value->pack();

#ifdef PLATFORM_BIG_ENDIAN
    add_packed(type, packed); // endian-swap all values
#else
    add_data(packed);
#endif
};

// add_packed adds data from a packed value, returning the number of bytes read from the buffer.
sizetag_t Datagram::add_packed(const Type *type, const vector<uint8_t>& packed, sizetag_t offset)
{
    switch(type->subtype()) {
    case kTypeChar:
        {
            check_read_length(packed, offset, sizeof(char));
            char r = *(char *)(&packed[offset]);
            offset += sizeof(char);
            add_char(r);
            return offset;
        }
    case kTypeInt8:
        {
            check_read_length(packed, offset, sizeof(int8_t));
            int8_t r = *(int8_t *)(&packed[offset]);
            offset += sizeof(int8_t);
            add_int8(r);
            return offset;
        }
    case kTypeInt16:
        {
            check_read_length(packed, offset, sizeof(int16_t));
            int16_t r = *(int16_t *)(&packed[offset]);
            offset += sizeof(int16_t);
            add_int16(r);
            return offset;
        }
    case kTypeInt32:
        {
            check_read_length(packed, offset, sizeof(int32_t));
            int32_t r = *(int32_t *)(&packed[offset]);
            offset += sizeof(int32_t);
            add_int32(r);
            return offset;
        }
    case kTypeInt64:
        {
            check_read_length(packed, offset, sizeof(int64_t));
            int64_t r = *(int64_t *)(&packed[offset]);
            offset += sizeof(int64_t);
            add_int64(r);
            return offset;
        }
    case kTypeUint8:
        {
            check_read_length(packed, offset, sizeof(uint8_t));
            uint8_t r = *(uint8_t *)(&packed[offset]);
            offset += sizeof(uint8_t);
            add_uint8(r);
            return offset;
        }
    case kTypeUint16:
        {
            check_read_length(packed, offset, sizeof(uint16_t));
            uint16_t r = *(uint16_t *)(&packed[offset]);
            offset += sizeof(uint16_t);
            add_uint16(r);
            return offset;
        }
    case kTypeUint32:
        {
            check_read_length(packed, offset, sizeof(uint32_t));
            uint32_t r = *(uint32_t *)(&packed[offset]);
            offset += sizeof(uint32_t);
            add_uint32(r);
            return offset;
        }
    case kTypeUint64:
        {
            check_read_length(packed, offset, sizeof(uint64_t));
            uint64_t r = *(uint64_t *)(&packed[offset]);
            offset += sizeof(uint64_t);
            add_uint64(r);
            return offset;
        }
    case kTypeFloat32:
        {
            check_read_length(packed, offset, sizeof(float));
            float r = *(float *)(&packed[offset]);
            offset += sizeof(float);
            add_float32(r);
            return offset;
        }
    case kTypeFloat64:
        {
            check_read_length(packed, offset, sizeof(double));
            double r = *(double *)(&packed[offset]);
            offset += sizeof(double);
            add_float64(r);
            return offset;
        }
    case kTypeFixed:
        {
            const Numeric *numeric = type->as_numeric();
            check_read_length(packed, offset, numeric->fixed_size());
            if(numeric->is_signed()) {
                switch(numeric->fixed_size()) {
                case 1:
                    {
                        int8_t r = *(int8_t *)(&packed[offset]);
                        offset += sizeof(int8_t);
                        add_int8(r);
                        return offset;
                    }
                case 2:
                    {
                        int16_t r = *(int16_t *)(&packed[offset]);
                        offset += sizeof(int16_t);
                        add_int16(r);
                        return offset;
                    }
                case 4:
                    {
                        int32_t r = *(int32_t *)(&packed[offset]);
                        offset += sizeof(int32_t);
                        add_int32(r);
                        return offset;
                    }
                case 8:
                    {
                        int64_t r = *(int64_t *)(&packed[offset]);
                        offset += sizeof(int64_t);
                        add_int64(r);
                        return offset;
                    }
                default:
                    return offset;
                }
            } else {
                switch(numeric->fixed_size()) {
                case 1:
                    {
                        uint8_t r = *(uint8_t *)(&packed[offset]);
                        offset += sizeof(uint8_t);
                        add_uint8(r);
                        return offset;
                    }
                case 2:
                    {
                        uint16_t r = *(uint16_t *)(&packed[offset]);
                        offset += sizeof(uint16_t);
                        add_uint16(r);
                        return offset;
                    }
                case 4:
                    {
                        uint32_t r = *(uint32_t *)(&packed[offset]);
                        offset += sizeof(uint32_t);
                        add_uint32(r);
                        return offset;
                    }
                case 8:
                    {
                        uint64_t r = *(uint64_t *)(&packed[offset]);
                        offset += sizeof(uint64_t);
                        add_uint64(r);
                        return offset;
                    }
                default:
                    return offset;
                }
            }
        }
    case kTypeString:
    case kTypeBlob:
        {
            if(type->has_fixed_size()) {
                sizetag_t len = type->fixed_size();
                check_read_length(packed, offset, len);
                add_data(&packed[offset], len);
                offset += len;
                return offset;
            } else {
                check_read_length(packed, offset, sizeof(sizetag_t));
                sizetag_t len = *(sizetag_t *)(&packed[offset]);
                offset += sizeof(sizetag_t);
                check_read_length(packed, offset, len);

                add_blob(&packed[offset], len);
                offset += len;
                return offset;
            }
        }
    case kTypeArray:
        {
            sizetag_t len;
            const Array *arr = type->as_array();

            if(type->has_fixed_size()) {
                len = type->fixed_size();
                check_read_length(packed, offset, len);

                if(arr->element_type()->fixed_size() == 1) {
                    // If the element size is 1, we don't have to worry about endianness
                    add_blob(&packed[offset], len);
                    offset += len;
                    return offset;
                }
            } else {
                check_read_length(packed, offset, sizeof(sizetag_t));
                len = *(sizetag_t *)(&packed[offset]);
                offset += sizeof(sizetag_t);
                check_read_length(packed, offset, len);

                if(arr->element_type()->fixed_size() == 1) {
                    // If the element size is 1, we don't have to worry about endianness
                    add_blob(&packed[offset], len);
                    offset += len;
                    return offset;
                }

                add_size(packed.size());
            }

            // Copy elements from the array till we reach the expected size
            sizetag_t array_end = offset + len;
            while(offset < array_end) {
                offset = add_packed(arr->element_type(), packed, offset);
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
            size_t num_fields = dstruct->num_fields();
            for(unsigned int i = 0; i < num_fields; ++i) {
                offset = add_packed(dstruct->nth_field(i)->type(), packed, offset);
            }
            return offset;
        }
    case kTypeMethod:
        {
            const Method *dmethod = type->as_method();
            size_t num_params = dmethod->num_params();
            for(unsigned int i = 0; i < num_params; ++i) {
                offset = add_packed(dmethod->nth_param(i)->type(), packed, offset);
            }
            return offset;
        }
    case kTypeNone:
        return offset;
    }
    return offset;
}

void swap(Datagram& lhs, Datagram& rhs)
{
    using std::swap;

    swap(lhs.buf, rhs.buf);
    swap(lhs.buf_cap, rhs.buf_cap);
    swap(lhs.buf_offset, rhs.buf_offset);
}


} // close namespace bamboo
