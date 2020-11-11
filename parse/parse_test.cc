#include "parse.h"
#include "readfile.h"
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



// The fixture for testing class readfile
class ReadFileTest : public ::testing::Test {
 protected:
};

TEST_F(ReadFileTest, READFILE) {
  std::string filename("results_file");
  std::unordered_map<std::string, std::string> sop_map = readfile::ReadResultFile(filename);


  // 5 should be A' & C' & D'
  // ffff should be 1
  // 0 0
  // fc00 D & (B + C)
  //
  ASSERT_EQ(sop_map.find("5")->second, "A' & C' & D'");
  ASSERT_EQ(sop_map.find("ffff")->second, "1");
  ASSERT_EQ(sop_map.find("0")->second, "0");
  ASSERT_EQ(sop_map.find("fc00")->second, "D & (B + C)");
}

}  // namespace

