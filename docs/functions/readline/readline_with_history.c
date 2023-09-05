#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

/* we are not allowed to use history_list()
void print_history() {
    HIST_ENTRY **history_list = history_list();

    if (history_list) {
        int i;
        for (i = 0; history_list[i]; i++) {
            printf("%d: %s\n", i+1, history_list[i]->line);
        }
    } else {
        printf("No history.\n");
    }
}
*/

typedef struct		s_list 
{
	char						*line;
	struct s_list		*next;
}									t_list

t_list	*add_node(t_list *head, char *line)
{
	t_list *new_node = malloc(sizeof(t_list));
	new_node->line = strdup(line);
	new_node->next = NULL;
	if (head == NULL)
	{
		return new_node;
	}
	t_list	*current = head;
	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = new_node;
	return head;
}

void print_history(t_list *head)
{
	t_list *current = head;
	int count = 1;
	while (current != NULL)
	{
		printf("%d: %s\n", count++, current->line);
		current = current->next;
	}
}

int main() {
    char *line;
		t_list	history = NULL;
    using_history(); // probably not necessary

    while ((line = readline("$> ")) != NULL) {
        // If the user just pressed Enter, break the loop.
        if (line[0] == '\0') {
            free(line);
            break;
        }

        // Do something with the input.
        printf("You entered: %s\n", line);

        // Add the input to the command history.
        add_history(line);

        // Free the dynamically allocated input.
        free(line);
    }

    return 0;
}

