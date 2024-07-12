/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 18:08:16 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 07:54:49 by bruno            ###   ########.fr       */
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
	i = 0;
	tmp = token;
	while (tmp)
	{
		if (tmp->type == redir_out || tmp->type == redir_out2 || tmp->type == redir_in)
			break ;
		i++;
		tmp = tmp->next;
	}
	arr = malloc(sizeof(char *) * (i + 1));
	if (!arr)
		return (NULL);
	tmp = token;
	i = 0;
	while (tmp)
	{
		if (tmp->type == redir_out || tmp->type == redir_out2 || tmp->type == redir_in)
			break ;
		arr[i] = ft_strdup(tmp->content);
		if (!arr[i])
		{
			while (i > 0)
				free(arr[--i]);
			free(arr);
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
