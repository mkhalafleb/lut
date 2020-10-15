#include "parse.h"
#include "gtest/gtest.h"
#include <string>
#include <optional>

namespace {

// The fixture for testing class TextReader
class ParseTest : public ::testing::Test {
 protected:
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(ParseTest, LEGALPARSE) {
  std::string hex_legal("ffee");

  std::optional<uint16_t> val = lut_arg_parser::parse_hex(hex_legal);
  ASSERT_TRUE(val.has_value());
  ASSERT_EQ(*val, 0xffee);
}

TEST_F(ParseTest, LEGALLESS4) {
  std::string hex_legal("fa");

  std::optional<uint16_t> val = lut_arg_parser::parse_hex(hex_legal);
  ASSERT_TRUE(val.has_value());
  ASSERT_EQ(*val, 0xfa);
}

TEST_F(ParseTest, ILLEGAL) {
  std::string hex_legal("fahello");

  std::optional<uint16_t> val = lut_arg_parser::parse_hex(hex_legal);
  ASSERT_FALSE(val.has_value());
}

}  // namespace

