#ifndef EVICTION_POLICY_H
#define EVICTION_POLICY_H

#include <string>
#include <optional>

class EvictionPolicy {
    public:
        virtual void keyAccessed(const std::string& key) = 0;
        virtual void keyRemoved(const std::string& key) = 0;
        virtual std::string evict() = 0;
        virtual ~EvictionPolicy() = default;
};      

#endif