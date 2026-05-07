#include <sqlite3.h>
#include <stdbool.h>

bool userLogin(char username[], char password[], sqlite3* db);
bool userRegister(char username[], char password[], sqlite3* db);
bool alterBal(char username[], char password[], int diff, sqlite3* db);
bool getBal(int* p_bal);
bool setupTables(sqlite3* db);
