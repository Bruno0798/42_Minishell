/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 18:04:33 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/22 15:09:08 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_expander(t_commands *command)
{
	t_token *token;
	char *value;

	token = command->token;
	
	while(token)
	{
		if (token->content[0] == '$')
		{
			value = ft_get_value(command->env, token->content + 1);
			free(token->content);
			token->content = ft_calloc(ft_strlen(value) + 1, sizeof(char));
			ft_strlcpy(token->content, value, ft_strlen(value) + 1);
		}
			token = token->next;
	}
}

char *ft_get_value(t_env *env, char *key)
{
	t_env *current;

	current = env;
	while(current)
	{
		if (!ft_strcmp(current->key, key))
			return current->value;
		current = current->next;
	}
	return NULL;
}