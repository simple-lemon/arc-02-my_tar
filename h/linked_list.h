#include "../h/linked_list.h"
#include "../h/my_string.h"

void tar_null(posix_header* list) {
    null(list->name, 100);
    null(list->mode, 8);
    null(list->uid, 8);
    null(list->gid, 8);
    null(list->size, 12);
    null(list->mtime, 12);
    null(list->chksum, 8);
    null(list->linkname, 100);
    null(list->magic, 6);
    null(list->version, 2);
    null(list->uname, 32);
    null(list->gname, 32);
    null(list->devmajor, 8);
    null(list->devminor, 8);
    null(list->prefix, 155);
    null(list->add, 12);
}

void add_list(lists** main_list, char* name) 
{
    lists* new = (lists*)malloc(sizeof(lists));
    new->next = NULL;
    new->name = name;
    if(!(*main_list)) 
    {
        *main_list = new;
    }
    else 
    {
        lists* head = *main_list;
        for( ; head->next; head = head->next) {}
        head->next = new;
    }
}

void clean_list(lists* list) 
{
    lists* temp = NULL;
    while(list) 
    {
        temp = list;
        list = list->next;
        temp->name = NULL;
        free(temp);
    }
}

void tar_list_add(list_tar** list, posix_header* f_info) 
{
    list_tar* new = (list_tar*)malloc(sizeof(list_tar));
    new->name_tar_list = f_info;
    new->next = NULL;
    if(!(*list)) 
    {
        *list = new;
    }
    else 
    {
        list_tar* head = *list;
        for( ; head->next; head = head->next) {}
        head->next = new;
    }
}

void clean_tar_list(list_tar* list) 
{
    list_tar* temp;
    while(list) 
    {
        tar_null(list->name_tar_list);
        free(list->name_tar_list);
        temp = list;
        list = list->next;
        free(temp);
    }
}

void add_tar_content_list(list_tar_content** list, char* name, char* mtime) 
{
    list_tar_content* new = (list_tar_content*)malloc(sizeof(list_tar_content));
    new->name = (char*)calloc(sizeof(char), (my_strlen(name) + 1));
    new->name = my_strcpy(new->name, name);
    new->mtime = (char*)calloc(sizeof(char), (my_strlen(mtime) + 1));
    new->mtime = my_strcpy(new->mtime, mtime);
    new->next = NULL;
    if(*list == NULL) 
    {
        *list = new;
    }
    else 
    {
        list_tar_content* head = *list;
        for( ; head->next; head = head->next) {}
        head->next = new;
    }
}

void list_tar_content_clean(list_tar_content* list) 
{
    list_tar_content* temp;
    while(list) 
    {
        temp = list;
        list = list->next;
        free(temp->name);
        free(temp->mtime);
        free(temp);
    }
}
