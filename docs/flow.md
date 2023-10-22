# Data Flow

Our shell processes user commands through a sequential four-stage pipeline: the Tokenizer breaks down the collected input into individual tokens; the Lexer categorizes these tokens into recognized language constructs; the Parser evaluates and builds a structured representation of these constructs; and the Executor takes this structure to perform the actual system commands. This document details the data flow through these stages.

## Tokenizer

The tokenizer accepts two primary inputs:

- A string (char \*) representing the user's raw command, which is the real input for the tokenizer.
- An uninitialized array of t_token structs. The length of this array is predetermined by a separate function that counts the anticipated number of tokens in the input string. We outsourced this operation to better fullfill the 25 lines per function limitation.

Each t_token struct houses two fields:

- A t_token_type (an enum) indicating the token's category or type.
- A string capturing the actual value or content of the token.

```c
typedef struct s_token
{
t_token_type type;
char \*str;
} t_token;
```

### Output

The tokenizer not only dissects the user's input into distinct tokens but also initiates a preliminary lexing process. As a result, each element in the returned t_token array is stamped with a specific t_token_type enum value, reflecting the initial interpretation of that token's role or significance. The possible types include:

T_WORD: Represents commands, arguments, or filenames. This broad category might include executable commands, parameters passed to commands, or file paths.

T_PIPE (|): Signifies a pipe operation, enabling the output of one command to be used as the input for another.

T_REDIRECT_IN (<) and T_REDIRECT_OUT (>): Denote input and output redirection, respectively.

T_REDIRECT_APPEND (>>): Implies output redirection, but with the intent to append rather than overwrite.

T_HEREDOC (<<): Begins a "here document", a section of a shell script that is treated as input to a command.

T_HEREDOC_DELIMITER: Signifies the delimiter that ends a "here document".

T_DOUBLE_QUOTE ("..."): Captures strings wrapped in double quotes, including the quotes themselves.

T_SINGLE_QUOTE ('...'): Captures strings wrapped in single quotes, including the quotes themselves.

T_ENV_VAR ($VAR_NAME): Represents environment variables accessed with a preceding $.

T_END: A special token marking the end of the token array, serving as a sentinel.

The Lexer will later finalize the lexing process, further refining and possibly expanding upon these initial token categorizations, while also performing expansion operations.

This enhanced output section provides a clearer idea of the tokenizer's output while setting the stage for the Lexer's subsequent processing.

Regenerate

````

```

```
````
