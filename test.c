#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main() {
    int fd[2];
    char buffer[20];
    // 创建管道
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    // 创建子进程
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {  // 子进程
        close(fd[0]);  // 关闭读端

        const char* message = "Hello from child process!";
        write(fd[1], message, strlen(message) + 1);
        close(fd[1]);

        return 0;
    } else {  // 父进程
        close(fd[1]);  // 关闭写端

        read(fd[0], buffer, sizeof(buffer));
        printf("Received message from child: %s\n", buffer);
        close(fd[0]);

        return 0;
    }
}
