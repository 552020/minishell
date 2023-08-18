# dir functions

- opendir
- readdir
- closedir
- chdir

## opendir

`DIR *opendir(const char *dirname)` 

## readdir

`struct dirent *readdir(DIR *dirp);`

> The readdir() function returns a pointer to the next directory entry.  The directory entry remains valid until the next call to readdir() or closedir() on the same directory stream. The function returns NULL upon reaching the end of the directory or on error.  In the event of an error, errno may be set to any of the values documented for the getdirentries(2) system call.  Note that the order of the directory entries vended by readdir() is not specified. Some filesystems may return entries in lexicographic sort order and others may not.  Also note that not all filesystems will provide a value for d_type and may instead set the field to DT_UNKNOWN.



