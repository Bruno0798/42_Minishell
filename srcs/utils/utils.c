/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 18:08:16 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/06/20 04:25:16 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**ft_lst_to_arr(t_token *token)
{
	int		i;		/* Counter for the number of tokens and the current index in the array */
	char	**arr;	/* Array of strings representing the content of each token */
	t_token	*tmp;	/* Temporary pointer to the current token */

	i = 1;
	tmp = token;
	while (tmp)	/* Iterate over each token in the linked list */
	{
		if (tmp->type == redir_out || tmp->type == redir_out2
			|| tmp->type == redir_in)
			break ;
		i++;
		tmp = tmp->next;
	}
	arr = malloc(sizeof(char *) * i);	/* Allocate memory for the array of strings */
	tmp = token;
	i = 0;
	while (tmp)  /* Iterate over each token in the linked list again */
	{
		if (tmp->type == redir_out || tmp->type == redir_out2
			|| tmp->type == redir_in)
			break ;
		arr[i++] = ft_strdup(tmp->content);/* Assign the content of the current token to the corresponding element in the array */
		tmp = tmp->next;
	}
	arr[i] = NULL;  /* Set the last element of the array to NULL */
	return (arr);  /* Return the array of strings */
}

void	check_permissions(t_commands *command, char *string)
{
	int	file;
	int	execution;
	int	write;

	//return 0 on sucess. -1 on permission denied
	file = access(string, F_OK); //check if file exists
	execution = access(string, X_OK); //check for execution permission
	write = access(string, W_OK); //check for writing permission
	if (!file && execution)
		print_error(string, EXECUTION_PERMISSION, 1);
	else if (!file && !execution && string[0] == '.')
		print_error(string, DIRECTORY_EXISTS, 1);
	else if (ft_strchr(string, '/') || !ft_get_value(command->env, "PATH"))
		print_error(string, ERROR_DIR, 1);
	else
		ft_fprintf(2, "command not found\n", 1);
	// free_all(command, 2);
	EXIT_STATUS = 127 - ((!file && execution) || !write);
	free_all(command, 2);
	exit(EXIT_STATUS);
}
