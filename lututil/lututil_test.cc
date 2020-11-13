#include "lututil.h"
#include "parse/parse.h"
#include "gtest/gtest.h"
#include "lutmask/lutmask.h"
#include "parse/readfile.h"
#include <string>
#include <optional>
#include <unordered_map>
#include <limits>


namespace {

// The fixture for testing class TextReader
class LutUtilTest : public ::testing::Test {
 protected:
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(LutUtilTest, LEGALTEST) {
  std::string hex_legal("fffc");
   
  std::optional<uint16_t> parsed = lut_arg_parser::parse_hex(hex_legal); 
  ASSERT_TRUE(parsed.has_value());

  lutmask::LutMask mask(*parsed, 4);
  std::string sop = lututil::generate_sop(mask);

  ASSERT_EQ(sop, "B + C + D");

}

TEST_F(LutUtilTest, LEGALFILE) {
  const std::string filename("results_file");
  const std::string dir_path("lututil");
  std::unordered_map full_map = readfile::ReadResultFile(filename, dir_path);

  ASSERT_NE(full_map.size(), 0);
  ASSERT_EQ(full_map.size() - 1, std::numeric_limits<uint16_t>::max());

  for (auto &it : full_map) {
    std::optional<uint16_t> parsed = lut_arg_parser::parse_hex(it.first); 
    lutmask::LutMask mask(*parsed, 4);
    ASSERT_EQ(it.second, lututil::generate_sop(mask));

  }
}


}
