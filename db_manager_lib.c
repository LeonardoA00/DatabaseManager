#include "db_manager_lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printdb(database* db)
{
  obj* p;
  printf("-----------------\n");
  printf(" %s\n", db->filename);
  printf(" %d records\n", db->n);
  printf("-----------------\n");

  for(p = db->h; p != NULL; p = p->next)
  {
    printf("ID: [%d]\n", p->id);
    printf("Name: %s\n", p->name);
    printf("Surname: %s\n", p->surname);
  }
}

void append(database* db, obj* new)
{
  obj* p;

  if(db->h)
  {
    for(p = db->h; p->next != NULL; p = p->next)
      ;
    p->next = new;
    new->next = NULL;
  }
  else
  {
    db->h = new;
    new->next = NULL;
  }
  db->n = db->n + 1;

}

void push(database* db, obj* new)
{
  new->next = db->h;
  db->h = new;
  db->n = db->n + 1;
}

int append_n(database* db, int n, int auto_id)
{
  int i;
  obj* new;

  load_ids(db);

  for(i = 0; i < n; i++)
  {
    new = malloc(sizeof(obj));
    if(new)
    {
      if(auto_id)
        new->id = available_id(db);
      else
      {
        do {
          printf("ID: ");
          scanf("%d", &(new->id));
        } while(!valid_id(db, new->id));
      }

      //Modify here the attributes
      printf("Name: ");
      scanf("%s", new->name);
      printf("Surname: ");
      scanf("%s", new->surname);

      append(db, new);
    }
    else
      printf("append_n: error allocating %d element\n", i);
  }

  return i;
}

int append_till(database* db, char stopword[], int auto_id)
{
  int i;
  obj* new;
  char tmpstr[STR_LEN + 1];

  load_ids(db);

  i = 0;
  while(1)
  {
    if(!auto_id)
      printf("ID: ");
    else
      printf("Name: ");
    scanf("%s", tmpstr);
    if(strcmp(tmpstr, stopword) == 0)
      return i;

    new = malloc(sizeof(obj));

    if(new)
    {
      if(auto_id)
        new->id = available_id(db);
      else
      {
        new->id = atoi(tmpstr);
        while(!valid_id(db, new->id))
        {
          printf("ID: ");
          scanf("%d", &(new->id));
        }
      }


      //Modify here the attributes
      if(auto_id)
        strcpy(new->name, tmpstr);
      else
      {
        printf("Name: ");
        scanf("%s", new->name);
      }
      printf("Surname: ");
      scanf("%s", new->surname);

      append(db, new);
    }
    else
    {
      printf("append_n: error allocating %d element\n", i);
      break;
    }


    i++;
  }

  return i;
}


/*Database file management*/
database db_new()
{
  database db;
  *(db.filename) = '\0';
  db.n = 0;
  db.h = NULL;

  return db;
}

/*file operarions*/
database db_open(char path[])
{
  database db;
  FILE* fp;
  obj* new;

  db.h = NULL;
  db.n = 0;
  strcpy(db.filename, path);

  fp = fopen(path, "rb");
  if(fp)
  {
    new = malloc(sizeof(obj));
    fread(new, sizeof(obj), 1, fp);
    while(!feof(fp) && new)
    {
      append(&db, new);
      new = malloc(sizeof(obj));
      fread(new, sizeof(obj), 1, fp);
    }
    if(!new)
      printf("db_open: error allocating memory for a database element\n");
    fclose(fp);
  }
  else
    printf("db_open: error opening %s\n", path);

  return db;
}

int db_save(database* db)
{
  FILE* fp;
  obj* p;

  fp = fopen(db->filename, "wb");
  if(fp)
  {
    for(p = db->h; p != NULL; p = p->next)
      fwrite(p, sizeof(obj), 1, fp);

    fclose(fp);
  }
  else
  {
    printf("db_save: error opening the file\n");
    return -1;
  }

  return 0;
}

//IDS MANAGEMENT
int* taken_ids;

void load_ids(database* db)
{
  obj* p;
  int i;

  taken_ids = malloc(db->n * sizeof(int));

  if(taken_ids)
    for(p = db->h, i = 0; p != NULL; p = p->next, i++)
      *(taken_ids + i) = p->id;
  else
    printf("load_ids: error allocating memory\n");

}

int available_id(database* db)
{
  int* taken_ids_new;
  int id;
  int i;
  int found;

  found = 1;
  for(id = 0; found == 1; id++)
  {
    found = 0;
    for(i = 0; i < db->n && found == 0; i++)
      if(id == *(taken_ids + i))
        found = 1;
  }
  id--;

  taken_ids_new = malloc((db->n + 1) * sizeof(int));
  if(taken_ids_new)
  {
    for(i = 0; i < db->n; i++)
      *(taken_ids_new + i) = *(taken_ids + i);
    *(taken_ids_new + i) = id;

    free(taken_ids);
    taken_ids = taken_ids_new;

  }
  else
    printf("available_id: error allocating memory\n");

  return id;
}

int valid_id(database* db, int id)
{
  int valid;
  int i;

  load_ids(db);

  valid = 1;
  for(i = 0; i < db->n && valid; i++)
    if(id == *(taken_ids + i))
      valid = 0;

  return valid;
}
