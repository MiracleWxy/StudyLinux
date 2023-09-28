#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    // 定义FIFO的文件路径
    char *fifo_path = "/tmp/myfifo";

    // 创建FIFO文件
    mkfifo(fifo_path, 0666);

    // 创建子进程
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // 子进程用于写FIFO
        int fd_write = open(fifo_path, O_WRONLY);
        if (fd_write == -1) {
            perror("open");
            exit(1);
        }

        // 写入数据到FIFO
        char *message = "Hello, FIFO!";
        write(fd_write, message, strlen(message));

        close(fd_write);
    } else {
        // 父进程用于读FIFO
        int fd_read = open(fifo_path, O_RDONLY);
        if (fd_read == -1) {
            perror("open");
            exit(1);
        }

        char buffer[256];
        int bytes_read = read(fd_read, buffer, sizeof(buffer));
        if (bytes_read == -1) {
            perror("read");
            exit(1);
        }

        buffer[bytes_read] = '\0';
        printf("父进程读取到的数据: %s\n", buffer);

        close(fd_read);

        // 删除FIFO文件
        unlink(fifo_path);
    }

    return 0;
}

