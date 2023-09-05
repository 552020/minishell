# access

- `int access(const char *path, int amode)`
- path: the path of the file or directory you want to check;
- the access permissions you want to check `F_OK`, `R_OK`, `W_OK`, `X_OK`. Existence, read, write, execute/search.
- It return 0 on success, -1 otherwise and it sets errno.
- It checks the permissions of a file. 
- #include <unistd.h>

