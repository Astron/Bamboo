// Filename: ValueInterface.cpp
#include "ValueInterface.h"
#include <stdexcept> // out_of_range
#include <typeinfo>  // bad_cast
#include "values/Value.h"
using namespace std;
namespace bamboo {


// implicit conversion operators.
char ValueInterface::as_char() const { throw bad_cast(); }
int8_t ValueInterface::as_int8() const { throw bad_cast(); }
int16_t ValueInterface::as_int16() const { throw bad_cast(); }
int32_t ValueInterface::as_int32() const { throw bad_cast(); }
int64_t ValueInterface::as_int64() const { throw bad_cast(); }
uint8_t ValueInterface::as_uint8() const { throw bad_cast(); }
uint16_t ValueInterface::as_uint16() const { throw bad_cast(); }
uint32_t ValueInterface::as_uint32() const { throw bad_cast(); }
uint64_t ValueInterface::as_uint64() const { throw bad_cast(); }
float ValueInterface::as_float32() const { throw bad_cast(); }
double ValueInterface::as_float64() const { throw bad_cast(); }
string ValueInterface::as_string() const { throw bad_cast(); }
vector<uint8_t> ValueInterface::as_blob() const { throw bad_cast(); }

// size returns the size (the number of items) in a value.
//     Size may be called on an array, string, blob, struct, or function value.
sizetag_t ValueInterface::size() const { return 0; }

// array_accessor like operations
Value ValueInterface::get_item(sizetag_t) {
	throw out_of_range("get_item isn't allowed for this type of value.");
}
Value ValueInterface::get_item(const std::string&) {
	throw out_of_range("get_item by item name isn't allowed for this type of value.");
}
const Value ValueInterface::get_item(sizetag_t) const {
	throw out_of_range("get_item isn't allowed for this type of value.");
}
const Value ValueInterface::get_item(const std::string&) const {
	throw out_of_range("get_item by item name isn't allowed for this type of value.");
}
void ValueInterface::set_item(sizetag_t, const Value) {
	throw out_of_range("set_item isn't allowed for this type of value.");
}
void ValueInterface::set_item(const std::string&, const Value) {
	throw out_of_range("set_item by item name isn't allowed for this type of value.");
}


} // close namespace bamboo
