#include "Database.h"
#include <iostream>
#include <cstdio>

Database::Database(const std::string& server, const std::string& user,
                   const std::string& password, const std::string& database)
    : server(server), user(user), password(password), database(database), conn(nullptr) {}

Database::~Database() {
    if (conn) {
        mysql_close(conn);
    }
}

bool Database::init() {
    conn = mysql_init(NULL);
    if (!conn) {
        std::cerr << "mysql_init() failed\n";
        return false;
    }

    if (!mysql_real_connect(conn, server.c_str(), user.c_str(), password.c_str(),
                            database.c_str(), 0, NULL, 0)) {
        std::cerr << "mysql_real_connect() failed: " << mysql_error(conn) << std::endl;
        return false;
    }

    return true;
}

bool Database::schrijven(const std::string& card_uid, const std::string& device_id) {
    if (!conn) {
        std::cerr << "Database not initialized.\n";
        return false;
    }

    char query[512];
    snprintf(query, sizeof(query),
             "INSERT INTO rfid_logs (card_uid, device_id) VALUES('%s', '%s')",
             card_uid.c_str(), device_id.c_str());

    if (mysql_query(conn, query)) {
        std::cerr << "INSERT failed: " << mysql_error(conn) << std::endl;
        return false;
    }

    std::cout << "RFID-gegevens succesvol toegevoegd!" << std::endl;
    return true;
}