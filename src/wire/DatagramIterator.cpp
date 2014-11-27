#include "DatagramIterator.h"
#include "../bits/buffers.h"
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


// read_value interprets the following data as a value for the Type in native endianness.
Value DatagramIterator::read_value(const Type *type)
{
    // FIXME: Need to unpack the values directly from little-endian for better performance.
    vector<uint8_t> packed = read_packed(type);
    return Value::unpack(type, packed);
}

vector<uint8_t> DatagramIterator::read_packed(const Type *type)
{
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

void DatagramIterator::read_packed(const Type *type, vector<uint8_t>& buffer)
{
#ifndef PLATFORM_BIG_ENDIAN
    if(type->has_fixed_size()) {
        // If we're a fixed-sized type like uint, int, float, etc
        // or any other type lucky enough to be fixed size, read_packed is just a copy
        vector<uint8_t> data = read_data(type->fixed_size());
        pack_value(data, buffer);
        return;
    }
#endif

    // For the unlucky types/machines, we have to figure out their size manually
    switch(type->subtype()) {
    case Subtype_Numeric:
        switch(type->fixed_size()) {
        case 1:
            pack_value(read_uint8(), buffer);
            break;
        case 2:
            pack_value(read_uint16(), buffer);
            break;
        case 4:
            pack_value(read_uint32(), buffer);
            break;
        case 8:
            pack_value(read_uint64(), buffer);
            break;
        default:
            break;
        }
        break;
    case Subtype_String:
    case Subtype_Blob:
        if(type->has_fixed_size()) {
            pack_value(read_data(type->fixed_size()), buffer);
        } else {
            sizetag_t len = read_size();
            pack_value(len, buffer);
            pack_value(read_data(len), buffer);
        }
        break;
    case Subtype_Array:
        {
            sizetag_t len;
            const Array *arr = type->as_array();

            if(arr->has_fixed_size()) {
                len = type->fixed_size();
            } else {
                len = read_size();
                pack_value(len, buffer);
            }

            if(arr->element_type()->fixed_size() == 1) {
                // If the element size is 1, we don't have to worry about endianness
                pack_value(read_data(len), buffer);
                break;
            }

            // Read elements from the array till we reach the expected size
            sizetag_t array_end = m_offset + len;
            while(m_offset < array_end) {
                read_packed(arr->element_type(), buffer);
            }
            if(m_offset > array_end) {
                stringstream error;
                error << "Datagram iterator tried to read array data, but array data"
                      " exceeded the expected array length of " << len << ".\n";
                throw DatagramIteratorEOF(error.str());
            }
        }
        break;
    case Subtype_Struct:
        {
            const Struct *dstruct = type->as_struct();
            size_t num_fields = dstruct->num_fields();
            for(unsigned int i = 0; i < num_fields; ++i) {
                read_packed(dstruct->nth_field(i)->type(), buffer);
            }
        }
        break;
    case Subtype_Method:
        {
            const Method *dmethod = type->as_method();
            size_t num_params = dmethod->num_params();
            for(unsigned int i = 0; i < num_params; ++i) {
                read_packed(dmethod->nth_param(i)->type(), buffer);
            }
        }
        break;
    case Subtype_None:
        break;
    }
}

// skip_type can be used to seek past the packed data for a Type.
//     Throws DatagramIteratorEOF if it skips past the end of the datagram.
void DatagramIterator::skip_type(const Type *type)
{
    if(type->has_fixed_size()) {
        sizetag_t length = type->fixed_size();
        check_read_length(length);
        m_offset += length;
        return;
    }

    switch(type->subtype()) {
    case Subtype_String:
    case Subtype_Blob:
    case Subtype_Array:
        {
            sizetag_t length = read_size();
            check_read_length(length);
            m_offset += length;
        }
        break;
    case Subtype_Struct:
        {
            const Struct *dstruct = type->as_struct();
            size_t num_fields = dstruct->num_fields();
            for(unsigned int i = 0; i < num_fields; ++i) {
                skip_type(dstruct->nth_field(i)->type());
            }
        }
        break;
    case Subtype_Method:
        {
            const Method *dmethod = type->as_method();
            size_t num_params = dmethod->num_params();
            for(unsigned int i = 0; i < num_params; ++i) {
                skip_type(dmethod->nth_param(i)->type());
            }
        }
        break;
    case Subtype_None:
        break;
    default:
        break;
    }
}


} // close namespace bamboo
