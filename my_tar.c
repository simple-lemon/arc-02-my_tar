#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
typedef struct s_posix_header
{                              /* byte offset */
    char name[100];               /*   0 */
    char mode[8];                 /* 100 */
    char uid[8];                  /* 108 */
    char gid[8];                  /* 116 */
    char size[12];                /* 124 */
    char mtime[12];               /* 136 */
    char chksum[8];               /* 148 */
    char typeflag;                /* 156 */
    char linkname[100];           /* 157 */
    char magic[6];                /* 257 */
    char version[2];              /* 263 */
    char uname[32];               /* 265 */
    char gname[32];               /* 297 */
    char devmajor[8];             /* 329 */
    char devminor[8];             /* 337 */
    char prefix[155];             /* 345 */
    char just_spc[12];            /* 500 */
                                  /* 512 */
}posix_header;

typedef struct s_Node{
    char* name;
    struct stat fs;
    struct s_Node *next;
} listnode;

int my_strcmp(char* p1, char* p2){
    for(int i  = 0; p1[i] || p2[i]; i++){
        if(p1[i] != p2[i]){
            return (int)(p1[i] - p2[i]);
        }
    }
    return 0;
}

int my_strlen(char* word){
    int len = 0;
    while(word[len]) len++;
    return len;
}

int ascii_sum(char* p1){
    int sum=0;
    for(int i=0; i<my_strlen(p1); i++){
        sum+=(int)p1[i];
    }
    return sum;
}

void my_strcpy(char* dest, char* piece){
    for(int i=0; i<my_strlen(piece); i++){
        dest[i]=piece[i];
    }
}

void my_strcat(char* dest, char* piece){
    int index=0;
    for(int i=my_strlen(dest); index<my_strlen(piece); i++){
        dest[i]=piece[index];
        index++;
    }
}

void putstr(int where_to, char* word){
    write(where_to,word,my_strlen(word));
}

void write_to_file(int fd, int index, char* string){
    lseek(fd, index, SEEK_SET);
    write(fd, string, my_strlen(string));
}

void print_error(char* filename, int mode){
    if (mode==1){
        char str1[] = "my_tar: ";
        char str2[] = ": Cannot stat: No such file or directory\n"; 
        putstr(2, str1);
        putstr(2, filename);
        putstr(2, str2);    
    }
    else if (mode==2){
        char str1[] = "my_tar: Cannot open \n";
        putstr(2, str1);
        putstr(2, filename);
    }
}

void addlast(listnode** list, char* name){
    listnode* temp = (listnode*)malloc(sizeof(listnode));
    temp->name = name;
    stat(name, &temp->fs);
    temp->next = NULL;
    if(*list == NULL){
        *list = temp;
    }else{
        listnode* t1 = *list;
        while(t1->next != NULL){
            t1 = t1->next;
        }
        t1->next = temp;
    }
}

char* int_to_string(int n) {

    int sign = (n < 0) ? -1 : 1;
    size_t len = 1;
    int tmp = n / 10;
    while (tmp != 0) {
        len++;
        tmp /= 10;
    }
    if (sign == -1) len++;
    char* str = malloc((len + 1) * sizeof(char));
    if (str == NULL) return NULL;
    if (sign == -1) {
        *str = '-';
        n *= -1;
    }
    str[len] = '\0';
    while (len > 0) {
        str[len - 1] = '0' + (n % 10);
        n /= 10;
        len--;
    }
    return str;
}

void fill_null(char* string, int length){
    for(int i=0; i<length; i++){
        string[i]='\0';
    }
}

int pow_function(int p1, int p2){
    if (!p2) return 1;
    int sum = 1;
    for(int i=0; i<p2; i++){
        sum*=p1;
    }
    return sum;
}

char* strip_zero(char* string){
    int length = my_strlen(string), index=0;
    int copy=0;
    char* result = (char*)calloc(length,sizeof(char));
    for(int i=0; i<my_strlen(string); i++){
        if(!copy){
            if (string[i]!='0') copy=1;
        }
        if (copy){
            result[index]=string[i];
            index++;
        }
    }
    return result;
}

