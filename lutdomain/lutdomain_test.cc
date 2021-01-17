#include "lutdomain.h"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class TextReader
class LutDomainTest : public ::testing::Test {
 protected:
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(LutDomainTest, EQUALDOMAIN) {
  lutdomain::LutDomain d1(4, {'A', 'B', 'C', 'D'});
  lutdomain::LutDomain d2(4, {'A', 'B', 'C', 'D'});

  EXPECT_EQ(d1, d2);

}

TEST_F(LutDomainTest, NOTEQUALSIZE) {
  lutdomain::LutDomain d1(4, {'A', 'B', 'C', 'D'});
  lutdomain::LutDomain d2(3, {'A', 'B', 'C', 'D'});

  EXPECT_NE(d1, d2);
}

TEST_F(LutDomainTest, NOTEQUALARRAY) {
  lutdomain::LutDomain d1(3, {'A', 'Q', 'C', 'D'});
  lutdomain::LutDomain d2(3, {'A', 'B', 'C', 'D'});

  EXPECT_NE(d1, d2);
}

TEST_F(LutDomainTest, EQUALSMALLER) {
  lutdomain::LutDomain d1(3, {'A', 'B', 'C', 'D'});
  lutdomain::LutDomain d2(3, {'A', 'B', 'C', 'D'});

  EXPECT_EQ(d1, d2);
}




}  // namespace

