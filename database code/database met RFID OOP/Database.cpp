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

bool Database::schrijvenrfid(const std::string& card_uid, const std::string& naam) {
    
    if (!conn) {
        std::cerr << "Database not initialized.\n";
        return false;
    }

    char query[512];
    snprintf(query, sizeof(query),
             "INSERT INTO rfid_logs (card_uid, naam) VALUES('%s', '%s')",
             card_uid.c_str(), naam.c_str());

    if (mysql_query(conn, query)) {
        std::cerr << "INSERT failed: " << mysql_error(conn) << std::endl;
        return false;
    }

    std::cout << "RFID-gegevens succesvol toegevoegd!" << std::endl;
    return true;
}

bool Database::bestaatRfid(const std::string& card_uid) {
    if (!conn) {
        std::cerr << "Database niet geïnitialiseerd.\n";
        return false;
    }

    char query[256];
    snprintf(query, sizeof(query),
             "SELECT 1 FROM rfid_logs WHERE card_uid = '%s' LIMIT 1",
             card_uid.c_str());

    if (mysql_query(conn, query)) {
        std::cerr << "SELECT query mislukt: " << mysql_error(conn) << std::endl;
        return false;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        std::cerr << "Resultaat ophalen mislukt: " << mysql_error(conn) << std::endl;
        return false;
    }

    bool exists = mysql_num_rows(result) > 0;
    mysql_free_result(result);
    
    return exists;
}

bool Database::verwijderRfid(const std::string& card_uid) {
    if (!conn) {
        std::cerr << "Database niet geïnitialiseerd.\n";
        return false;
    }

    char query[256];
    snprintf(query, sizeof(query),
             "DELETE FROM rfid_logs WHERE card_uid = '%s'",
             card_uid.c_str());

    if (mysql_query(conn, query)) {
        std::cerr << "DELETE query mislukt: " << mysql_error(conn) << std::endl;
        return false;
    }

    if (mysql_affected_rows(conn) == 0) {
        std::cout << "Geen rijen verwijderd. UID bestaat mogelijk niet.\n";
        return false;
    }

    std::cout << "UID succesvol verwijderd.\n";
    return true;
}
int Database::tellerUniekePersonen() {
    if (!conn) {
        std::cerr << "Database niet geïnitialiseerd.\n";
        return -1;
    }

    const char* query = "SELECT COUNT(DISTINCT card_uid) FROM rfid_logs";

    if (mysql_query(conn, query)) {
        std::cerr << "SELECT query mislukt: " << mysql_error(conn) << std::endl;
        return -1;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res) {
        std::cerr << "Resultaten ophalen mislukt: " << mysql_error(conn) << std::endl;
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    int uniekePersonen = -1;
    if (row && row[0]) {
        uniekePersonen = std::atoi(row[0]);
    } else {
        std::cerr << "Kon geen resultaten lezen.\n";
    }

    mysql_free_result(res);
    return uniekePersonen;
}