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