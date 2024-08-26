#include "database_manager.h"

DatabaseManager::DatabaseManager(const std::string& connStr) : connectionString(connStr), conn(nullptr) {}

DatabaseManager::~DatabaseManager() {
    if (conn) {
        disconnect();
    }
}

void DatabaseManager::connect() {
    if (!conn) {
        conn = new pqxx::connection(connectionString);
    }
}

void DatabaseManager::disconnect() {
    if (conn) {
        delete conn;
        conn = nullptr;
    }
}

void DatabaseManager::deleteKey(const size_t storeId, const std::string& key) {
    deleteString(storeId, key);
    // deleteList(storeId, key);
    // deleteSet(storeId, key);
}

// bool DatabaseManager::exceedsCapacity(const size_t storeId) {
//     pqxx::work txn(*conn);
//     pqxx::result capacity_res = txn.exec_params(
//         "SELECT capacity FROM eviction WHERE store_id = $1", storeId
//     );

//     if (capacity_res.empty()) {
//         throw std::runtime_error("Store ID not found in eviction table.");
//     }

//     int capacity = capacity_res[0][0].as<int>();

//     pqxx::result strings_count_res = txn.exec_params(
//         "SELECT COUNT(*) FROM strings WHERE store_id = $1", storeId
//     );
//     pqxx::result sets_count_res = txn.exec_params(
//         "SELECT COUNT(*) FROM sets WHERE store_id = $1", storeId
//     );
//     pqxx::result lists_count_res = txn.exec_params(
//         "SELECT COUNT(*) FROM lists WHERE store_id = $1", storeId
//     );
    
//     int total_count = strings_count_res[0][0].as<int>() +
//                       sets_count_res[0][0].as<int>() +
//                       lists_count_res[0][0].as<int>();
    
//     return total_count > capacity;
// }

// std::string DatabaseManager::evict(const size_t storeId) {
//     pqxx::work txn(*conn);
//     pqxx::result res = txn.exec_params(
//         "SELECT cache FROM " + EVICTION_TABLE + " WHERE store_id = $1", storeId
//     );

//     if (res.empty()) {
//         throw std::runtime_error("Store ID not found.");
//     }

//     std::string cache_json = res[0][0].as<std::string>();
//     nlohmann::json cache = nlohmann::json::parse(cache_json);

//     if (cache.empty()) {
//         throw std::runtime_error("Cache is empty, nothing to evict.");
//     }

//     std::string keyEvicted = cache.back();
//     cache.pop_back();
//     std::string updated_cache_json = cache.dump();
//     txn.exec_params(
//         "UPDATE cache_data SET cache = $1 WHERE store_id = $2",
//         updated_cache_json, storeId
//     );
//     txn.commit();
//     deleteKey(storeId, keyEvicted);
//     return keyEvicted;
// }

size_t DatabaseManager::setExpiration(const size_t storeId, const std::string& key, const std::chrono::seconds& sec) {
    pqxx::work txn(*conn);
    
    auto expiration_time = std::chrono::steady_clock::now() + sec;
    auto expiration_seconds = std::chrono::duration_cast<std::chrono::seconds>(expiration_time.time_since_epoch()).count();
    
    std::string sql = "UPDATE strings SET expiration = $1 WHERE store_id = $2 AND key = $3";
    pqxx::result res = txn.exec_params(sql, expiration_seconds, storeId, key);
    txn.commit();
    
    if (res.affected_rows() == 0) {
        return 0;
    }
    return 1;
}

std::optional<std::chrono::steady_clock::time_point> DatabaseManager::getExpiration(const size_t storeId, const std::string& key, const std::string& type) {
    pqxx::work txn(*conn);

    std::string sql = "SELECT expiration FROM strings WHERE store_id = $1 AND key = $2";
    pqxx::result res = txn.exec_params(sql, storeId, key);

    if (res.empty() || res[0][0].is_null()) {
        return std::nullopt;
    }

    auto expiration_seconds = res[0][0].as<std::int64_t>();
    auto expiration_duration = std::chrono::seconds(expiration_seconds);
    auto expiration_time = std::chrono::steady_clock::time_point(expiration_duration);

    return expiration_time;
}

// will clear expiration on update
void DatabaseManager::insertString(const size_t storeId, const std::string& key, const std::string& value) {
    pqxx::work txn(*conn);
    txn.exec0("INSERT INTO strings (store_id, key, value) VALUES (" 
                + std::to_string(storeId) + ", "
                + txn.quote(key) + ", "
                + txn.quote(value) + ") "
                "ON CONFLICT (store_id, key) DO UPDATE "
                "SET value = excluded.value, "
                "expiration = NULL;");
    txn.commit();
}

void DatabaseManager::deleteString(const size_t storeId, const std::string& key) {
    pqxx::work txn(*conn);
    std::string sql_string = "DELETE FROM " + std::string(STRING_TABLE) + " WHERE store_id = $1 AND key = $2;";
    // TODO: delete from eviction

    txn.exec_params(sql_string, storeId, key);
    txn.commit();
}

std::optional<std::string> DatabaseManager::fetchString(const size_t storeId, const std::string& key) {
    pqxx::work txn(*conn);
    pqxx::result res = txn.exec(
        "SELECT value FROM " + std::string(STRING_TABLE) + 
        " WHERE store_id = " + std::to_string(storeId) + 
        " AND key = " + txn.quote(key)
    );
    txn.commit();
    if (!res.empty()) {
        return res[0][0].c_str();
    } else {
        return std::nullopt;
    }
}
