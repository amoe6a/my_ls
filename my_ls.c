#include "my_ls.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void sort_filenames(char filename[][MAXSIZE], int numfiles)
{
    for (size_t i = 1; (int)i < numfiles; i++)
    {
		for (size_t j = 1; (int)j < numfiles; j++)
        {
            if (my_strcmp(filename[j - 1], filename[j]) > 0)
                swap_strings(filename[j - 1], filename[j]);
        }
	}
}
void sort_by_time(char filename[][MAXSIZE], int numfiles, const char *path)
{
    char newpath[numfiles][MAXSIZE];
    struct stat buff[numfiles];
    long long file_time[numfiles];
    for (size_t i = 0; (int)i < numfiles; i++)
    {
        sprintf(newpath[i], "%s/%s", path, filename[i]);
        stat(newpath[i], &buff[i]);
        file_time[i] = buff[i].st_mtim.tv_sec * BILLION + buff[i].st_mtim.tv_nsec;
    }
    for (size_t i = 1; (int)i < numfiles; i++)
    {
		for (size_t j = 1; (int)j < numfiles; j++)
        {
            if (file_time[j - 1] < file_time[j])
            {
				swap_strings(filename[j - 1], filename[j]);
                swap_Secs((void **)&file_time[j - 1], (void **)&file_time[j]);
			}
            else if (file_time[j - 1] == file_time[j])
            {
                if (my_strcmp(filename[j - 1], filename[j]) > 0)
                {
                    swap_strings(filename[j - 1], filename[j]);
                    swap_Secs((void **)&file_time[j - 1], (void **)&file_time[j]);
                }
            }
        }
	}
}
char *argcase(const char *str, char *string_to_return)
{
    if (str == NULL || *str != '-' || my_strlen(str) <= 1) return "err";
    int size = my_strlen(str);
    int have_a = 0, have_t = 0, have_R = 0;
    for (size_t i = 0, j = 0; (int)i < size - 1; i++)
    {
        if (str[i + 1] == 'R')
        {
            if (!have_R)
            {
                string_to_return[j] = 'R';
                have_R = 1;
                j++;
            }
        }
        else if (str[i + 1] == 't')
        {
            if (!have_t)
            {
                string_to_return[j] = 't';
                have_t = 1;
                j++;
            }
        }
        else if (str[i + 1] == 'a')
        {
            if (!have_a)
            {
                string_to_return[j] = 'a';
                have_a = 1;
                j++;
            }
        }
        else
            return "err";
    }
    return string_to_return;
}
options *give_opt_Rat(const char *argv)
{
    options *opt = malloc(sizeof(options));
    my_int_memset((int *)opt, 0, 3);
    char str_to_return[my_strlen(argv)];
    my_char_memset(str_to_return, '\0', my_strlen(argv));
    char *option = argcase(argv, str_to_return);
    if (my_strcmp(option, "err") == 0)
    {
        free(opt);
        return NULL;
    }
    if (my_strstr(option, "R") != NULL) opt->is_R = 1;
    if (my_strstr(option, "a") != NULL) opt->is_a = 1;
    if (my_strstr(option, "t") != NULL) opt->is_t = 1;
    return opt;
}
int path_exist_and_isDir(const char *path)
{
    struct stat buf;
    int output = stat(path, &buf);
    if (output != 0)
        return 0;
    else
    {
        if (S_ISREG(buf.st_mode))
            return 2;
        else if (S_ISDIR(buf.st_mode))
            return 1;
        else
            return 0;
    }
}
char *list_files(const char *path, options *opt)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror(path);
        closedir(dir);
        return NULL;
    }
    struct dirent *crntdir;
    int i = 0, numfiles = 0, buffbytesize = 1;
    while ((crntdir = readdir(dir)) != NULL)
    {
        if (my_strlen(crntdir->d_name) > MAXSIZE)
        {
            fprintf(stderr, "\nFile's name exceeds allowed size of 256\n");
            closedir(dir);
            return NULL;
        }
        if (opt->is_a)
        {
            numfiles++;
            buffbytesize += my_strlen(crntdir->d_name);
        }
        else
        {
            if (*crntdir->d_name != '.')
            {
                numfiles++;
                buffbytesize += my_strlen(crntdir->d_name);
            }
        }
    }
    closedir(dir);
    dir = opendir(path);
    if (numfiles == 0)
    {
        char *buffer = (char *)malloc(sizeof(char));
        buffer[0] = '\0';
        closedir(dir);
        return buffer;
    }
    char *buffer = (char *)malloc(sizeof(char) * (buffbytesize + 2 * (numfiles - 1)));
    my_char_memset(buffer, '\0', buffbytesize + 2 * (numfiles - 1));
    char filename[numfiles][MAXSIZE];
    while ((crntdir = readdir(dir)) != NULL)
    {
        if (opt->is_a)
        {
            my_char_memset(filename[i], '\0', MAXSIZE);
            my_strcpy(filename[i], crntdir->d_name);
            i++;
        }
        else
        {
            if (*crntdir->d_name != '.')
            {
                my_char_memset(filename[i], '\0', MAXSIZE);
                my_strcpy(filename[i], crntdir->d_name);
                i++;
            }
        }
    }
    if (opt->is_t)
        sort_by_time(filename, numfiles, path);
    else
        sort_filenames(filename, numfiles);
    for (i = 0; i < numfiles - 1; i++)
    {
        my_strcat(buffer, filename[i]);
        my_strcat(buffer, "  ");
    }
    my_strcat(buffer, filename[numfiles - 1]);
    closedir(dir);
    return buffer;
}
void do_ls(char *path, options *opt)
{
    if (opt->is_R) printf("%s:\n", path);
    char *toprint = list_files(path, opt);
    if (toprint == NULL) return;
    printf("%s", toprint);
    free(toprint);
    if (opt->is_R)
    {
        DIR *dp = opendir(path);
        if (!dp)
        {
            perror(path);
            closedir(dp);
            return;
        }
        struct dirent *traversedirs;
        int numdirs = 0, i = 0;
        while ((traversedirs = readdir(dp)) != NULL)
        {
            if (traversedirs->d_type == 4 && my_strcmp(traversedirs->d_name, ".") && my_strcmp(traversedirs->d_name, ".."))
            {
                if (opt->is_a)
                    numdirs++;
                else
                {
                    if (*traversedirs->d_name != '.') numdirs++;
                }
            }
        }
        closedir(dp);
        DIR *dir = opendir(path);
        char newdir[my_strlen(path) + MAXSIZE];
        char dirs[numdirs][MAXSIZE];
        i = 0;
        while ((traversedirs = readdir(dir)) != NULL)
        {
            if (traversedirs->d_type == 4 && my_strcmp(traversedirs->d_name, ".") && my_strcmp(traversedirs->d_name, ".."))
            {
                if (opt->is_a)
                {
                    my_char_memset(dirs[i], '\0', MAXSIZE);
                    my_strcpy(dirs[i], traversedirs->d_name);
                    i++;
                }
                else
                {
                    if (*traversedirs->d_name != '.')
                    {
                        my_char_memset(dirs[i], '\0', MAXSIZE);
                        my_strcpy(dirs[i], traversedirs->d_name);
                        i++;
                    }
                }
            }
        }
        if (opt->is_t)
            sort_by_time(dirs, numdirs, path);
        else
            sort_filenames(dirs, numdirs);
        i = 0;
        while (i < numdirs)
        {
            sprintf(newdir, "%s/%s", path, dirs[i]);
            printf("\n\n");
            if (path_exist_and_isDir(newdir) == 2)
                printf("%s", newdir);
            else
                do_ls(newdir, opt);
            i++;
        }
        closedir(dir);
    }
}