// Filename: StringValue.cpp
#include "StringValue.h"
#include <stdexcept> // invalid_argument
#include "../bits/buffers.h"
#include "../values/Value.h"
#include "../values/IntValue.h"
#include "../module/DistributedType.h"
using namespace std;
namespace bamboo {

StringValue::StringValue(sizetag_t length) { m_string = string(length, '\0'); }
StringValue::StringValue(const string& str) { m_string = str; }

vector<uint8_t> StringValue::pack(const DistributedType *type) const {
    switch(type->get_subtype()) {
        case kTypeString: {
            return vector<uint8_t>(m_string.begin(), m_string.end());
        }
        case kTypeVarstring: {
            vector<uint8_t> ret = as_buffer(sizetag_t(m_string.length()));
            pack_value(m_string, ret);
            return ret;
        }
        default: {
            throw invalid_argument("can't pack string value as non-string type.");
        }
    }
}
void StringValue::pack(const DistributedType *type, vector<uint8_t>& buf) const {
    switch(type->get_subtype()) {
        case kTypeString: {
            pack_value(m_string, buf);
            break;
        }
        case kTypeVarstring: {
            pack_value(sizetag_t(m_string.length()), buf);
            pack_value(m_string, buf);
            break;
        }
        default: {
            throw invalid_argument("can't pack string value as non-string type.");
        }
    }
}

string StringValue::as_string() const {
    return m_string;
}

sizetag_t StringValue::size() const {
    return m_string.length();
}

Value StringValue::_getitem_(sizetag_t index) { return Value(new IntValue(m_string[index])); }
const Value StringValue::_getitem_(sizetag_t index) const { return Value(new IntValue(m_string[index])); }
void StringValue::_setitem_(sizetag_t index, const Value val) { m_string[index] = (char)val; }


} // close namespace bamboo
