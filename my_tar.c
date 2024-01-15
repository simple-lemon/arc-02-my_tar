#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "./h/linked_list.h"
#include "./h/my_string.h" 

void filling_tar_file(posix_header* f_data, char* f_name) 
{
    tar_null(f_data);
    struct stat f_stat;
    stat(f_name, &f_stat);
    short chsum = 256;
    char* str;
    my_strcpy(f_data->name, f_name);
    chsum += asc(f_name);
    my_strcpy(f_data->mode, (str = dec_to_oc(f_stat.st_mode, 7)));
    free(str);
    chsum += asc(f_data->mode);
    my_strcpy(f_data->size, (str = dec_to_oc(f_stat.st_size, 11)));
    free(str); 
    chsum += asc(f_data->size);
    my_strcpy(f_data->uid, (str = dec_to_oc(f_stat.st_uid, 7)));
    free(str);
    chsum += asc(f_data->uid);
    my_strcpy(f_data->gid, (str = dec_to_oc(f_stat.st_gid, 7)));
    free(str);
    chsum += asc(f_data->gid);
    my_strcpy(f_data->mtime, (str = dec_to_oc(f_stat.st_mtime, 11)));
    free(str);
    chsum += asc(f_data->mtime);
    f_data->typeflag = '0';
    chsum += 48;
    my_strcpy(f_data->uname, "docode");
    chsum += (2*asc("docode"));
    my_strcpy(f_data->gname, "docode");
    chsum += asc("ustar  ");
    my_strcpy(f_data->magic, "ustar  ");
    my_strcpy(f_data->chksum, (str = dec_to_oc(chsum, 7)));
    free(str);
}

i8 xf_correct(list_tar_content* list, posix_header* f_data) 
{
    list_tar_content* temp = list;
    for( ; temp; temp = temp->next) 
    {
        if(my_strcmp(temp->name, f_data->name) == 0 && my_strcmp(temp->mtime, f_data->mtime) < 0) 
        {
            temp->mtime = my_strcpy(temp->mtime, f_data->mtime);
            return 1;
        }
        if(my_strcmp(temp->name, f_data->name) == 0 && my_strcmp(temp->mtime, f_data->mtime) == 0) 
        {
            return 1;
        }
    }
    return 0;
}

i8 check(list_tar_content* list, posix_header* f_data) 
{
    list_tar_content* temp = list;
    int fd;
    for( ; temp; temp = temp->next) 
    {
        if(my_strcmp(f_data->name, temp->name) == 0 && my_strcmp(f_data->mtime, temp->mtime) == 0) 
        {
            fd = open(temp->name,  O_RDONLY);
            if(fd == -1) 
            {
                return 1;
            } 
            else 
            {
                close(fd);
            }
        }
    }
    return 0;
}

char* error_check(list_tar_content* t_list, list_tar_content* f_list, char* name) 
{
    char* result = NULL;
    list_tar_content* temp = t_list;
    for( ; temp; temp = temp->next) {
        if(my_strcmp(name, temp->name) == 0) 
        {
            if(my_isdigit(temp->mtime) == 0) 
            {
                result = temp->mtime;
            }
            break;
        }
    }
    for(temp = f_list; temp; temp = temp->next) 
    {
        if(my_strcmp(name, temp->name) == 0) 
        {
            return NULL;
        }
    }
    return result;
}

