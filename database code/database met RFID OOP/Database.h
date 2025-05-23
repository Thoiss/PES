#ifndef Database_H
#define Database_H

#include <mariadb/mysql.h>
#include <string>

class Database {
private:
MYSQL *conn;
std::string server;
std::string user;
std::string password;
std::string database;


public:
    Database(const std::string& server, const std::string& user, const std::string& password, const std::string& database);
    ~Database();
    bool init();
    bool schrijvenrfid(const std::string& card_uid, const std::string& device_id);
    bool bestaatRfid(const std::string& card_uid);
    bool verwijderRfid(const std::string& card_uid);
    bool lezenuser();

};
#endif