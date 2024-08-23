#include <gtest/gtest.h>
#include <thread>
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

TEST_F(KeyValueStoreTest, DBTest) {
    EXPECT_EQ(store->testDb("test_key"), "yo!");
}

TEST_F(KeyValueStoreTest, SetAndGet) {
    store->set(1, "test_key", "test_value");
    EXPECT_EQ(store->get(1, "test_key"), "test_value");
}

// TEST_F(KeyValueStoreTest, SetDelGet) {
//     store->set("test_key", "test_value");
//     store->del("test_key");
//     EXPECT_FALSE(store->get("test_key").has_value());
// }

// TEST_F(KeyValueStoreTest, LeftPushTwiceLen) {
//     store->lPush("test_list", "item1");
//     store->lPush("test_list", "item2");
//     EXPECT_EQ(store->lLen("test_list"), 2);
// }

// TEST_F(KeyValueStoreTest, LeftPushLRange) {
//     store->lPush("test_list", "item1");
//     EXPECT_EQ(store->lRange("test_list", 0, 0), (std::deque<std::string>{"item1"}));
// }

// TEST_F(KeyValueStoreTest, LeftPushTwiceRightPopLRange) {
//     store->lPush("test_list", "item1");
//     store->lPush("test_list", "item2");
//     store->rPop("test_list");
//     EXPECT_EQ(store->lRange("test_list", 0, 0), (std::deque<std::string>{"item2"}));
// }

// TEST_F(KeyValueStoreTest, RightPushTwiceLeftPopLRange) {
//     store->rPush("test_list", "item1");
//     store->rPush("test_list", "item2");
//     store->lPop("test_list");
//     EXPECT_EQ(store->lRange("test_list", 0, 0), (std::deque<std::string>{"item2"}));
// }

// TEST_F(KeyValueStoreTest, RightLeftPushLRange) {
//     store->rPush("test_list", "item1");
//     store->rPush("test_list", "item2");
//     store->lPush("test_list", "item0");
//     store->rPush("test_list", "item3");
//     EXPECT_EQ(store->lRange("test_list", 0, 3), (std::deque<std::string>{"item0", "item1", "item2", "item3"}));
// }

// TEST_F(KeyValueStoreTest, SetAddRemCard) {
//     store->sAdd("test_set", "item1");
//     store->sAdd("test_set", "item2");
//     store->sAdd("test_set", "item3");
//     EXPECT_EQ(store->sRem("test_set", "item2"), 1);
//     EXPECT_EQ(store->sCard("test_set"), 2);
// }

// TEST_F(KeyValueStoreTest, SetAddRemMembers) {
//     EXPECT_EQ(store->sMembers("test_set"), (std::unordered_set<std::string>{}));
//     store->sAdd("test_set", "item1");
//     store->sAdd("test_set", "item2");
//     store->sAdd("test_set", "item3");
//     EXPECT_EQ(store->sRem("test_set", "item2"), 1);
//     EXPECT_EQ(store->sMembers("test_set"), (std::unordered_set<std::string>{"item1", "item3"}));
// }

// TEST_F(KeyValueStoreTest, SetAddIsMember) {
//     EXPECT_EQ(store->sIsMember("test_set", "item1"), 0);
//     store->sAdd("test_set", "item1");
//     EXPECT_EQ(store->sIsMember("test_set", "item1"), 1);
// }

// TEST_F(KeyValueStoreTest, StringExpireBasic) {
//     store->set("test_key1", "item1");
//     store->expire("test_key1", std::chrono::seconds(1));
//     EXPECT_TRUE(store->get("test_key1").has_value());
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//     EXPECT_FALSE(store->get("test_key1").has_value());
// }

// TEST_F(KeyValueStoreTest, ListExpireBasic) {
//     store->lPush("test_list1", "item1");
//     store->expire("test_list1", std::chrono::seconds(1));
//     EXPECT_TRUE(store->lRange("test_list1",0,0).has_value());
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//     EXPECT_FALSE(store->lRange("test_list1",0,0).has_value());
// }

// TEST_F(KeyValueStoreTest, StringExpirePersist) {
//     store->set("test_key1", "item1");
//     store->expire("test_key1", std::chrono::seconds(1));
//     store->persist("test_key1");
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//     EXPECT_TRUE(store->get("test_key1").has_value());
// }

// TEST_F(KeyValueStoreTest, StringExpireUpdate) {
//     store->set("test_key1", "item1");
//     store->expire("test_key1", std::chrono::seconds(2));
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//     store->set("test_key1", "item2");
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//     EXPECT_TRUE(store->get("test_key1").has_value());
// }

// TEST_F(KeyValueStoreTest, ListExpireUpdate) {
//     store->lPush("test_list1", "item1");
//     store->expire("test_list1", std::chrono::seconds(1));
//     store->rPush("test_list1", "item2");
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//     EXPECT_FALSE(store->lRange("test_list1",0,1).has_value());
// }

// TEST_F(KeyValueStoreTest, SetExpireUpdate) {
//     EXPECT_EQ(store->expire("test_set1", std::chrono::seconds(1)), 0);
//     store->sAdd("test_set1", "item1");
//     store->sAdd("test_set2", "item2");
//     store->expire("test_set1", std::chrono::seconds(1));
//     store->expire("test_set2", std::chrono::seconds(1));
//     store->sRem("test_set1", "item1");
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//     EXPECT_EQ(store->sCard("test_set1"), 0);
//     EXPECT_EQ(store->sCard("test_set2"), 0);
// }

// TEST_F(KeyValueStoreTest, SetCapacityLRU) {
//     store->lPush("test_list1", "item1");
//     store->set("test_key1", "item1");
//     store->set("test_key2", "item2");
//     store->sAdd("test_set1", "item1");
//     EXPECT_TRUE(store->lRange("test_list1", 0, 0).has_value());
//     EXPECT_TRUE(store->get("test_key1").has_value());
//     EXPECT_TRUE(store->get("test_key2").has_value());
//     EXPECT_EQ(store->sCard("test_set1"), 1);

//     store->setCapacity(3);

//     EXPECT_FALSE(store->lRange("test_list1", 0, 0).has_value());
//     EXPECT_TRUE(store->get("test_key1").has_value());
//     EXPECT_TRUE(store->get("test_key2").has_value());
//     EXPECT_EQ(store->sCard("test_set1"), 1);
// }

// TEST_F(KeyValueStoreTest, SetCapacityChangeEviction) {
//     EXPECT_EQ(store->useLRU(), 0);
//     EXPECT_EQ(store->useLFU(), 1);
//     store->setCapacity(3);
//     store->lPush("test_list1", "item1");
//     store->set("test_key1", "item1");
//     store->lLen("test_list1");
//     store->get("test_key1");
//     store->set("test_key2", "item2");
//     store->sAdd("test_set1", "item1");
//     EXPECT_FALSE(store->get("test_key2").has_value());
// }
