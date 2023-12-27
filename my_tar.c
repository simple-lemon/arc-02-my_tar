#include "./h/libarys.h"
#include "./h/linked_list.h"
#include "./h/my_string.h" 
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
void null(char* str, int num) { 
    for(short i = 0; i < num; i++) {
        str[i] = '\0';
    }
}

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

void fill_tar_file(posix_header* f_data, char* f_name) {
    tar_null(f_data);
    struct stat f_stat;
    stat(f_name, &f_stat);
    short chsum = 256;
    char* str;
    my_strcpy(f_data->name, f_name);
    chsum += ascii(f_name);
    my_strcpy(f_data->mode, (str = decimal_change_octal(f_stat.st_mode, 7)));
    free(str);
    chsum += ascii(f_data->mode);
    my_strcpy(f_data->size, (str = decimal_change_octal(f_stat.st_size, 11)));
    free(str); 
    chsum += ascii(f_data->size);
    my_strcpy(f_data->uid, (str = decimal_change_octal(f_stat.st_uid, 7)));
    free(str);
    chsum += ascii(f_data->uid);
    my_strcpy(f_data->gid, (str = decimal_change_octal(f_stat.st_gid, 7)));
    free(str);
    chsum += ascii(f_data->gid);
    my_strcpy(f_data->mtime, (str = decimal_change_octal(f_stat.st_mtime, 11)));
    free(str);
    chsum += ascii(f_data->mtime);
    f_data->typeflag = '0';
    chsum += 48;
    my_strcpy(f_data->uname, "docode");
    chsum += (2*ascii("docode"));
    my_strcpy(f_data->gname, "docode");
    chsum += ascii("ustar  ");
    my_strcpy(f_data->magic, "ustar  ");
    my_strcpy(f_data->chksum, (str = decimal_change_octal(chsum, 7)));
    free(str);
}

void tar_list_creator(list_tar** list, posix_header* f_info) {
    list_tar* new = (list_tar*)malloc(sizeof(list_tar));
    new->name_tar_list = f_info;
    new->next = NULL;
    if(!(*list)) {
        *list = new;
    }
    else {
        list_tar* head = *list;
        for( ; head->next; head = head->next) {}
        head->next = new;
    }
}

void cleaner_tar_list(list_tar* list) {
    list_tar* temp;
    while(list) {
        tar_null(list->name_tar_list);
        free(list->name_tar_list);
        temp = list;
        list = list->next;
        free(temp);
    }
}

void list_tar_content_creator(list_tar_content** list, char* name, char* mtime) {
    list_tar_content* new = (list_tar_content*)malloc(sizeof(list_tar_content));
    new->name = (char*)calloc(sizeof(char), (my_strlen(name) + 1));
    new->name = my_strcpy(new->name, name);
    new->mtime = (char*)calloc(sizeof(char), (my_strlen(mtime) + 1));
    new->mtime = my_strcpy(new->mtime, mtime);
    new->next = NULL;
    if(*list == NULL) {
        *list = new;
    }
    else {
        list_tar_content* head = *list;
        for( ; head->next; head = head->next) {}
        head->next = new;
    }
}

void list_tar_content_cleaner(list_tar_content* list) {
    list_tar_content* temp;
    while(list) {
        temp = list;
        list = list->next;
        free(temp->name);
        free(temp->mtime);
        free(temp);
    }
}

i8 xf_correct(list_tar_content* list, posix_header* f_data) {
    list_tar_content* temp = list;
    for( ; temp; temp = temp->next) {
        if(my_strcmp(temp->name, f_data->name) == 0 && my_strcmp(temp->mtime, f_data->mtime) < 0) {
            temp->mtime = my_strcpy(temp->mtime, f_data->mtime);
            return 1;
        }
        if(my_strcmp(temp->name, f_data->name) == 0 && my_strcmp(temp->mtime, f_data->mtime) == 0) {
            return 1;
        }
    }
    return 0;
}

