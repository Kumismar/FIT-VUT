#include "gtest/gtest.h"

extern "C" {
    #include "../src/headers/constants.h"
    #include "../src/headers/stack.h"
    #include "../src/headers/pointerList.h"
}

TEST(StackPush, OneElement)
{
    Stack stack;
    initStack(&stack);

    push(&stack, (char*)"Node1");
    EXPECT_STREQ(stack.top->s, "Node1");
}

TEST(StackPush, MoreElements)
{
    Stack stack;
    initStack(&stack);

    push(&stack, (char*)"Node1");
    push(&stack, (char*)"Node2");
    push(&stack, (char*)"Node3");
    push(&stack, (char*)"Node4");
    EXPECT_STREQ(stack.top->s, "Node4");
}

TEST(StackPop, PopOneElement)
{
    Stack stack;
    initStack(&stack);

    push(&stack, (char*)"Node1");
    pop(&stack);
    EXPECT_TRUE(stack.top == NULL);
}

TEST(StackTop, OneElement)
{
    Stack stack;
    initStack(&stack);

    push(&stack, (char*)"Node1");
    EXPECT_STREQ(top(&stack), (char*)"Node1");
}

TEST(StackTop, FourPushTwoPop)
{
    Stack stack;
    initStack(&stack);

    push(&stack, (char*)"Node1");
    push(&stack, (char*)"Node2");
    push(&stack, (char*)"Node3");
    push(&stack, (char*)"Node4");
    pop(&stack);
    pop(&stack);
    EXPECT_STREQ(top(&stack), (char*)"Node2");
}