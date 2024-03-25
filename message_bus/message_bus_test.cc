#include <gtest/gtest.h>

#include "message_bus.h"

using excercise::MessageBus;

TEST(MessageBusTest, TestCase) {
  EXPECT_EQ(0, 0);
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MessageBus bus;
  
  return RUN_ALL_TESTS();
}