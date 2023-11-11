#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    DIR *dir;
    struct dirent *entry;
    struct stat entry_stat;
    char *path = argv[1];

    dir = opendir(path);
    if (dir == NULL) {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char fullpath[1024];
        printf("Name: %s, Type: ", entry->d_name);
        
        // Construct the full path
        sprintf(fullpath, "%s/%s", path, entry->d_name);

        if (lstat(fullpath, &entry_stat) == -1) {
            perror("lstat");
            continue;
        }

        mode_t type = entry_stat.st_mode & S_IFMT;
        if (type == S_IFREG) {
            printf("Regular File\n");
        } else if (type == S_IFDIR) {
            printf("Directory\n");
        } else if (type == S_IFLNK) {
            printf("Symbolic Link\n");
        } else if (type == S_IFCHR) {
            printf("Character Device\n");
        } else if (type == S_IFBLK) {
            printf("Block Device\n");
        } else if (type == S_IFIFO) {
            printf("FIFO/Named Pipe\n");
        } else if (type == S_IFSOCK) {
            printf("Socket\n");
        } else {
            printf("Unknown\n");
        }
    }

    closedir(dir);
    return 0;
}

