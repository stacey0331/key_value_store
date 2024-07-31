#include "key_value_store.h"

KeyValueStore::KeyValueStore() 
    : store(), capacity(0) {}

void KeyValueStore::set(const std::string& key, const std::string& val) {
    store.insert_or_assign(key, Value(val));
}

std::optional<std::string> KeyValueStore::get(const std::string& key) {
    auto it = store.find(key);
    if (it == store.end()) {
        return std::nullopt;
    }
    return it->second.getString();
}
