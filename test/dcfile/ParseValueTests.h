// Filename: ParseValueTests.h
#include "../test_helper.h"
#include <bamboo/module/module>
#include <bamboo/dcfile/parse.h>
using namespace bamboo;
using namespace std;

class ParseValueTests : public CxxTest::TestSuite
{
  public:
    void testParseChar()
    {
      Numeric *type = new Numeric(Subtype_Char);
      Value value = Value::parse(type, "'k'", had_error);

      assert(not had_error);
      expect_equals(value.char_, 'k');
    }

    void testParseCharArray()
    {
      bool had_error = false;

      // Parser for String array


      // Parse for custom Char Array
      Array *type = new Array(Type::Char, NumericRange(0u, 32u));
      Value value = Value::parse(type, "['T','e','s','t']", had_error);

      assert(not had_error);
      expect_equals(value.string_, "Test");
    }

    void testParseString()
    {
      bool had_error = false;
      Array *type = Array::String();
      Value value = Value::parse(type, "\"Test\"", had_error);

      assert(not had_error);
      expect_equals(value.string_, "Test");
    }
};
