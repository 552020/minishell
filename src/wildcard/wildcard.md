# build_pattern

This function will build a "pattern" from a string containing a wildcard. Example: from a string like "cat file\_.txt",
it will build a pattern like `file*.txt`.
The arguments are:

    - asterisk: a pointer to the first asterisk in the string. Example: `file*.txt`
    -> `*`

    - input_start: a pointer to the beginning of the string. Example: `cat file*.txt`
    -> `c`

    - pattern_ptr: a pointer to a t_pattern variable. This variable will be filled with the pattern. We use this struct not only in this function but also in the `get_matching_entries` function.

The return value is void but actually the point of the function is to fill the t_pattern variable,
the pattern field, which is the whole pattern like "file*.txt". The suffix,
prefix and midfixes fields are also filled. The prefix in this example is "file" and the suffix is ".txt". The midfixes are the strings between the asterisks. In this example,
there are no midfixes. Examples of midfixes in a pattern like "file*hello*world*.txt*",
"hello" and "world". The reason to distinguish between them is that we can check prefix and suffix with ft_strncmp,
but we need to check midfixes with ft_strstr.
*/
