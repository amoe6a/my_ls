#include <stdio.h>
#include "my_ls.h"
int main(int argc, char **argv)
{
    int num_opts = 0, num_files = 0;
    options opt_total = {0, 0, 0};
    char files[argc - 1][MAXSIZE];
    char regfiles[argc - 1][MAXSIZE];
    int num_reg_files = 0, i = 1, f_indx = 0;
    while (argv[i])
    {
        if (*argv[i] == '-')
        {
            options *tmp;
            tmp = give_opt_Rat(argv[i]);
            if (tmp == NULL)
            {
                free(tmp);
                errmsg();
                return 0;
            }
            opt_total.is_R += tmp->is_R;
            opt_total.is_a += tmp->is_a;
            opt_total.is_t += tmp->is_t;
            num_opts++;
            free(tmp);
        }
        else
        {
            if (path_exist_and_isDir(argv[i]) == 0)
                printf("my_ls: cannot access '%s': No such file or directory\n", argv[i]);
            else if (path_exist_and_isDir(argv[i]) == 2)
            {
                my_char_memset(regfiles[num_reg_files], '\0', MAXSIZE);
                my_strcpy(regfiles[num_reg_files], argv[i]);
                num_reg_files++;
            }
            else
            {
                my_char_memset(files[f_indx], '\0', MAXSIZE);
                my_strcpy(files[f_indx], argv[i]);
                num_files++;
                f_indx++;
            }
        }
        i++;
    }
    if (!num_files && !num_opts) return 0;
    if (opt_total.is_t)
    {
        sort_by_time(regfiles, num_reg_files, ".");
        sort_by_time(files, num_files, ".");
    }
    else
    {
        sort_filenames(regfiles, num_reg_files);
        sort_filenames(files, num_files);
    }
    for (i = 0; i < num_reg_files - 1; i++) printf("%s  ", regfiles[i]);
    if (num_reg_files > 0) printf("%s\n", regfiles[num_reg_files - 1]);
    if (num_files == 0)
    {
        if (num_opts) do_ls(".", &opt_total);
        return 0;
    }
    if (num_reg_files > 0) printf("\n");
    for (i = 0; i < num_files - 1; i++)
    {
        if (num_files > 1 && !opt_total.is_R) printf("%s:\n", files[i]);
        do_ls(files[i], &opt_total);
        printf("\n\n");
    }
    if ((num_files > 1 && !opt_total.is_R) || (num_reg_files > 0 && !opt_total.is_R))
        printf("%s:\n", files[i]);
    do_ls(files[num_files - 1], &opt_total);
    return 0;
}