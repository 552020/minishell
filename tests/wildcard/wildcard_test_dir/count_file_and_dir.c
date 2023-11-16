#include <stdio.h>
#include <dirent.h>

int main() {
    DIR *d;
    struct dirent *dir;
    int count = 0;

    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            count++;
        }
        closedir(d);
    }

    printf("There are %d files and directories in the current directory.\n", count);
    return 0;
}