int OctToDecimal(char* octnum){
    int index=0, decimal=0;
    char* oct_without_zero = strip_zero(octnum);
    for(int i=my_strlen(oct_without_zero)-1; i>=0; i--){
        decimal+=(oct_without_zero[i]-48)*pow_function(8,index);
        index++;
    }
    return decimal;
}

char* dec_to_oct(int decnum, int space) {
    int oc_num=0, i=1;
    while (decnum!=0) {
        oc_num+=(decnum%8)*i;
        decnum/=8;
        i*=10;
    }
    char* oc_char = int_to_string(oc_num);
    char* result = (char*)calloc(space,sizeof(char));
    fill_null(result, space);

    for(int i=0; i<(space-(int)my_strlen(oc_char)-1); i++){
        result[i]='0';
    }
    my_strcat(result, oc_char);
    free(oc_char);
    return result;
}

void pos_fill_null(posix_header* sth){
    fill_null(sth->name, 100);
    fill_null(sth->mode, 8);
    fill_null(sth->uid, 8);
    fill_null(sth->gid, 8);
    fill_null(sth->size, 12);
    fill_null(sth->mtime, 12);
    fill_null(sth->chksum, 8);
    fill_null(sth->linkname, 100);
    fill_null(sth->magic, 6);
    fill_null(sth->version, 2);
    fill_null(sth->uname, 32);
    fill_null(sth->gname, 32);
    fill_null(sth->devmajor, 8);
    fill_null(sth->devminor, 8);
    fill_null(sth->prefix, 155);
    fill_null(sth->just_spc, 12);
}

void fill_pos(posix_header* sth, char* filename){
    struct stat fs;
    stat(filename, &fs);
    pos_fill_null(sth);
    int chksum = 256;
    my_strcpy(sth->name,filename);
    my_strcpy(sth->mode, dec_to_oct(fs.st_mode,8));
    my_strcpy(sth->uid, dec_to_oct(fs.st_uid,8));
    my_strcpy(sth->gid, dec_to_oct(fs.st_gid,8));
    my_strcpy(sth->size, dec_to_oct(fs.st_size,12));
    my_strcpy(sth->mtime, dec_to_oct(fs.st_mtim.tv_sec,12));
    sth->typeflag = '0';
    my_strcpy(sth->magic, "ustar  ");
    my_strcpy(sth->uname, "docode");
    my_strcpy(sth->gname, "docode");
    chksum+=ascii_sum(sth->name);
    chksum+=ascii_sum(sth->mode);
    chksum+=ascii_sum(sth->uid);
    chksum+=ascii_sum(sth->gid);
    chksum+=ascii_sum(sth->size);
    chksum+=ascii_sum(sth->mtime);
    chksum+=48;
    chksum+=ascii_sum(sth->magic);
    chksum+=ascii_sum(sth->uname);
    chksum+=ascii_sum(sth->gname);
    my_strcpy(sth->chksum, dec_to_oct(chksum, 8));
}

void name_of_files(char** argv, int argc){
    if(argc > 3){
        for(int i = 3; i < argc; i++){
            print_error(argv[i], 2);
        }
        return;
    }
    int fd = open(argv[2], O_RDONLY);
    posix_header sth;
    while(read( fd, &sth, 512)){
        if(!my_strcmp(sth.magic, "ustar  ")){
            putstr(1, sth.name);
            putstr(1,"\n");
        }
    }
    close(fd);
}

