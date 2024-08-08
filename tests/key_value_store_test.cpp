#include <gtest/gtest.h>
#include "../include/key_value_store.h"

class KeyValueStoreTest : public ::testing::Test {
    protected:
        void SetUp() override {
            store = new KeyValueStore(5, std::make_unique<LRU>());
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

TEST_F(KeyValueStoreTest, RightLeftPushLRange) {
    store->rPush("test_list", "item1");
    store->rPush("test_list", "item2");
    store->lPush("test_list", "item0");
    store->rPush("test_list", "item3");
    EXPECT_EQ(store->lRange("test_list", 0, 3), (std::deque<std::string>{"item0", "item1", "item2", "item3"}));
}

TEST_F(KeyValueStoreTest, SetAddRemCard) {
    store->sAdd("test_set", "item1");
    store->sAdd("test_set", "item2");
    store->sAdd("test_set", "item3");
    EXPECT_EQ(store->sRem("test_set", "item2"), 1);
    EXPECT_EQ(store->sCard("test_set"), 2);
}

TEST_F(KeyValueStoreTest, SetAddRemMembers) {
    EXPECT_EQ(store->sMembers("test_set"), (std::unordered_set<std::string>{}));
    store->sAdd("test_set", "item1");
    store->sAdd("test_set", "item2");
    store->sAdd("test_set", "item3");
    EXPECT_EQ(store->sRem("test_set", "item2"), 1);
    EXPECT_EQ(store->sMembers("test_set"), (std::unordered_set<std::string>{"item1", "item3"}));
}

TEST_F(KeyValueStoreTest, SetAddIsMember) {
    EXPECT_EQ(store->sIsMember("test_set", "item1"), 0);
    store->sAdd("test_set", "item1");
    EXPECT_EQ(store->sIsMember("test_set", "item1"), 1);
}

TEST_F(KeyValueStoreTest, LRUEvictString) {
    store->set("test_key1", "item1");
    store->set("test_key2", "item2");
    store->set("test_key3", "item3");
    store->set("test_key4", "item4");
    store->set("test_key5", "item5");
    store->set("test_key6", "item6");
    EXPECT_FALSE(store->get("test_key1").has_value());
}