i8 check(list_tar_content* list, posix_header* f_data) {
    list_tar_content* temp = list;
    int fd;
    for( ; temp; temp = temp->next) {
        if(my_strcmp(f_data->name, temp->name) == 0 && my_strcmp(f_data->mtime, temp->mtime) == 0) {
            fd = open(temp->name,  O_RDONLY);
            if(fd == -1) {
                return 1;
            } 
            else {
                close(fd);
            }
        }
    }
    return 0;
}

char* error_check(list_tar_content* t_list, list_tar_content* f_list, char* name) {
    char* result = NULL;
    list_tar_content* temp = t_list;
    for( ; temp; temp = temp->next) {
        if(my_strcmp(name, temp->name) == 0) {
            if(my_isdigit(temp->mtime) == 0) {
                result = temp->mtime;
            }
            break;
        }
    }
    for(temp = f_list; temp; temp = temp->next) {
        if(my_strcmp(name, temp->name) == 0) {
            return NULL;
        }
    }
    return result;
}

i8 null_check(char* str, short num) {
    for(short i = 0; i < num; i++) {
        if(str[i] == '\0') {
            return 1;
        }
    }
    return 0;
}

i8 cf(int argc, char** argv) {
    if(argc == 2) {
        printf("my_tar: option requires an argument -- 'f'\nTry 'tar --help' or 'tar --usage' for more information.\n");
        return 1;
    }
    if(argc == 3) {
        printf("my_tar: Cowardly refusing to create an empty archive\nTry 'tar --help' or 'tar --usage' for more information.\n");
        return 1;
    }
    int fd;
    if((fd = open(argv[2], O_RDONLY)) > 0) {
        unlink(argv[2]);
    }
    i8 flag = 0;
    i8 i = 3;
    lists* f_list = NULL;
    lists* error = NULL;
    for( ; i < argc; i++) {
        if((fd = open(argv[i], O_RDONLY)) < 0) {
            list_creator(&error, argv[i]);
            flag++;
        }
        if(my_strcmp(argv[i], "-cf") == 0) {
            cleaner(error);
            cleaner(f_list);
            printf("my_tar: option requires an argument -- 'f'\nTry 'my_tar --help' or 'tar --usage' for more information.");
            return 1;
        }
        if((my_strcmp("-xf", argv[i]) == 0) || (my_strcmp("-tf", argv[i]) == 0) || (my_strcmp("-uf", argv[i]) == 0) || (my_strcmp("-rf", argv[i]) == 0)) {
            cleaner(error);
            cleaner(f_list);
            printf("my_tar: You may not specify more than one '-Acdtrux', '--delete' or  '--test-label' option\nTry 'my_tar --help' or 'tar --usage' for more information.\n");
            return 1;
        }
        else {
            list_creator(&f_list, argv[i]);
            close(fd);
        }
    }
    creat(argv[2], 000644);
    lists* loop = f_list;
    fd = open(argv[2], O_WRONLY | O_APPEND);
    char* str_tar = calloc(sizeof(char), 513);
    int f_d;
    posix_header f_data;
    for( ; loop; loop = loop->next) {
        fill_tar_file(&f_data, loop->name);
        write(fd, &f_data, 512);
        f_d = open(loop->name, O_RDONLY);
        while(read(f_d, str_tar, 512)) {
            write(fd, str_tar, 512);
            null(str_tar, 513);
        }
        close(f_d);
    }
    free(str_tar);
    cleaner(f_list);
    if(flag) {
        lists* head = error;
        for( ; error; error = error->next) {
            printf("my_tar: %s: Cannot stat: No such file or directory\n", error->name);
        }
        printf("my_tar: Exiting with failure status due to previous errors\n");
        cleaner(head);
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}

i8 tf(int argc, char** argv) {
    if(argc == 2) {
        printf("my_tar: option requires an argument -- 'f'\nTry 'my_tar --help' or 'tar --usage' for more information.\n");
        return 1;
    }
    if(argc > 3) {
        printf("my_tar: Skipping to next header\n");
        for(i8 i = 3; i < argc; i++) {
            printf("my_tar: %s: Not found in archive\n", argv[i]);
        }
        printf("my_tar: Exiting with failure status due to previous errors\n");
        return 1;
    }
    int fd;
    if((fd = open(argv[2], O_RDONLY) < 0) && argc == 3) {
        printf("my_tar: %s: Cannot open: No such file or directory\nmy_tar: Error is not recoverable: exiting now\n", argv[2]);
        return 1;
    }
    close(fd);
    if(argc > 3) {
        fd = open(argv[2], O_RDONLY);
        printf("%d\n", fd);
        if(fd < 0) {
            printf("my_tar: %s: Not found in archive\nmy_tar: Exiting with failure status due to previous errors\n", argv[2]);
            return 1;
        }
        else {
            close(fd);
            printf("my_tar: %s: Not found in archive\nmy_tar: Exiting with failure status due to previous errors\n", argv[3]);
            return 1;
        }
    }
    fd = open(argv[2], O_RDONLY);
    posix_header f_data;
    while(read(fd, &f_data, 512)) {
        if(my_strcmp(f_data.magic, "ustar  ")  == 0 && my_isdigit(f_data.mtime) == 0) {
            printf("%s\n", f_data.name);
        }
        tar_null(&f_data);
    }
    close(fd);
    return 0;
}

i8 rf(char** argv, int argc) {
    if(argc == 2) {
        printf("my_tar: option requires an argument -- 'f'\nTry 'tar --help' or 'tar --usage' for more information.\n");
        return 1; 
    }
    int fd;
    if((fd = open(argv[2], O_WRONLY | O_APPEND)) < 0) {
        creat(argv[2], 000644);
    }
    if(argc == 3) {
        return 0;
    }
    close(fd);
    i8 i = 3;
    i8 flag = 0;
    lists* f_list = NULL;
    lists* error = NULL;
    for( ; i < argc; i++) {
        if((fd = open(argv[i], O_RDONLY)) < 0) {
            list_creator(&error, argv[i]);
            flag++;
        }
        else {
            close(fd);
            list_creator(&f_list, argv[i]);
        }
        if(my_strcmp(argv[i], "-tf") == 0) {
            cleaner(error);
            cleaner(f_list);
            printf("my_tar: option requires an argument -- 'f'\nTry 'tar --help' or 'tar --usage' for more information.\n");
            return 1;
        }
        if((my_strcmp(argv[i], "-cf") == 0) || (my_strcmp(argv[i], "-xf") == 0) || (my_strcmp(argv[i], "-uf") == 0 || (my_strcmp(argv[i], "-rf") == 0))) {
            cleaner(error);
            cleaner(f_list);
            printf("my_tar: You may not specify more than one '-Acdtrux', '--delete' or  '--test-label' option\nTry 'tar --help' or 'tar --usage' for more information.\n");
            return 1;
        }
    }
    fd = open(argv[2], O_WRONLY | O_APPEND);
    lists* head = f_list;
    lists* header = error;
    posix_header f_info;
    int f_d;
    char* str = calloc(sizeof(char), 513);
    for( ; f_list; f_list = f_list->next) {
        fill_tar_file(&f_info, f_list->name);
        write(fd, &f_info, 512);
        f_d = open(f_list->name, O_RDONLY);
        while(read(f_d, str, 512)) {
            write(fd, str, 512);
            null(str, 512);
        }                   
        close(f_d);
    } 
    free(str);
    if(flag) {
        for( ; error; error = error->next) {
            printf("my_tar: %s: Cannot open: No such file or directory\n", error->name);
        }
        cleaner(header);
        printf("my_tar: Exiting with failure status due to previous errors\n");
        cleaner(head);
        close(fd);
        return 1;
    }
    close(fd);
    cleaner(head);
    return 0;
}

i8 uf(char** argv, int argc) {
    if(argc == 2) {
        printf("my_tar: option requires an argument -- 'f'\nTry 'my_tar --help' or 'tar --usage' for more information.\n");
        return 1;
    }
    int fd = open(argv[2], O_RDONLY);
    if(fd == -1) {
        creat(argv[2], 000644);
    }
    if(argc == 3) {
        return 0;
    }
    close(fd);
    fd = open(argv[2], O_RDWR | O_APPEND);
    int f_d;
    i8 i;
    lists* error = NULL;
    for(i = 3; i < argc; i++) {
        if(my_strcmp(argv[i], "-uf") == 0) {
            printf("my_tar: option requires an argument -- 'f'\nTry 'my_tar --help' or 'tar --usage' for more information.\n");
            cleaner(error);
            return 1;
        }
        if(my_strcmp(argv[i], "-rf") == 0 || my_strcmp(argv[i], "-xf") == 0 || my_strcmp(argv[i], "-cf") == 0 || my_strcmp(argv[i], "-tf") == 0) {
            printf("my_tar: You may not specify more than one '-Acdtrux', '--delete' or  '--test-label' option\nTry 'tar --help' or 'tar --usage' for more information.\n");
            cleaner(error);
            return 1;
        }
        if((f_d = open(argv[i], O_RDONLY)) < 0) {
            list_creator(&error, argv[i]);
        }
        else {
            close(f_d);
        }
    }
    fd = open(argv[2], O_RDWR | O_APPEND);
    list_tar* t_list = NULL;
    posix_header* f_data  = (posix_header*)malloc(sizeof(posix_header));
    while(read(fd, f_data, 512)) {
        if(my_strcmp(f_data->magic, "ustar  ") == 0 && my_isdigit(f_data->mtime) == 0) {
            tar_list_creator(&t_list, f_data);
        }
        else{ 
            tar_null(f_data);
            f_data->typeflag = '\0';
            continue;
        }
        f_data = (posix_header*)malloc(sizeof(posix_header));
    }
    free(f_data);
    list_tar* tar_list;
    i8 flag;
    char* str;
    struct stat f_stat;
    lists* f_list = NULL;
    for(i = 3; i < argc; i++) {
        flag = 0;
        for(tar_list  = t_list; tar_list; tar_list = tar_list->next) {
            f_d = open(argv[i], O_RDONLY);
            if(my_strcmp(argv[i], tar_list->name_tar_list->name) == 0 && f_d != -1) {
                stat(argv[i], &f_stat);
                str = decimal_change_octal(f_stat.st_mtime, 11);
                if(ascii(str) != ascii(tar_list->name_tar_list->mtime)) {
                    flag = 1;
                }
                else {
                    flag = 2;
                }
                close(f_d);
                free(str);
            }
            if(f_d == -1) {
                flag = 4;
                break;
            }
        }
        if(flag == 0 || flag == 1) {
            list_creator(&f_list, argv[i]);
        }
    }
    cleaner_tar_list(t_list);
    lists* list = f_list;
    str = (char*)calloc(sizeof(char), 513);
    f_data = (posix_header*)malloc(sizeof(posix_header));
    for( ; list; list = list->next) {
        fill_tar_file(f_data, list->name);
        write(fd, f_data, 512);
        f_d = open(list->name, O_RDONLY);
        while(read(f_d, str, 512)) {
            write(fd, str, 512);
            null(str, 513);
        }
        close(f_d);
        tar_null(f_data);
    }
    free(str);
    free(f_data);
    cleaner(f_list);
    list = error;
    while(list) {
        printf("my_tar: %s: Cannot stat: No such file or directory\n", list->name);
        list = list->next;
        if(list == NULL) {
            printf("my_tar: Exiting with failure status due to previous errors\n");
            cleaner(error);
            return 1;
        }
    }
    return 0;
   
}

i8 xf(char** argv, int argc) {
    if(argc == 2) {
        printf("my_tar: option requires an argument -- 'f'\nTry 'my_tar --help' or 'tar --usage' for more information.");
        return 1;
    }
    int fd;
    fd = open(argv[2], O_RDONLY);
    if(fd == -1) {
        printf("my_tar: %s: Cannot open: No such file or directory\nmy_tar: Error is not recoverable: exiting now", argv[2]);
        return 1;
    }
    if(argc == 3) {
        return 0;
    }
    short i;
    for(i = 3; i < argc; i++) {
        if(my_strcmp(argv[i], "-xf") == 0) {
            printf("my_tar: option requires an argument -- 'f'\nTry 'tar --help' or 'tar --usage' for more information.\n");
            return 1;
        }
        if(my_strcmp(argv[i], "-cf") == 0 || my_strcmp(argv[i], "-uf") == 0 || my_strcmp(argv[i], "-rf") == 0 || my_strcmp(argv[i], "-tf") == 0) {
            printf("my_tar: You may not specify more than one '-Acdtrux', '--delete' or  '--test-label' option\nTry 'tar --help' or 'tar --usage' for more information.\n");
            return 1;
        }
    }
    list_tar_content* t_list = NULL;
    posix_header* f_data = (posix_header*)malloc(sizeof(posix_header));
    while(read(fd, f_data, 512)){
        if(my_strcmp(f_data->magic, "ustar  ") == 0 && my_isdigit(f_data->mtime) == 0) {
            if(xf_correct(t_list, f_data) == 0) {
                list_tar_content_creator(&t_list, f_data->name, f_data->mtime);
            }
        }
        tar_null(f_data);
        f_data->typeflag = '\0';
    }
    list_tar_content* temp = t_list;
    lists* error = NULL;
    list_tar_content* f_list = NULL;
    char* str;
    for(i = 3; i < argc; i++) {
        if((str = error_check(temp, f_list, argv[i])) == NULL) {
            list_creator(&error, argv[i]);
        }
        else { 
            list_tar_content_creator(&f_list, argv[i], str);
        }
    }
    list_tar_content_cleaner(t_list);
    close(fd);
    int f_d;
    fd = open(argv[2], O_RDONLY);
    temp = f_list;
    str = (char*)calloc(sizeof(char), 513);
    while(read(fd, f_data, 512)) {
        if(my_strcmp(f_data->magic, "ustar  ") == 0 && my_isdigit(f_data->mtime) == 0) {
            if(check(temp, f_data)) {
                creat(f_data->name, 000644);
                f_d = open(f_data->name, O_WRONLY | O_APPEND);
                while(read(fd, str, 512)) {
                    if(null_check(str, 512)) {
                        for(i = 0; str[i] != '\0'; i++) {
                            write(f_d, &str[i], 1);
                        }
                        break;
                    }
                    write(f_d, str, 512);
                    null(str, 513);
                }
                close(f_d);
            }
        }
    }
    list_tar_content_cleaner(f_list);
    free(f_data);
    close(fd);
    free(str);
    lists* tmp = error;
    while(tmp) {
        printf("my_tar: %s: Not found in archive\n", tmp->name);
        tmp = tmp->next;
        if(!tmp) {
            cleaner(error);
            printf("my_tar: Exiting with failure status due to previous errors\n");
            return 1;
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    if(argc == 1) {
        printf("my_tar: You must specify one of the '-Acdtrux', '--delete' or '--test-label' options\nTry 'tar --help' or 'tar --usage' for more information.\n");
        return 1;
    }
    if(my_strcmp(argv[1], "-cf") == 0) {
        return cf(argc, argv);
    }
    if(my_strcmp(argv[1], "-tf") == 0) {
        return tf(argc, argv);
    }
    if(my_strcmp(argv[1], "-rf") == 0) {
        return rf(argv, argc);
    }
    if(my_strcmp(argv[1], "-uf") == 0) {
        return uf(argv, argc);
    }
    if(my_strcmp(argv[1], "-xf") == 0) {
        return xf(argv, argc);
    }
    else {
        printf("my_tar: You must specify one of the '-Acdtrux', '--delete' or '--test-label' options\nTry 'tar --help' or 'tar --usage' for more information.\n");
        return 1;
    }
    return 0;
}
