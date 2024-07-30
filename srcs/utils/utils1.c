/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 21:14:51 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 11:44:32 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_pipes(char const *str)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (str[++i])
		if (str[i] == '|')
			count++;
	return (count);
}

t_commands	*pipe_commands(char *str, t_env *env)
{
	t_commands	*command;
	t_commands	*current;
	char		**splitted_command;
	int			i;

	i = 0;
	splitted_command = ft_split2(str, '|');
	command = ft_new_commands(splitted_command[0], env);
	if (command == NULL)
	{
		free(splitted_command);
		return (NULL);
	}
	current = command;
	while (splitted_command[++i])
	{
		current->next = ft_new_commands(splitted_command[i], env);
		if (command->next == NULL)
			return (NULL);
		current = current->next;
	}
	free(splitted_command);
	return (command);
}

t_token	*ft_new_token(char *str, int flag)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->content = str;
	token->type = ft_token_type(str);
	token->next = NULL;
	if (ft_strpbrk(str, "<>") && token->type == command && flag)
		token = check_redir_syntax(token);
	return (token);
}
