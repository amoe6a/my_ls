#ifndef MY_LS_H
#define MY_LS_H

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#define MAXSIZE 256// max size of the filename do not exceed 256 bytes.
#define BILLION 1000000000

typedef struct s_opts
{
    int is_R;
    int is_a;
    int is_t;
} options;
unsigned long my_strlen(const char *arr);
char *my_strcpy(char *param_1, const char *param_2);
char *my_strncpy(char *param_1, const char *param_2, int size);
int my_strcmp(const char *param_1, const char *param_2);
char *my_strstr(const char *hay, const char *needl);
int *my_int_memset(int *arr, int c, unsigned long len);
char *my_char_memset(char *arr, int c, unsigned long len);
char *my_strcat(char *dst, const char *src);
char *my_strncat(char *dst, const char *src, unsigned long sizen);

void errmsg();
void swap_Secs(void **first, void **second);
void swap_strings(char *first, char *second);
void sort_filenames(char filename[][MAXSIZE], int numfiles);
void sort_by_time(char filename[][MAXSIZE], int numfiles, const char *path);
char *argcase(const char *str, char *string_to_return);
options *give_opt_Rat(const char *argv);
char *list_files(const char *path, options *opt);
int path_exist_and_isDir(const char *path);
void do_ls(char *path, options *opt);

#endif