#include <gtest/gtest.h>

#include "symbolTable/stackLinkedSymbolTable.hpp"

TEST(SymbolTable, SUC) {
  auto table = std::make_unique<XYZ::StackLinkedSymbolTable>();
  EXPECT_NE(table, nullptr);
  EXPECT_EQ(table->size(), 0);
  table->insert("a", std::make_unique<XYZ::SymbolRecord>());
  EXPECT_EQ(table->size(), 1);
  table->insert("b", std::make_unique<XYZ::SymbolRecord>());
  EXPECT_EQ(table->size(), 2);
  auto res = table->lookup("a");
  EXPECT_NE(res, nullptr);
  res = table->lookup("b");
  EXPECT_NE(res, nullptr);

  // into new block
  table->enterBlock();
  EXPECT_EQ(table->size(), 2);
  table->insert("a", std::make_unique<XYZ::SymbolRecord>());
  EXPECT_EQ(table->size(), 3);
  res = table->lookup("a");
  EXPECT_NE(res, nullptr);
  res = table->lookup("b");
  EXPECT_NE(res, nullptr);

  // into new block
  table->enterBlock();
  EXPECT_EQ(table->size(), 3);
  table->insert("a", std::make_unique<XYZ::SymbolRecord>());
  EXPECT_EQ(table->size(), 4);
  res = table->lookup("a");
  EXPECT_NE(res, nullptr);
  // exit block
  table->exitBlock();
  EXPECT_EQ(table->size(), 3);
  res = table->lookup("a");
  EXPECT_NE(res, nullptr);
  // exit block
  table->exitBlock();
  EXPECT_EQ(table->size(), 2);
  res = table->lookup("a");
  EXPECT_NE(res, nullptr);
  res = table->lookup("b");
  EXPECT_NE(res, nullptr);

  // remove
  table->remove("a");
  EXPECT_EQ(table->size(), 1);
  res = table->lookup("a");
  EXPECT_EQ(res, nullptr);
  res = table->lookup("b");
  EXPECT_NE(res, nullptr);
  // clear
  table->clear();
  EXPECT_EQ(table->size(), 0);
  res = table->lookup("a");
  EXPECT_EQ(res, nullptr);
}

// 测试边缘情况
TEST(SymbolTable, EdgeCase) {
  auto table = std::make_unique<XYZ::StackLinkedSymbolTable>();
  EXPECT_NE(table, nullptr);
  EXPECT_EQ(table->size(), 0);

  // 测试在空表中查找
  auto res = table->lookup("a");
  EXPECT_EQ(res, nullptr);

  // 测试在空表中删除
  EXPECT_FALSE(table->remove("a"));

  // 测试在空表中更新
  EXPECT_FALSE(table->update("a", nullptr));

  // 测试在空表中进入块
  table->enterBlock();
  EXPECT_EQ(table->size(), 0);
}