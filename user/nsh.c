#include "../kernel/types.h"
#include "user.h"
#include "../kernel/fcntl.h"

#define MAXARGS 10
#define MAXWORD 30

void execPipe(char* argv[],int argc);

// from sh.c
// 获得用户输入的命令
int getcmd(char *buf, int nbuf)
{
    fprintf(2, "@ ");
    memset(buf, 0, nbuf);
    gets(buf, nbuf);
    if (buf[0] == 0) // EOF
        return -1;
    return 0;
}

char args[MAXARGS][MAXWORD];
char whitespace[] = " \t\r\n\v";
 
void setargs(char* cmd, char* argv[], int* argc)
{
    for(int i = 0; i < MAXARGS; i++){
        argv[i] = &args[i][0];
    }
    int i = 0; // 表示第i个word
    // 切分word
    for (int j = 0; cmd[j] != '\n' && cmd[j] != '\0'; j++)
    {
        // 跳过之前的空格
        while (strchr(whitespace, cmd[j]))
        {
            j++;
        }
        argv[i++] = cmd + j;
        // 找到下一个空格
        while (!strchr(whitespace, cmd[j]))
        {
            j++;
        }
        cmd[j]='\0';
    }
    argv[i] = 0;
    *argc = i;
}

void runcmd(char*argv[], int argc)
{
    for(int i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i], "|"))
        {
            execPipe(argv, argc);
        }
    }
     
    for(int i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i], ">"))
        {
            close(1);
            open(argv[i+1], O_CREATE|O_WRONLY);
            argv[i] = 0;
        }
        if(!strcmp(argv[i], "<"))
        {
            close(0);
            open(argv[i+1], O_RDONLY);
            argv[i] = 0;
        }
    }
    exec(argv[0], argv);
}
 
void execPipe(char* argv[], int argc)
{
    int i = 0;
    for(; i < argc; i++){
        if(!strcmp(argv[i], "|"))
        {
            argv[i] = 0;
            break;
        }
    }
    
    int fd[2];
    pipe(fd);
    if(!fork())
    {
        // 子进程执行左边的命令 把自己的标准输出关闭
        close(1);
        dup(fd[1]);
        close(fd[0]);
        close(fd[1]);
        runcmd(argv, i);
    } else
    {
        // 父进程执行右边的命令 把自己的标准输入关闭
        close(0);
        dup(fd[0]);
        close(fd[0]);
        close(fd[1]);
        runcmd(argv+i+1, argc-i-1);
    }
}

int main()
{
    char buf[100];
    
    while (getcmd(buf, sizeof(buf)) >= 0)
    {
        if (!fork())
        {
            char* argv[MAXARGS];
            int argc = -1;
            setargs(buf, argv, &argc);
            runcmd(argv, argc);
        }
        wait(0);
    }
 
    exit(0);
}