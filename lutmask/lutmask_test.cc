#include "lutmask.h"
#include "gtest/gtest.h"
#include <exception>

namespace {

// The fixture for testing class TextReader
class LutMaskTest : public ::testing::Test {
 protected:
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(LutMaskTest, BADCLASS) {
  ASSERT_THROW(lutmask::LutMask badmask(0xff, 5);,std::out_of_range);
}

TEST_F(LutMaskTest, GOODCLASS) {
  ASSERT_NO_THROW(lutmask::LutMask goodmask(0xff, 4));
}


}  // namespace