void uf_update(char** argv, int argc){
    for(int i = 3; i < argc; i++){
        int file_info = open(argv[i], O_RDONLY), fd = open(argv[2], O_RDONLY), spotted = 0;
        struct stat fs;
        posix_header something;
        stat(argv[i], &fs);
        int mtim = fs.st_mtim.tv_sec;
        int time_update = mtim;
        while(read(fd, &something, 512)){
            if(!my_strcmp(something.name, argv[i])){
                spotted=1;
                if(time_update<OctToDecimal(something.mtime)){
                    time_update = OctToDecimal(something.mtime);
                }
            }
        }
        close(fd);
        close(file_info);
        if(!spotted || time_update<mtim){

            int filedata = open(argv[2], O_WRONLY | O_APPEND);
            fill_pos(&something, argv[i]);
            write(fd, &something, 512);

            char* string = (char*)calloc(513,sizeof(char));
            fill_null(string, 513);
            while(read(filedata, string, 512)){
                write(fd, string, 512);
                fill_null(string, 513);
            }
            free(string);
            close(filedata);
        }
    }
}

void update(char** argv, int argc){
    int fd = open(argv[2], O_WRONLY | O_APPEND);
    for(int i = 3; i < argc; i++){
        posix_header something;
        fill_pos(&something, argv[i]);

        write(fd, &something, 512);

        char* string = (char*)calloc(513,sizeof(char));
        fill_null(string, 513);
        int filedata = open(argv[i], O_RDONLY);
        while(read(filedata, string, 512)){
            write(fd, string, 512);
            fill_null(string, 513);
        }
        free(string);
        close(filedata);
    }
    close(fd);
}

void create_file(int argc, char** argv){
    if(open(argv[2], O_RDONLY) > 0){
        unlink(argv[2]);
    }
    listnode* list = NULL;
    int fd;
    for(int i = 3; i < argc; i++){
        if((fd = open(argv[i], O_RDONLY)) > 0){
            addlast(&list, argv[i]);
        }else{
            print_error(argv[i], 1);
        }
        close(fd);
    }
    creat(argv[2], 0000644);

    listnode* list_cpy = list;

    fd = open(argv[2], O_WRONLY | O_APPEND);
    while(list_cpy){
        posix_header sth;
        fill_pos(&sth, list_cpy->name);

        write(fd, &sth, 512);

        char* string = (char*)calloc(513,sizeof(char));
        fill_null(string, 513);
        int filedata = open(sth.name, O_RDONLY);
        while(read(filedata, string, 512)){
            write(fd, string, 512);
            fill_null(string, 513);
        }
        free(string);
        close(filedata);
        list_cpy = list_cpy->next;
    }
    close(fd);
}

void xf_function(char** argv, int argc){
    for(int i = 2; i < argc; i++){
        int fd = open(argv[i], O_RDONLY);
        posix_header pos_h;
        while(read(fd, &pos_h, 512)){
            if(!my_strcmp(pos_h.magic,"ustar  ")){
                int length = 0, size = OctToDecimal(pos_h.size), mode = OctToDecimal(pos_h.mode);
                char* filename = (char*)calloc(my_strlen(pos_h.name)+1, sizeof(char));
                char* content = (char*)calloc(size,sizeof(char));
                char* temp = (char*)calloc(513,sizeof(char));
                my_strcpy(filename, pos_h.name);
                while(length<=size){
                    read(fd, temp, 512);
                    my_strcat(content, temp);
                    fill_null(temp, 512);
                    length+=512;
                }
                    creat(filename, mode);
                    int filedata = open(filename, O_WRONLY);
                    write(filedata, content, size);
                    close(filedata);
            }
        }
        close(fd);
    }
}

int main(int argc, char** argv){
    if(*argv[1]){
        if(!(my_strcmp(argv[1], "-cf"))){
            create_file(argc, argv);
        }
        else if (!(my_strcmp(argv[1], "-tf"))) {
            name_of_files(argv, argc);
        }
        else if (!(my_strcmp(argv[1], "-rf"))) {
            update(argv, argc);
        }
        else if (!(my_strcmp(argv[1], "-uf"))) {
            uf_update(argv, argc);
        }
        else if (!(my_strcmp(argv[1], "-xf"))) {
            xf_function(argv, argc);
        }
    }
    return 0;
}