#ifndef _DB_MANAGER_LIB_H_
#define _DB_MANAGER_LIB_H_

#define STR_LEN 64

//The object type of a single database element
typedef struct obj_s {
  //All fields
  int id;
  char name[STR_LEN + 1];
  char surname[STR_LEN + 1];

  struct obj_s* next;
} obj;

//The database object
typedef struct database_s {
  char filename[STR_LEN + 1];
  int n;

  obj* h;
} database;

//Print all the database
extern void printdb(database* db);
//Append a given object at the end of the database
extern void append(database* db, obj* new);
//Pushes the database and insert a given element as the first
extern void push(database* db, obj* new);
//Fill the database with n elements asked to the user
extern int append_n(database* db, int n, int auto_id);
//Fill the database till the user instert the stopword
extern int append_till(database* db, char stopword[], int auto_id);
//Create a blank database with all its variables initialized
extern database db_new();
//Open a database loaded from a given file
extern database db_open(char path[]);
//Saves the database to its file specified in db.filename;
extern int db_save(database* db);

//void increasing(database* db);

#endif