i8 cf(int argc, char** argv)
{
    if(argc == 2) 
    {
        printf("my_tar: option requires an argument -- 'f'\nTry 'tar --help' or 'tar --usage' for more information.\n");
        return 1;
    }
    if(argc == 3) 
    {
        printf("my_tar: Cowardly refusing to create an empty archive\nTry 'tar --help' or 'tar --usage' for more information.\n");
        return 1;
    }
    int fd;
    if((fd = open(argv[2], O_RDONLY)) > 0) 
    {
        unlink(argv[2]);
    }
    i8 flag = 0;
    i8 i = 3;
    lists* f_list = NULL;
    lists* error = NULL;
    for( ; i < argc; i++) {
        if((fd = open(argv[i], O_RDONLY)) < 0)
        {
            add_list(&error, argv[i]);
            flag++;
        }
        if(my_strcmp(argv[i], "-cf") == 0) 
        {
            clean_list(error);
            clean_list(f_list);
            printf("my_tar: option requires an argument -- 'f'\nTry 'my_tar --help' or 'tar --usage' for more information.");
            return 1;
        }
        if((my_strcmp("-xf", argv[i]) == 0) || (my_strcmp("-tf", argv[i]) == 0) || (my_strcmp("-uf", argv[i]) == 0) || (my_strcmp("-rf", argv[i]) == 0)) 
        {
            clean_list(error);
            clean_list(f_list);
            printf("my_tar: You may not specify more than one '-Acdtrux', '--delete' or  '--test-label' option\nTry 'my_tar --help' or 'tar --usage' for more information.\n");
            return 1;
        }
        else 
        {
            add_list(&f_list, argv[i]);
            close(fd);
        }
    }
    creat(argv[2], 000644);
    lists* loop = f_list;
    fd = open(argv[2], O_WRONLY | O_APPEND);
    char* str_tar = calloc(sizeof(char), 513);
    int f_d;
    posix_header f_data;
    for( ; loop; loop = loop->next) 
    {
        filling_tar_file(&f_data, loop->name);
        write(fd, &f_data, 512);
        f_d = open(loop->name, O_RDONLY);
        while(read(f_d, str_tar, 512))
        {
            write(fd, str_tar, 512);
            null(str_tar, 513);
        }
        close(f_d);
    }
    free(str_tar);
    clean_list(f_list);
    if(flag) 
    {
        lists* head = error;
        for( ; error; error = error->next) 
        {
            printf("my_tar: %s: Cannot stat: No such file or directory\n", error->name);
        }
        printf("my_tar: Exiting with failure status due to previous errors\n");
        clean_list(head);
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}

i8 tf(int argc, char** argv) 
{
    if(argc == 2) 
    {
        printf("my_tar: option requires an argument -- 'f'\nTry 'my_tar --help' or 'tar --usage' for more information.\n");
        return 1;
    }
    if(argc > 3) 
    {
        printf("my_tar: Skipping to next header\n");
        for(i8 i = 3; i < argc; i++) {
            printf("my_tar: %s: Not found in archive\n", argv[i]);
        }
        printf("my_tar: Exiting with failure status due to previous errors\n");
        return 1;
    }
    int fd;
    if((fd = open(argv[2], O_RDONLY) < 0) && argc == 3) 
    {
        printf("my_tar: %s: Cannot open: No such file or directory\nmy_tar: Error is not recoverable: exiting now\n", argv[2]);
        return 1;
    }
    close(fd);
    if(argc > 3) 
    {
        fd = open(argv[2], O_RDONLY);
        printf("%d\n", fd);
        if(fd < 0) 
        {
            printf("my_tar: %s: Not found in archive\nmy_tar: Exiting with failure status due to previous errors\n", argv[2]);
            return 1;
        }
        else 
        {
            close(fd);
            printf("my_tar: %s: Not found in archive\nmy_tar: Exiting with failure status due to previous errors\n", argv[3]);
            return 1;
        }
    }
    fd = open(argv[2], O_RDONLY);
    posix_header f_data;
    while(read(fd, &f_data, 512)) 
    {
        if(my_strcmp(f_data.magic, "ustar  ")  == 0 && my_isdigit(f_data.mtime) == 0) {
            printf("%s\n", f_data.name);
        }
        tar_null(&f_data);
    }
    close(fd);
    return 0;
}

i8 rf(char** argv, int argc) 
{
    if(argc == 2) 
    {
        printf("my_tar: option requires an argument -- 'f'\nTry 'tar --help' or 'tar --usage' for more information.\n");
        return 1; 
    }
    int fd;
    if((fd = open(argv[2], O_WRONLY | O_APPEND)) < 0) 
    {
        creat(argv[2], 000644);
    }
    if(argc == 3) 
    {
        return 0;
    }
    close(fd);
    i8 i = 3;
    i8 flag = 0;
    lists* f_list = NULL;
    lists* error = NULL;
    for( ; i < argc; i++) 
    {
        if((fd = open(argv[i], O_RDONLY)) < 0) 
        {
            add_list(&error, argv[i]);
            flag++;
        }
        else 
        {
            close(fd);
            add_list(&f_list, argv[i]);
        }
        if(my_strcmp(argv[i], "-tf") == 0) 
        {
            clean_list(error);
            clean_list(f_list);
            printf("my_tar: option requires an argument -- 'f'\nTry 'tar --help' or 'tar --usage' for more information.\n");
            return 1;
        }
        if((my_strcmp(argv[i], "-cf") == 0) || (my_strcmp(argv[i], "-xf") == 0) || (my_strcmp(argv[i], "-uf") == 0 || (my_strcmp(argv[i], "-rf") == 0))) 
        {
            clean_list(error);
            clean_list(f_list);
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
    for( ; f_list; f_list = f_list->next) 
    {
        filling_tar_file(&f_info, f_list->name);
        write(fd, &f_info, 512);
        f_d = open(f_list->name, O_RDONLY);
        while(read(f_d, str, 512)) 
        {
            write(fd, str, 512);
            null(str, 512);
        }                   
        close(f_d);
    } 
    free(str);
    if(flag) {
        for( ; error; error = error->next) 
        {
            printf("my_tar: %s: Cannot open: No such file or directory\n", error->name);
        }
        clean_list(header);
        printf("my_tar: Exiting with failure status due to previous errors\n");
        clean_list(head);
        close(fd);
        return 1;
    }
    close(fd);
    clean_list(head);
    return 0;
}

i8 uf(char** argv, int argc) 
{
    if(argc == 2) 
    {
        printf("my_tar: option requires an argument -- 'f'\nTry 'my_tar --help' or 'tar --usage' for more information.\n");
        return 1;
    }
    int fd = open(argv[2], O_RDONLY);
    if(fd == -1) 
    {
        creat(argv[2], 000644);
    }
    if(argc == 3) 
    {
        return 0;
    }
    close(fd);
    fd = open(argv[2], O_RDWR | O_APPEND);
    int f_d;
    i8 i;
    lists* error = NULL;
    for(i = 3; i < argc; i++)
    {
        if(my_strcmp(argv[i], "-uf") == 0) 
        {
            printf("my_tar: option requires an argument -- 'f'\nTry 'my_tar --help' or 'tar --usage' for more information.\n");
            clean_list(error);
            return 1;
        }
        if(my_strcmp(argv[i], "-rf") == 0 || my_strcmp(argv[i], "-xf") == 0 || my_strcmp(argv[i], "-cf") == 0 || my_strcmp(argv[i], "-tf") == 0) 
        {
            printf("my_tar: You may not specify more than one '-Acdtrux', '--delete' or  '--test-label' option\nTry 'tar --help' or 'tar --usage' for more information.\n");
            clean_list(error);
            return 1;
        }
        if((f_d = open(argv[i], O_RDONLY)) < 0) 
        {
            add_list(&error, argv[i]);
        }
        else {
            close(f_d);
        }
    }
    fd = open(argv[2], O_RDWR | O_APPEND);
    list_tar* t_list = NULL;
    posix_header* f_data  = (posix_header*)malloc(sizeof(posix_header));
    while(read(fd, f_data, 512)) 
    {
        if(my_strcmp(f_data->magic, "ustar  ") == 0 && my_isdigit(f_data->mtime) == 0) 
        {
            tar_list_add(&t_list, f_data);
        }
        else
        { 
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
    for(i = 3; i < argc; i++) 
    {
        flag = 0;
        for(tar_list  = t_list; tar_list; tar_list = tar_list->next) 
        {
            f_d = open(argv[i], O_RDONLY);
            if(my_strcmp(argv[i], tar_list->name_tar_list->name) == 0 && f_d != -1) 
            {
                stat(argv[i], &f_stat);
                str = dec_to_oc(f_stat.st_mtime, 11);
                if(asc(str) != asc(tar_list->name_tar_list->mtime)) 
                {
                    flag = 1;
                }
                else {
                    flag = 2;
                }
                close(f_d);
                free(str);
            }
            if(f_d == -1) 
            {
                flag = 4;
                break;
            }
        }
        if(flag == 0 || flag == 1) 
        {
            add_list(&f_list, argv[i]);
        }
    }
    clean_tar_list(t_list);
    lists* list = f_list;
    str = (char*)calloc(sizeof(char), 513);
    f_data = (posix_header*)malloc(sizeof(posix_header));
    for( ; list; list = list->next) 
    {
        filling_tar_file(f_data, list->name);
        write(fd, f_data, 512);
        f_d = open(list->name, O_RDONLY);
        while(read(f_d, str, 512)) 
        {
            write(fd, str, 512);
            null(str, 513);
        }
        close(f_d);
        tar_null(f_data);
    }
    free(str);
    free(f_data);
    clean_list(f_list);
    list = error;
    while(list) 
    {
        printf("my_tar: %s: Cannot stat: No such file or directory\n", list->name);
        list = list->next;
        if(list == NULL)
         {
            printf("my_tar: Exiting with failure status due to previous errors\n");
            clean_list(error);
            return 1;
        }
    }
    return 0;
   
}

i8 xf(char** argv, int argc) 
{
    if(argc == 2) 
    {
        printf("my_tar: option requires an argument -- 'f'\nTry 'my_tar --help' or 'tar --usage' for more information.");
        return 1;
    }
    int fd;
    fd = open(argv[2], O_RDONLY);
    if(fd == -1) 
    {
        printf("my_tar: %s: Cannot open: No such file or directory\nmy_tar: Error is not recoverable: exiting now", argv[2]);
        return 1;
    }
    if(argc == 3) 
    {
        return 0;
    }
    short i;
    for(i = 3; i < argc; i++) 
    {
        if(my_strcmp(argv[i], "-xf") == 0) 
        {
            printf("my_tar: option requires an argument -- 'f'\nTry 'tar --help' or 'tar --usage' for more information.\n");
            return 1;
        }
        if(my_strcmp(argv[i], "-cf") == 0 || my_strcmp(argv[i], "-uf") == 0 || my_strcmp(argv[i], "-rf") == 0 || my_strcmp(argv[i], "-tf") == 0) 
        {
            printf("my_tar: You may not specify more than one '-Acdtrux', '--delete' or  '--test-label' option\nTry 'tar --help' or 'tar --usage' for more information.\n");
            return 1;
        }
    }
    list_tar_content* t_list = NULL;
    posix_header* f_data = (posix_header*)malloc(sizeof(posix_header));
    while(read(fd, f_data, 512))
    {
        if(my_strcmp(f_data->magic, "ustar  ") == 0 && my_isdigit(f_data->mtime) == 0) 
        {
            if(xf_correct(t_list, f_data) == 0) 
            {
                add_tar_content_list(&t_list, f_data->name, f_data->mtime);
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
        if((str = error_check(temp, f_list, argv[i])) == NULL) 
        {
            add_list(&error, argv[i]);
        }
        else 
        { 
            add_tar_content_list(&f_list, argv[i], str);
        }
    }
    list_tar_content_clean(t_list);
    close(fd);
    int f_d;
    fd = open(argv[2], O_RDONLY);
    temp = f_list;
    str = (char*)calloc(sizeof(char), 513);
    while(read(fd, f_data, 512)) 
    {
        if(my_strcmp(f_data->magic, "ustar  ") == 0 && my_isdigit(f_data->mtime) == 0) {

            if(check(temp, f_data)) {
                creat(f_data->name, 000644);
                f_d = open(f_data->name, O_WRONLY | O_APPEND);
                while(read(fd, str, 512)) 
                {
                    if(null_check(str, 512))
                    {
                        for(i = 0; str[i] != '\0'; i++) 
                        {
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
    list_tar_content_clean(f_list);
    free(f_data);
    close(fd);
    free(str);
    lists* tmp = error;
    while(tmp) {
        printf("my_tar: %s: Not found in archive\n", tmp->name);
        tmp = tmp->next;
        if(!tmp) {
            clean_list(error);
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
