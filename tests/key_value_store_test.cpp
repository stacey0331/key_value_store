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
    store->set("key1", "value1");
    EXPECT_EQ(store->get("key1"), "value1");
}

TEST_F(KeyValueStoreTest, SetDelGet) {
    store->set("key2", "value2");
    store->del("key2");
    EXPECT_FALSE(store->get("key2").has_value());
}

TEST_F(KeyValueStoreTest, LeftPushTwiceLen) {
    store->lPush("key3", "value3.0");
    store->lPush("key3", "value3.1");
    EXPECT_EQ(store->lLen("key3"), 2);
}

TEST_F(KeyValueStoreTest, LeftPushLRange) {
    store->lPush("key3", "value3.0");
    EXPECT_EQ(store->lRange("key3", 0, 0), (std::deque<std::string>{"value3.0"}));
}

TEST_F(KeyValueStoreTest, LeftPushTwiceRightPopLRange) {
    store->lPush("key3", "value3.0");
    store->lPush("key3", "value3.1");
    store->rPop("key3");
    EXPECT_EQ(store->lRange("key3", 0, 0), (std::deque<std::string>{"value3.1"}));
}