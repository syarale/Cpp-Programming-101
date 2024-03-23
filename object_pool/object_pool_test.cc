#include <vector>
#include <gtest/gtest.h>

#include "object_pool.h"

using excercise::ObjectPool;

constexpr uint32_t SIZE = 100;
constexpr uint32_t X_VALUE = 20;
constexpr uint32_t Y_VALUE = 21;

uint32_t count = 0;

class Foo {
  public:
   explicit Foo(int x, int y) : x_(x), y_(y) { count ++; }
   ~Foo() { count --; }

   uint32_t get_x() { return x_; }
   uint32_t get_y() { return y_; }

  private:
   uint32_t x_ = 0;
   uint32_t y_ = 0;
};

TEST(ObjectPoolTest, TestCase) {
  {
    // TODO(leisy): fix compile error: ambiguity when instantiating
    // auto IncreaseNum = [](Foo* foo){ };
    // auto object_pool = std::make_shared<ObjectPool<Foo>>(SIZE, IncreaseNum, X_VALUE, Y_VALUE);
    auto object_pool = std::make_shared<ObjectPool<Foo>>(SIZE, X_VALUE, Y_VALUE);
    EXPECT_EQ(count, SIZE);
    EXPECT_EQ(object_pool->GetFreeNums(), SIZE);
    {
      std::vector<std::shared_ptr<Foo>> tmp_vec;
      for (uint32_t i = 0; i < SIZE; i ++) {
        std::shared_ptr<Foo> obj_ptr = object_pool->GetObject();
        tmp_vec.push_back(obj_ptr);

        EXPECT_EQ(obj_ptr->get_x(), X_VALUE);
        EXPECT_EQ(obj_ptr->get_y(), Y_VALUE);
      }
      EXPECT_EQ(object_pool->GetFreeNums(), 0);

      std::shared_ptr<Foo> obj_ptr = object_pool->GetObject();
      ASSERT_EQ(obj_ptr, nullptr);
    }
    EXPECT_EQ(object_pool->GetFreeNums(), SIZE);
    
    std::shared_ptr<Foo> obj_ptr = object_pool->GetObject();
    EXPECT_EQ(obj_ptr->get_x(), X_VALUE);
    EXPECT_EQ(obj_ptr->get_y(), Y_VALUE);
    EXPECT_EQ(object_pool->GetFreeNums(), SIZE - 1);
  }

  EXPECT_EQ(count, 0);
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}