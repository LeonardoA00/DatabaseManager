#include "db_manager_lib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ARGS 16

int function_call(char str[], database* db);
void substring(char str[], int* subc, char* subv[]);
void help();

int main(int argc, char* argv[])
{
  database db = db_new();
  int exit;
  int error;
  char str[STR_LEN + 1];

  exit = 0;
  error = 0;

  //Argument call
  if(argc > 1)
    db = db_open(argv[1]);

  do {
    printf(">> ");
    gets(str);

    exit = function_call(str, &db);

  } while(!exit);


  return 0;
}

void substring(char str[], int* subc, char* subv[])
{
  int i, pos;

  subv[0] = malloc(STR_LEN * sizeof(char));

  for(i = 0, pos = 0, *subc = 0; str[i] != '\0'; i++)
  {
    *(subv[*subc] + (i - pos)) = str[i];
    if(str[i] == ' ')
    {
      *(subv[*subc] + (i - pos)) = '\0';
      *subc = *subc + 1;
      subv[*subc] = malloc(STR_LEN * sizeof(char));
      pos = i + 1;
    }
  }
  *(subv[*subc] + (i - pos)) = '\0';
  *subc = *subc + 1;
}

void help()
{
  printf("------------------\n");
  printf(" DB_MANAGER\n");
  printf("          Made by\n");
  printf(" Leonardo Airoldi");
  printf("------------------\n\n");

  printf("LIST OF COMMANDS\n");
  printf("command_name [argument] [argument] -[optional_argument]\nFor true/false args use 1/0\n\n");
  printf("help\n...Show this help\n");
  printf("printdb\n...Print the open databse\n");
  printf("append_n [n] -[auto_id]\n...Append the database with n elements. True/False for auto_id\n");
  printf("append_till -[stopword] -[auto_id]\n...Append the database until the stopword is inserted. The default stopword is 'exit'. True/False for auto_id\n");
  printf("new\n...Create a new database\n");
  printf("path [path]\n...Change the database path/filename\n");
  printf("open [path]\n...Opens the database located at [path]\n");
  printf("save\n...Saves the databse at his path\n");

  printf("\nYou can open a database at program launch by launching the program passing a [path] argument\n");
}

int function_call(char str[], database* db)
{
  char *cmdv[MAX_ARGS];
  int cmdc;
  int exit;
  int error;

  exit = 0;
  substring(str, &cmdc, cmdv);
  //Funciton calls
  if(strcmp(cmdv[0], "help") == 0)
    help();
  else if(strcmp(cmdv[0], "printdb") == 0)
    printdb(db);
  else if(strcmp(cmdv[0], "append_n") == 0)
  {
    if(cmdc == 2)
      append_n(db, atoi(cmdv[1]), 1);
    else if(cmdc == 3)
      append_n(db, atoi(cmdv[1]), atoi(cmdv[2]));
  }
  else if(strcmp(cmdv[0], "append_till") == 0)
  {
    if(cmdc == 1)
      append_till(db, "exit", 1);
    else if(cmdc == 2)
      append_till(db, cmdv[1], 1);
    else if(cmdc == 3)
      append_till(db, cmdv[1], atoi(cmdv[2]));
  }
  else if(strcmp(cmdv[0], "new") == 0)
    *db = db_new();
  else if(strcmp(cmdv[0], "open") == 0)
    *db = db_open(cmdv[1]);
  else if(strcmp(cmdv[0], "path") == 0)
    strcpy((*db).filename, cmdv[1]);
  else if(strcmp(cmdv[0], "save") == 0)
  {
    error = db_save(db);
    if(!error)
      printf("Database saved successfuly to %s\n", (*db).filename);
    else
      printf("Error saving the database to %s\n", (*db).filename);
  }

  else if(strcmp(cmdv[0], "exit") == 0 || strcmp(cmdv[0], "quit") == 0 || strcmp(cmdv[0], "x") == 0)
    exit = 1;

    return exit;
}
