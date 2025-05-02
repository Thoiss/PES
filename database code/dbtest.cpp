//gcc dbtest.cpp -o dbtest -lmariadb

#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    const char *server = "localhost";
    const char *user = "julian";
    const char *password = "Root";
    const char *database = "rfid_db";

    const char *card_uid = "HAHAHAHAHAHA";
    const char *device_id = "raspberry1";

    char query[512];

    // Initialize verbinding
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return EXIT_FAILURE;
    }

    // Verbinden met database
    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    // SQL-query voorbereiden
    snprintf(query, sizeof(query),
             "INSERT INTO rfid_logs (card_uid, device_id) VALUES('%s', '%s')",
             card_uid, device_id);

    // Query uitvoeren
    if (mysql_query(conn, query)) {
        fprintf(stderr, "INSERT failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    printf("RFID-gegevens succesvol toegevoegd!\n");

    // Verbinding sluiten
    mysql_close(conn);
    return EXIT_SUCCESS;
}