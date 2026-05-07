#include "gamedb.h"
#include <sqlite3.h>
#include <stdio.h>

#define BUFSIZE 256

// what was defined in past code:
// DB_CONN_FAIL
// NO_USER_FOUND
// USER_FOUND
// DB_ERROR
// REG_DONE
// UPDATE_DONE
// OBTAIN_DONE

int login_callback(void *data, int columns, char **row_fields,
                   char **row_names);
int getBal_callback(char *p_var, int columns, char **row_fields,
                    char **row_names);

char sql_query[BUFSIZE] = "";

bool setupTables(sqlite3 *db) {

  int sql_return;
  char str_userTable[] =
      "LoginTable(Username TEXT PRIMARY KEY, Password TEXT NOT NULL);";
  char str_balTable[] =
      "BalTable(Username TEXT, Bal INTEGER NOT NULL, FOREIGN KEY(Username) "
      "REFERENCES LoginTable(Username) ON DELETE CASCADE ON UPDATE CASCADE);";

  sprintf(sql_query, "CREATE TABLE IF NOT EXISTS %s", str_userTable);
  sql_return = sqlite3_exec(db, sql_query, 0, 0, 0);
  if (sql_return != SQLITE_OK) {
    perror("error in sqlite3_exec()");
    return false;
  }

  sprintf(sql_query, "CREATE TABLE IF NOT EXISTS %s", str_balTable);
  sql_return = sqlite3_exec(db, sql_query, 0, 0, 0);
  if (sql_return != SQLITE_OK) {
    perror("error in sqlite3_exec()");
    return false;
  }

  return true;
}

bool userLogin(char username[], char password[], sqlite3 *db) {

  int sql_return;

  sprintf(sql_query,
          "SELECT * FROM LoginTable WHERE Username = '%s' AND Password = '%s';",
          username, password);
  sql_return = sqlite3_exec(db, sql_query, login_callback, 0, 0);

  if (sql_return == SQLITE_OK) {
    return true;
  } else {
    return false;
  }
}

int login_callback(void *data, int columns, char **row_fields,
                   char **row_names) {
  if (columns > 0)
    return 0;
  else
    return 1;
}

bool userRegister(char username[], char password[], sqlite3 *db) {

  int sql_return;

  sprintf(sql_query, "INSERT INTO LoginTable VALUES('%s','%s');", username,
          password);
  sql_return = sqlite3_exec(db, sql_query, 0, 0, 0);

  if (sql_return == SQLITE_OK) {

    sprintf(sql_query, "INSERT INTO BalTable VALUES('%s',1000);", username);
    sql_return = sqlite3_exec(db, sql_query, 0, 0, 0);

    if (sql_return == SQLITE_OK)
      return true;
  }

  return false;
}
