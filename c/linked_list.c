#include "../h/linked_list.h"
void list_creator(lists** main_list, char* name) {
    lists* new = (lists*)malloc(sizeof(lists));
    new->next = NULL;
    new->name = name;
    if(!(*main_list)) {
        *main_list = new;
    }
    else {
        lists* head = *main_list;
        for( ; head->next; head = head->next) {}
        head->next = new;
    }
}

void cleaner(lists* list) {
    lists* temp = NULL;
    while(list) {
        temp = list;
        list = list->next;
        temp->name = NULL;
        free(temp);
    }
}
