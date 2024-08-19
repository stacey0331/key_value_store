/**
 * @file lfu.h
 * @brief Define and implement a Least Frequently Used (LFU) cache.
 * 
 * The cache evicts the least frequently used items when the capacity is exceeded.
 * Note that when the cache is at its capacity and all items have access count more than 1, no new items could be added. 
 * The above note is implemented in key_value_store.cpp where the item is added before eviction. 
 * When multiple items have the same minimum count, the least recently used item is evicted. 
 * 
 */

#ifndef LFU_H
#define LFU_H

#include <unordered_map>
#include <list>
#include "eviction_policy.h"

class LFU : public EvictionPolicy {
    public: 
        void keyAccessed(const std::string& key) override {
            ++freq[key];

            auto it = std::find(keys.begin(), keys.end(), key);
            if (it != keys.end()) {
                keys.erase(it);
            }
            keys.push_front(key);
        }

        void keyRemoved(const std::string& key) override {
            freq.erase(key);
            
            auto it = std::find(keys.begin(), keys.end(), key);
            if (it != keys.end()) {
                keys.erase(it);
            }
        }

        std::string evict() override {
            auto minItRev = keys.rbegin();
            size_t minAccess = freq[*keys.rbegin()];

            for(auto it = keys.rbegin(); it != keys.rend(); ++it) {
                if (freq[*it] < minAccess) {
                    minAccess = freq[*it];
                    minItRev = it;
                }
            }
            auto minIt = minItRev.base();
            std::string ret = *(--minIt);

            freq.erase(ret);
            keys.erase(minIt);
            return ret;
        }

    private:
        std::unordered_map<std::string, size_t> freq;
        std::list<std::string> keys; 
};

#endif