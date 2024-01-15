#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

#ifndef POSIX_HEADER
#define POSIX_HEADER

typedef struct posix_header {                              
  char name[100];
  char mode[8];
  char uid[8];                  
  char gid[8];                   
  char size[12];                
  char mtime[12];
  char chksum[8];               
  char typeflag;                
  char linkname[100];           
  char magic[6];                
  char version[2];              
  char uname[32];               
  char gname[32];               
  char devmajor[8];             
  char devminor[8];             
  char prefix[155];             
  char add[12];          
} posix_header;

#endif

#ifndef LISTS
#define LISTS

typedef struct listnode 
{
    char* name;
    struct listnode*next;
} lists;

#endif
#ifndef LIST_TAR
#define LIST_TAR

typedef struct list {
    struct list* next;
    posix_header* name_tar_list;
} list_tar;

#endif

#ifndef LIST_TAR_CONTENT
#define LIST_TAR_CONTENT

typedef struct list_tar {
    struct list_tar* next;
    char* name;
    char* mtime;
} list_tar_content;

#endif

void add_list(lists** main_list, char* name);
void clean_list(lists* list);
void tar_list_add(list_tar** list, posix_header* f_info);
void clean_tar_list(list_tar* list);
void add_tar_content_list(list_tar_content** list, char* name, char* mtime);
void list_tar_content_clean(list_tar_content* list);
void tar_null(posix_header* list);

#endif
