#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define RD 0
#define WR 1
int main(int argc, char **argv)
{
    char b = 'p';
    int pid = fork();
    int p2c[2];
    int c2p[2];
    pipe(p2c);
    pipe(c2p);
    if (pid == 0)
    {
        close(p2c[WR]);
        close(c2p[RD]);
        read(p2c[RD], &b, 1);
        printf("%d: received ping\n", getpid());
        close(p2c[RD]);
        write(c2p[WR], &b, 1);
        close(c2p[WR]);
    }
    else
    {
        close(p2c[RD]);
        close(c2p[WR]);
        write(p2c[WR], &b, 1);
        close(p2c[WR]);
        read(c2p[RD], &b, 1);
        printf("%d: received pong\n", getpid());
        close(c2p[RD]);
    }
    exit(0);
}