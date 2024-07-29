/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 18:08:16 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/29 18:42:36 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**ft_lst_to_arr(t_token *token)
{
	int		i;
	char	**arr;
	t_token	*tmp;

	if (!token)
		return (NULL);
	i = ft_token_size(token);
	arr = malloc(sizeof(char *) * (i + 1));
	if (!arr)
		return (NULL);
	tmp = token;
	i = 0;
	while (tmp)
	{
		arr[i] = ft_strdup(tmp->content);
		if (!arr[i])
		{
			free_double_pointer_array(arr);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	arr[i] = (NULL);
	return (arr);
}

void	check_permissions(t_commands *command, char *string)
{
	int	file;
	int	execution;

	file = access(string, F_OK);
	execution = access(string, X_OK);
	if (!file && execution && ft_strchr(string, '/'))
		print_error(EXECUTION_PERMISSION, NULL, 126);
	else if (!file && !execution && string[0] != '.')
		print_error(DIRECTORY_EXISTS, NULL, 126);
	else if (ft_strchr(string, '/') || !ft_get_value(command->env, "PATH"))
		print_error(ERROR_DIR, NULL, 127);
	else
		print_error(COMMAND_NOT_FOUND, string, 127);
	free_all(command, 2);
	exit(g_exit_status);
}

bool	ft_has_special_char(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (true);
	return (false);
}

void	ft_remove_quotes(t_commands *commands)
{
	t_token		*curr;
	t_commands	*head;

	head = commands;
	while (head != NULL)
	{
		curr = head->token;
		while (curr)
		{
			if ((curr->type == command || curr->type == files)
				&& is_between_quotes(curr->content))
				curr->content = ft_delete_quotes(curr->content);
			curr = curr->next;
		}
		head = head->next;
	}
}

int	count_commands(t_commands *command)
{
	int			count;
	t_commands	*token;

	count = 0;
	token = command;
	while (token)
	{
		count++;
		token = token->next;
	}
	return (count);
}
