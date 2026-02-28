//birdflu.h
#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<assert.h>
#include<sys/syscall.h>
#include<dirent.h>
#include <fcntl.h> // Required for open(), O_RDONLY, O_DIRECTORY

//#include<birchutils.h>
//#include "birchutils.h"


typedef unsigned char int8;
typedef unsigned short  int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;

#define Blocksize 50000
//#define onedot(x) ((*(x)==".") && !(*(x+1)))
//#define twodots(x) ((*(x)==".") && (*(x+1)==".") && !(*(x+2)))


#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32)
#define $8 (int64)
#define $c (char *)
#define $i (int)

#define onedot(x) (x[0] == '.' && x[1] == '\0')
#define twodots(x) (x[0] == '.' && x[1] == '.' && x[2] == '\0')


typedef int8 Dir[64];
typedef int8 File[32];

enum e_filetype{
    file =1,
    dir =2,
    other =3
};
typedef enum e_filetype Filetype; 

struct s_entry{
    Filetype type;
    Dir dir;
    File file;
};
typedef struct s_entry Entry;

struct s_database{
    Entry *entries;
    int32 cap;
    int32 num;
};
typedef struct s_database Database;

// struct linux_dirent{
//     unsigned long d_ino;
//     off_t         d_off;
//     unsigned short d_reclen;
//     char          d_name[];
// };

typedef bool (*function)(Entry);
#define  linux_dirent dirent

Database* filter(Database*,function);
Database* mkdatabase(void);
bool adddir(Database*,int8*);
bool iself(Entry);
void addtodb(Database*,Entry);
void destroydb(Database*);
void showdb(Database*);

int main(int,char**);