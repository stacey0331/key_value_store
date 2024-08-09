#ifndef LRU_H
#define LRU_H

#include <list>
#include "eviction_policy.h"

class LRU : public EvictionPolicy {
    public:
        void keyAccessed(const std::string& key) override {
            auto it = std::find(keys.begin(), keys.end(), key);
            if (it != keys.end()) {
                keys.erase(it);
            }
            keys.push_front(key);
        }

        void keyRemoved(const std::string& key) override {
            auto it = std::find(keys.begin(), keys.end(), key);
            if (it != keys.end()) {
                keys.erase(it);
            }
        }

        std::string evict() override {
            auto ret = std::move(keys.back());
            keys.pop_back();
            return ret;
        }

    private: 
        std::list<std::string> keys;
};

#endif