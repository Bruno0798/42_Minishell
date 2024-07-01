/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 18:08:16 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/06/28 18:02:37 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char **ft_lst_to_arr(t_token *token) {
	int i;      /* Counter for the number of tokens and the current index in the array */
	char **arr; /* Array of strings representing the content of each token */
	t_token *tmp; /* Temporary pointer to the current token */

	if (!token)
		return NULL;

	i = 0;
	tmp = token;
	while (tmp) { /* Iterate over each token in the linked list */
		if (tmp->type == redir_out || tmp->type == redir_out2 || tmp->type == redir_in)
			break;
		i++;
		tmp = tmp->next;
	}

	arr = malloc(sizeof(char *) * (i + 1)); /* Allocate memory for the array of strings (including NULL terminator) */
	if (!arr)
		return NULL;

	tmp = token;
	i = 0;
	while (tmp) { /* Iterate over each token in the linked list again */
		if (tmp->type == redir_out || tmp->type == redir_out2 || tmp->type == redir_in)
			break;
		arr[i] = ft_strdup(tmp->content); /* Assign the content of the current token to the corresponding element in the array */
		if (!arr[i]) { /* If ft_strdup fails, free the already allocated memory */
			while (i > 0)
				free(arr[--i]);
			free(arr);
			return NULL;
		}
		i++;
		tmp = tmp->next;
	}
	arr[i] = NULL; /* Set the last element of the array to NULL */
	return arr; /* Return the array of strings */
}


void check_permissions(t_commands *command, char *string)
{
	int file;
	int execution;

	file = access(string, F_OK);
	execution = access(string, X_OK);

	if (!file && execution && ft_strchr(string, '/')) {
		ft_fprintf(2, EXECUTION_PERMISSION);
		g_exit_status = 126;
	} else if (!file && !execution && string[0] == '.' && string[1] == '/') {
		ft_fprintf(2, DIRECTORY_EXISTS);
		g_exit_status = 126;
	} else if (ft_strchr(string, '/') || !ft_get_value(command->env, "PATH")) {
		ft_fprintf(2, ERROR_DIR);
		g_exit_status = 127;
	} else {
		fprintf(stderr, "%s: %s\n", string, COMMAND_NOT_FOUND);
		g_exit_status = 127;
	}

	// Calculate the exit status
	free_all(command, 2);
	exit(g_exit_status);
}

void ft_token_addback(t_token **token, t_token *new)
{
	t_token *temp;

	temp = *token;
	if(token)
	{
		if (*token)
		{
			while (temp->next)
				temp = temp->next;
			temp->next = new;
		} else 
			*token = new;
	}
}
