#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc - 1 >= MAXARG)
    {
        fprintf(2, "xargs: too many arguments.\n");
        exit(1);
    }
    char lines[512];
    char *p = lines;
    char *q = p;
    char *x_argv[MAXARG] = {0};
    int x_argc = argc - 1;

    for (int i = 1; i < argc; ++i)
    {
        x_argv[i - 1] = argv[i];
    }
    while (read(0, p, sizeof(char)) == sizeof(char))
    {
        if (*p == '\n')
            *p = '\0';
        p++;
    }
    while (q < p)
    {
        if (fork() == 0)
        {
            p = q + strlen(q);
            for (char *ps = q; ps < p; ps++)
            {
                if (*ps == ' ')
                    *ps = '\0';
            }
            while (q < p)
            {
                x_argv[x_argc++] = q;
                q += strlen(q);
            }
            exec(x_argv[0], x_argv);
        }
        q += strlen(q) + 1;
    }
    wait(0);
    exit(0);
}
