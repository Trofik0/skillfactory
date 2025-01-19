#include "database.hpp"

Database::Database(const std::string& db_name) {
    if (sqlite3_open(db_name.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    }

    
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT);", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS messages (username TEXT, message TEXT);", nullptr, nullptr, nullptr);
}

Database::~Database() {
    sqlite3_close(db);
}

bool Database::register_user(const std::string& username, const std::string& password) {
    std::string hashed_password = BCrypt::generateHash(password);
    std::string sql = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + hashed_password + "');";
    
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error registering user: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool Database::authenticate_user(const std::string& username, const std::string& password) {
    std::string sql = "SELECT password FROM users WHERE username = '" + username + "';";
    char* errMsg = nullptr;
    char** results = nullptr;
    int rows, columns;

    if (sqlite3_get_table(db, sql.c_str(), &results, &rows, &columns, &errMsg) == SQLITE_OK) {
        if (rows > 0) {
            std::string stored_password = results[1]; 
            sqlite3_free_table(results); 
            return BCrypt::validatePassword(password, stored_password);
        }
        sqlite3_free_table(results); // Освобождаем память при неверных данных
    } else {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    return false;
}

void Database::save_message(const std::string& username, const std::string& message) {
    std::string sql = "INSERT INTO messages (username, message) VALUES ('" + username + "', '" + message + "');";
    char* errMsg = nullptr;
    sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    
    if (errMsg) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}
