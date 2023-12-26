#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <stdlib.h>
#ifndef LISTS
#define LISTS
typedef struct listnode {
    char* name;
    struct listnode*next;
} lists;
#endif
void list_creator(lists** main_list, char* name);
void cleaner(lists* list);

#endif
