// Filename: BlobValue.cpp
#include "BlobValue.h"
#include <stdexcept> // invalid_argument
#include "../bits/buffers.h"
#include "../values/Value.h"
#include "../values/IntValue.h"
#include "../module/DistributedType.h"
using namespace std;
namespace bamboo {


BlobValue::BlobValue(sizetag_t length) { m_blob = vector<uint8_t>(length, uint8_t(0)); }
BlobValue::BlobValue(const vector<uint8_t>& blob) { m_blob = blob; }

vector<uint8_t> BlobValue::pack(const DistributedType *type) const {
    switch(type->get_subtype()) {
        case kTypeBlob: {
            return m_blob;
        }
        case kTypeVarblob: {
            vector<uint8_t> ret = as_buffer(sizetag_t(m_blob.size()));
            pack_value(m_blob, ret);
            return ret;
        }
        default: {
            throw invalid_argument("can't pack blob value as non-blob type.");
        }
    }
}
void BlobValue::pack(const DistributedType *type, vector<uint8_t>& buf) const {
    switch(type->get_subtype()) {
        case kTypeBlob: {
            pack_value(m_blob, buf);
            break;
        }
        case kTypeVarblob: {
            pack_value(sizetag_t(m_blob.size()), buf);
            pack_value(m_blob, buf);
            break;
        }
        default: {
            throw invalid_argument("can't pack blob value as non-blob type.");
        }
    }
}

vector<uint8_t> BlobValue::as_blob() const {
    return m_blob;
}

sizetag_t BlobValue::size() const {
    return m_blob.size();
}

Value BlobValue::get_item(sizetag_t index) { return Value(new IntValue(m_blob[index])); }
const Value BlobValue::get_item(sizetag_t index) const { return Value(new IntValue(m_blob[index])); }
void BlobValue::set_item(sizetag_t index, const Value val) { m_blob[index] = (uint8_t)val; }


} // close namespace bamboo
