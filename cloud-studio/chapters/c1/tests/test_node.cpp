#include <gtest/gtest.h>
#include <node.h>

class NodeTest : public testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(NodeTest, IsVitrualDestructor) {
  auto node1 = create_node(PageNodeType);
  auto node2 = create_node(RectangleNodeType);
  delete node1;
  delete node2;
  EXPECT_EQ(BaseNode::get_count(), 0);
}
