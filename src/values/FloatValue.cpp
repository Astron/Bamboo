// Filename: FloatValue.cpp
#include "FloatValue.h"
#include <stdexcept> // invalid_argument
#include "bits/buffers.h"
#include "module/DistributedType.h"
using namespace std;
namespace bamboo {


FloatValue::FloatValue() { m_float = 0.0; }
FloatValue::FloatValue(float num) { m_float = num; }
FloatValue::FloatValue(double num) { m_float = num; }

// pack provides the packed data for the value in native endianness.
//     Throws: bad_cast
vector<uint8_t> FloatValue::pack(const DistributedType *type) const {
    switch(type->get_subtype()) {
        case kTypeFloat32:
            return as_buffer(float(m_float));
        case kTypeFloat64:
            return as_buffer(double(m_float));
        default:
            throw invalid_argument("can't pack floating-point value as non-float type.");
    }
}
void FloatValue::pack(const DistributedType *type, vector<uint8_t>& buf) const {
    switch(type->get_subtype()) {
        case kTypeFloat32: {
            pack_value(float(m_float), buf);
            break;
        }
        case kTypeFloat64: {
            pack_value(double(m_float), buf);
            break;
        }
        default: {
            throw invalid_argument("can't pack floating-point value as non-float type.");
        }
    }
}

float FloatValue::as_float32() const {
    return m_float;
}
double FloatValue::as_float64() const {
    return m_float;
}


} // close namespace bamboo
