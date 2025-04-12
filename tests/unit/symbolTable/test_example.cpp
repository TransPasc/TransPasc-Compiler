#include <gtest/gtest.h>

// #include "my_lib.h"

int add(int a, int b) { return a + b; }  // 示例函数
class Database {
 public:
  void connect(const std::string &db_name) { /* 连接数据库 */ }
  void clear() { /* 清空数据库 */ }
  bool insert(const std::string &key, const std::string &value) {
    return true;  // 插入数据
  }
};
// 测试用例示例

TEST(MathTest, AddBasic) {
  EXPECT_EQ(add(2, 3), 5);   // 普通断言
  ASSERT_NE(add(-1, 1), 1);  // 致命断言（失败时终止当前用例）
}

// 测试夹具示例
class DatabaseTest : public ::testing::Test {
 protected:
  void SetUp() override { db.connect("test.db"); }
  void TearDown() override { db.clear(); }
  Database db;
};

TEST_F(DatabaseTest, InsertData) { EXPECT_TRUE(db.insert("key", "value")); }
