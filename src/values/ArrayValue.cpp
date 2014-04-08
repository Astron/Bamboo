// Filename: ArrayValue.cpp
#include "ArrayValue.h"
#include <stdexcept> // invalid_argument
#include "bits/buffers.h"
#include "values/Value.h"
#include "values/IntValue.h"
#include "module/ArrayType.h"
using namespace std;
namespace bamboo {


ArrayValue::ArrayValue() : m_elements() {}

// FIXME: it should be possible to make the pack functions a whole bunch more efficient
vector<uint8_t> ArrayValue::pack(const DistributedType *type) const {
    const ArrayType *arr = type->as_array();
    if(arr == nullptr) {
        throw invalid_argument("can't pack array value as non-array type.");
    }

    vector<uint8_t> value;
    for(auto it = m_elements.begin(); it != m_elements.end(); ++it) {
        it->pack(arr->get_element_type(), value);
    }

    if(arr->has_fixed_size()) {
        return value;
    } else {
        vector<uint8_t> buf = as_buffer(sizetag_t(value.size()));
        pack_value(value, buf);
        return buf;
    }
}
void ArrayValue::pack(const DistributedType *type, vector<uint8_t>& buf) const {
    return pack_value(pack(type), buf);
}

sizetag_t ArrayValue::size() const {
    return m_elements.size();
}

Value ArrayValue::get_item(sizetag_t index) { return m_elements[index]; }
const Value ArrayValue::get_item(sizetag_t index) const { return m_elements[index]; }
void ArrayValue::set_item(sizetag_t index, const Value val) { m_elements[index] = val; }

void ArrayValue::append(const Value val) { m_elements.push_back(val); }


} // close namespace bamboo
