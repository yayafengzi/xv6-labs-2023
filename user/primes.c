#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define RD 0
#define WR 1
int main(int argc, char **argv)
{
    int *pwrite = (int *)malloc(sizeof(int) * 2);
    pipe(pwrite);

    int pid = fork();
    if (pid)
    {
        close(pwrite[RD]);
        printf("prime 2\n");
        for (int i = 3; i <= 35; i++)
        {
            write(pwrite[WR], &i, sizeof(i));
            // printf("write %d ", i);
        }
        close(pwrite[WR]);
    }
    else
    {
        int *pread;
        pread = pwrite;
        close(pread[WR]);
        int num;
        int factor = 2;
        int alloced = 0;
        while (read(pread[RD], &num, sizeof(num)))
        {

            // printf("read %d ", num);
            if (num % factor)
            {
                if (!pid)
                {
                    pwrite = (int *)malloc(sizeof(int) * 2);
                    alloced = 1;
                    pipe(pwrite);
                    pid = fork();
                    if (pid)
                    {
                        close(pwrite[RD]);
                        printf("prime %d\n", num);
                    }
                    else
                    {
                        alloced = 0;
                        free(pread);
                        pread = pwrite;
                        close(pread[WR]);
                        factor = num;
                    }
                }
                else
                {
                    write(pwrite[WR], &num, sizeof(num));
                    // printf("write %d ", num);
                }
            }
        }
        close(pread[RD]);
        close(pwrite[WR]);
        if (alloced)
            free(pread);
    }

    wait(0);
    free(pwrite);
    exit(0);
}