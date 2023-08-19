#include "minishell.h"

int ft_isspace(int c) {
  // ' ' (space) is 32 in ASCII
  // '\t' (horizontal tab) is 9 in ASCII
  // '\n' (newline) is 10 in ASCII
  // '\v' (vertical tab) is 11 in ASCII
  // '\f' (form feed) is 12 in ASCII
  // '\r' (carriage return) is 13 in ASCII

  if (c == 32 || (c >= 9 && c <= 13))
    return (1); // Indicates success (it's a whitespace)
  else
    return (0); // Indicates failure (it's not a whitespace)
}
