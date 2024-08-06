#include <gtest/gtest.h>
#include "../include/key_value_store.h"

class KeyValueStoreTest : public ::testing::Test {
    protected:
        void SetUp() override {
            store = new KeyValueStore();
        }

        void TearDown() override {
            delete store;
        }

        KeyValueStore* store;
};

TEST_F(KeyValueStoreTest, SetAndGet) {
    store->set("test_key", "test_value");
    EXPECT_EQ(store->get("test_key"), "test_value");
}

TEST_F(KeyValueStoreTest, SetDelGet) {
    store->set("test_key", "test_value");
    store->del("test_key");
    EXPECT_FALSE(store->get("test_key").has_value());
}

TEST_F(KeyValueStoreTest, LeftPushTwiceLen) {
    store->lPush("test_list", "item1");
    store->lPush("test_list", "item2");
    EXPECT_EQ(store->lLen("test_list"), 2);
}

TEST_F(KeyValueStoreTest, LeftPushLRange) {
    store->lPush("test_list", "item1");
    EXPECT_EQ(store->lRange("test_list", 0, 0), (std::deque<std::string>{"item1"}));
}

TEST_F(KeyValueStoreTest, LeftPushTwiceRightPopLRange) {
    store->lPush("test_list", "item1");
    store->lPush("test_list", "item2");
    store->rPop("test_list");
    EXPECT_EQ(store->lRange("test_list", 0, 0), (std::deque<std::string>{"item2"}));
}

TEST_F(KeyValueStoreTest, RightPushTwiceLeftPopLRange) {
    store->rPush("test_list", "item1");
    store->rPush("test_list", "item2");
    store->lPop("test_list");
    EXPECT_EQ(store->lRange("test_list", 0, 0), (std::deque<std::string>{"item2"}));
}