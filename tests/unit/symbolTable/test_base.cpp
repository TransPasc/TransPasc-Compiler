#include <gtest/gtest.h>

#include "symbolTable/stackLinkedSymbolTable.hpp"

TEST(SymbolTable, SUC) {
  /*
    变量作用域示意图:
    { // top block (行号1-3,14)
        a (1)
        b (2)
        g (3)
        { // block 1 (行号4-5)
            a (4)
            c (5)
            { // block 2 (行号6-7)
                a (6)
                b (7)
                // find c (应找到block1的c，行号5)
            }
            // find g (应找到top的g，行号3)
        }
        { // block3的父块 (行号8-10)
            a (8)
            b (9)
            c (10)
            { // block 3 (行号11-13)
                a (11)
                b (12)
                c (13)
                // find g (应找到top的g，行号3)
            }
            // find c (应找到当前块的c，行号10)
        }
        f (14)
    }
  */
  auto table = std::make_unique<XYZ::StackLinkedSymbolTable>();
  EXPECT_NE(table, nullptr);
  EXPECT_EQ(table->size(), 0);

  // 进入顶层块并插入变量
  table->enterBlock(); // 进入top block
  table->insert(std::make_unique<XYZ::SymbolRecord>("a", 1));
  table->insert(std::make_unique<XYZ::SymbolRecord>("b", 2));
  table->insert(std::make_unique<XYZ::SymbolRecord>("g", 3));
  EXPECT_EQ(table->size(), 3);

  // 进入block1并插入变量
  table->enterBlock(); // 进入block1
  table->insert(std::make_unique<XYZ::SymbolRecord>("a", 4));
  table->insert(std::make_unique<XYZ::SymbolRecord>("c", 5));
  EXPECT_EQ(table->size(), 5); // top(3) + block1(2)

  // 进入block2并插入变量
  table->enterBlock(); // 进入block2
  table->insert(std::make_unique<XYZ::SymbolRecord>("a", 6));
  table->insert(std::make_unique<XYZ::SymbolRecord>("b", 7));
  EXPECT_EQ(table->size(), 7); // 累积总数可能不适用，此处应重新考虑测试方式

  // --- 在block2中查找c ---
  auto res = table->lookup("c");
  ASSERT_NE(res, nullptr);

  EXPECT_EQ(res->getLineno(), 5); // 应找到block1的c（行号5）

  table->exitBlock();          // 退出block2，回到block1
  EXPECT_EQ(table->size(), 5); // 退出后恢复block1的size

  // --- 在block1中查找g ---
  res = table->lookup("g");
  ASSERT_NE(res, nullptr);
  EXPECT_EQ(res->getLineno(), 3); // 应找到top的g（行号3）

  table->exitBlock(); // 退出block1，回到top block
  EXPECT_EQ(table->size(), 3);

  // 进入block3的父块并插入变量
  table->enterBlock(); // 进入block3的父块
  table->insert(std::make_unique<XYZ::SymbolRecord>("a", 8));
  table->insert(std::make_unique<XYZ::SymbolRecord>("b", 9));
  table->insert(std::make_unique<XYZ::SymbolRecord>("c", 10));

  // 进入block3并插入变量
  table->enterBlock(); // 进入block3
  table->insert(std::make_unique<XYZ::SymbolRecord>("a", 11));
  table->insert(std::make_unique<XYZ::SymbolRecord>("b", 12));
  table->insert(std::make_unique<XYZ::SymbolRecord>("c", 13));

  // --- 在block3中查找g ---
  res = table->lookup("g");
  ASSERT_NE(res, nullptr);
  EXPECT_EQ(res->getLineno(), 3); // 应找到top的g（行号3）

  table->exitBlock(); // 退出block3，回到父块

  // --- 在父块中查找c ---
  res = table->lookup("c");
  ASSERT_NE(res, nullptr);
  EXPECT_EQ(res->getLineno(), 10); // 应找到当前块的c（行号10）

  table->exitBlock(); // 退出父块，回到top block

  // 在top block中插入f
  table->insert(std::make_unique<XYZ::SymbolRecord>("f", 14));
  EXPECT_EQ(table->size(), 4); // a(1), b(2), g(3), f(14)

  // 退出顶层块，所有变量应不可见
  table->exitBlock();
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

  // 测试在空表中进入块
  table->enterBlock();
  EXPECT_EQ(table->size(), 0);
}

TEST(SymbolTable, InsertDuplicateInSameScope) {
  auto table = std::make_unique<XYZ::StackLinkedSymbolTable>();
  table->enterBlock();
  table->insert(std::make_unique<XYZ::SymbolRecord>("a", 1));

  // 同一作用域重复插入, throw SymbolAlreadyExists
  EXPECT_THROW(table->insert(std::make_unique<XYZ::SymbolRecord>("a", 2)),
               XYZ::SymbolTableException); // 应该抛出异常
  EXPECT_THROW(table->insert(nullptr),
               XYZ::SymbolTableException); // 插入空指针应失败
}

TEST(SymbolTable, ExitBlockWithoutEnter) {
  auto table = std::make_unique<XYZ::StackLinkedSymbolTable>();

  // 未进入块直接退出（应抛出逻辑错误）
  EXPECT_THROW(table->exitBlock(), XYZ::SymbolTableException);
}

TEST(SymbolTable, LookupNonexistentSymbol) {
  auto table = std::make_unique<XYZ::StackLinkedSymbolTable>();
  table->enterBlock();

  EXPECT_EQ(table->lookup("phantom_var"), nullptr); // 应返回空指针
  table->exitBlock();

  // 空表查找
  EXPECT_EQ(table->lookup("any"), nullptr);
}

TEST(SymbolTable, LookupWithoutBlock) {
  auto table = std::make_unique<XYZ::StackLinkedSymbolTable>();
  // 未进入块查找
  EXPECT_EQ(table->lookup("a"), nullptr); // 应返回空指针
}

TEST(SymbolTable, OverExitBlocks) {
  auto table = std::make_unique<XYZ::StackLinkedSymbolTable>();
  table->enterBlock();
  table->enterBlock();

  // 正确退出两次
  table->exitBlock();
  table->exitBlock();

  // 第三次退出应该抛出异常
  EXPECT_THROW(table->exitBlock(), XYZ::SymbolTableException);
}

TEST(SymbolTable, InsertWithoutBlock) {
  auto table = std::make_unique<XYZ::StackLinkedSymbolTable>();

  EXPECT_THROW(table->insert(std::make_unique<XYZ::SymbolRecord>("a", 1)),
               XYZ::SymbolTableException);
}

TEST(SymbolTable, CrossScopeShadowing) {
  auto table = std::make_unique<XYZ::StackLinkedSymbolTable>();
  table->enterBlock();
  table->insert(std::make_unique<XYZ::SymbolRecord>("a", 1));

  // 子作用域遮蔽
  table->enterBlock();
  EXPECT_THROW(table->insert(std::make_unique<XYZ::SymbolRecord>("a", 2)),
               XYZ::SymbolTableException); // 应该抛出异常

  // 验证查找结果
  auto res = table->lookup("a");
  ASSERT_NE(res, nullptr);
  EXPECT_EQ(res->getLineno(), 1); // 应找到父作用域的a

  // 尝试在子作用域重复插入
  EXPECT_THROW(table->insert(std::make_unique<XYZ::SymbolRecord>("a", 3)),
               XYZ::SymbolTableException); // 应该抛出异常
}
