#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 1024

int fd = -1;
char current_file[256] = "";

void create_open_file()
{
    char filename[256];

    printf("\nEnter file name: ");
    scanf("%255s", filename);

    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }

    fd = open(filename, O_RDWR | O_CREAT, 0644);

    if (fd == -1)
    {
        perror("Error opening file");
        return;
    }

    strcpy(current_file, filename);

    printf("File opened successfully!\n");
    printf("File Descriptor: %d\n", fd);
}

void read_file()
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    if (fd == -1)
    {
        printf("No file is currently open.\n");
        return;
    }

    /* Start reading from the beginning */
    if (lseek(fd, 0, SEEK_SET) == (off_t)-1)
    {
        perror("Error moving file pointer");
        return;
    }

    bytes_read = read(fd, buffer, BUFFER_SIZE - 1);

    if (bytes_read == -1)
    {
        perror("Error reading file");
        return;
    }

    buffer[bytes_read] = '\0';

    printf("\n========== FILE CONTENT ==========\n");

    if (bytes_read == 0)
        printf("File is empty.\n");
    else
        printf("%s", buffer);

    printf("\n==================================\n");
    printf("Bytes read: %ld\n", (long)bytes_read);
}

void write_file()
{
    char data[BUFFER_SIZE];
    ssize_t bytes_written;

    if (fd == -1)
    {
        printf("No file is currently open.\n");
        return;
    }

    printf("\nEnter data to write: ");
    getchar();
    fgets(data, BUFFER_SIZE, stdin);

    /*
     * Move to the beginning before writing.
     * This demonstrates lseek().
     */
    if (lseek(fd, 0, SEEK_SET) == (off_t)-1)
    {
        perror("Error moving file pointer");
        return;
    }

    bytes_written = write(fd, data, strlen(data));

    if (bytes_written == -1)
    {
        perror("Error writing to file");
        return;
    }

    printf("Data written successfully!\n");
    printf("Bytes written: %ld\n", (long)bytes_written);
}

void append_file()
{
    int append_fd;
    char data[BUFFER_SIZE];
    ssize_t bytes_written;

    if (strlen(current_file) == 0)
    {
        printf("No file has been opened yet.\n");
        return;
    }

    append_fd = open(current_file, O_WRONLY | O_APPEND);

    if (append_fd == -1)
    {
        perror("Error opening file for append");
        return;
    }

    printf("\nEnter data to append: ");
    getchar();
    fgets(data, BUFFER_SIZE, stdin);

    bytes_written = write(append_fd, data, strlen(data));

    if (bytes_written == -1)
    {
        perror("Error appending to file");
        close(append_fd);
        return;
    }

    close(append_fd);

    printf("Data appended successfully!\n");
    printf("Bytes appended: %ld\n", (long)bytes_written);
}

void move_file_pointer()
{
    off_t offset;
    off_t result;
    int choice;

    if (fd == -1)
    {
        printf("No file is currently open.\n");
        return;
    }

    printf("\nEnter offset: ");
    scanf("%ld", &offset);

    printf("\n1. Beginning of file (SEEK_SET)\n");
    printf("2. Current position (SEEK_CUR)\n");
    printf("3. End of file (SEEK_END)\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            result = lseek(fd, offset, SEEK_SET);
            break;

        case 2:
            result = lseek(fd, offset, SEEK_CUR);
            break;

        case 3:
            result = lseek(fd, offset, SEEK_END);
            break;

        default:
            printf("Invalid choice.\n");
            return;
    }

    if (result == (off_t)-1)
    {
        perror("Error moving file pointer");
        return;
    }

    printf("File pointer moved successfully!\n");
    printf("Current position: %ld\n", (long)result);
}

void display_file_info()
{
    struct stat file_info;

    if (strlen(current_file) == 0)
    {
        printf("No file has been opened yet.\n");
        return;
    }

    if (stat(current_file, &file_info) == -1)
    {
        perror("Error getting file information");
        return;
    }

    printf("\n========== FILE INFORMATION ==========\n");
    printf("File name   : %s\n", current_file);
    printf("File size   : %ld bytes\n", (long)file_info.st_size);
    printf("Permissions : %o\n", file_info.st_mode & 0777);

    printf("File type   : ");

    if (S_ISREG(file_info.st_mode))
        printf("Regular file\n");
    else if (S_ISDIR(file_info.st_mode))
        printf("Directory\n");
    else
        printf("Other\n");

    printf("======================================\n");
}

void close_file()
{
    if (fd == -1)
    {
        printf("No file is currently open.\n");
        return;
    }

    if (close(fd) == -1)
    {
        perror("Error closing file");
        return;
    }

    fd = -1;

    printf("File closed successfully!\n");
}

int main()
{
    int choice;

    printf("============================================\n");
    printf(" Linux File Descriptor and File I/O System\n");
    printf("============================================\n");

    while (1)
    {
        printf("\n============== MENU ==============\n");
        printf("1. Create / Open File\n");
        printf("2. Read File\n");
        printf("3. Write File\n");
        printf("4. Append to File\n");
        printf("5. Move File Pointer\n");
        printf("6. Display File Information\n");
        printf("7. Close File\n");
        printf("8. Exit\n");
        printf("==================================\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                create_open_file();
                break;

            case 2:
                read_file();
                break;

            case 3:
                write_file();
                break;

            case 4:
                append_file();
                break;

            case 5:
                move_file_pointer();
                break;

            case 6:
                display_file_info();
                break;

            case 7:
                close_file();
                break;

            case 8:
                if (fd != -1)
                    close(fd);

                printf("\nThank you for using the system!\n");
                return 0;

            default:
                printf("Invalid choice. Please enter 1-8.\n");
        }
    }

    return 0;
}