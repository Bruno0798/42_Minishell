/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:39:10 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/30 12:20:26 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	check_and_initialize(char *str, t_env *env,
		t_commands **command, char ***words)
{
	if (is_everything_space(str))
		return (true);
	*command = malloc(sizeof(t_commands));
	(*command)->env = env;
	*words = ft_split2(str, SPACE);
	return (false);
}

bool	initialize_tokens(t_token **head, t_token **current, char **words)
{
	*head = ft_new_token(words[0], 1);
	if (*head == NULL)
		return (true);
	*current = *head;
	while ((*current)->next)
		*current = (*current)->next;
	return (false);
}

bool	add_tokens(t_token **current, char **words, int i)
{
	while (words[++i])
	{
		(*current)->next = ft_new_token(words[i], 1);
		if ((*current)->next == NULL)
			return (true);
		*current = (*current)->next;
		while ((*current)->next)
			*current = (*current)->next;
	}
	return (false);
}

void	update_command_tokens(t_token const *head)
{
	while (head)
	{
		if (head->next && head->type != 1 && head->type != 2 && head->type != 3)
			head->next->type = 3;
		head = head->next;
	}
}

t_commands	*ft_new_commands(char *str, t_env *env)
{
	t_commands	*command;
	t_token		*head;
	t_token		*current;
	char		**words;
	int			i;

	set_null(&command, &head, &current, &words);
	if (check_and_initialize(str, env, &command, &words))
		return (NULL);
	if (initialize_tokens(&head, &current, words))
	{
		cleanup(command, str, words);
		return (NULL);
	}
	i = 0;
	if (add_tokens(&current, words, i))
	{
		cleanup(command, str, words);
		return (NULL);
	}
	command->token = head;
	command->next = NULL;
	cleanup(NULL, str, words);
	update_command_tokens(head);
	return (command);
}
