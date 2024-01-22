/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 18:04:33 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/22 18:00:41 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_expander(t_commands *command)
{
	t_token *token;
	char *value;
	char	*str;

	token = command->token;
	while(token)
	{
		str = ft_strtrim(token->content, "\"");
		if (str[0] == '$')
		{
			value = ft_get_value(command->env, str + 1);
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