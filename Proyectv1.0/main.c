#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(EXIT_FAILURE);
    }

    if (mysql_real_connect(conn, "127.0.0.1", "root", "TU_PASSWORD", "biblioteca", 3306, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\nError: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    if (mysql_query(conn, "SELECT id, titulo, autor, disponible FROM libros")) {
        fprintf(stderr, "SELECT failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    res = mysql_store_result(conn);

    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    int num_fields = mysql_num_fields(res);

    while ((row = mysql_fetch_row(res))) {
        for(int i = 0; i < num_fields; i++) {
            printf("%s ", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }

    mysql_free_result(res);
    mysql_close(conn);
    exit(EXIT_SUCCESS);
}
