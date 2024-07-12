/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 21:14:51 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 07:54:10 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_pipes(char *str)
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

t_commands	*ft_new_commands(char *str, t_env *env)
{
	t_commands	*command;
	t_token		*head;
	t_token		*current;
	char		**words;
	int			i;

	i = 0;
	if (is_everything_space(str))
		return (NULL);
	command = malloc(sizeof(t_commands));
	command->env = env;
	words = ft_split2(str, SPACE);
	head = ft_new_token(words[0], 1);
	if (head == NULL)
	{
		free(str);
		free(words);
		return (NULL);
	}
	current = head;
	while (current->next)
		current = current->next;
	while (words[++i])
	{
		current->next = ft_new_token(words[i], 1);
		if (current->next == NULL)
		{
			free(str);
			free(words);
			return (NULL);
		}
		current = current->next;
		while (current->next)
			current = current->next;
	}
	command->token = head;
	command->next = NULL;
	free(str);
	free(words);
	while (head)
	{
		if (head->next && head->type != 1 && head->type != 2 && head->type != 3)
			head->next->type = 3;
		head = head->next;
	}
	return (command);
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
	if ((ft_strchr(str, '<') || ft_strchr(str, '>')) && token->type == command && flag)
		token = check_redir_syntax(token);
	return (token);
}
