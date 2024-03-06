/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 18:04:33 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/03/05 15:27:50 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_expand_join(char *s1, char *s2, t_env *env);

void ft_expander(t_commands *command)
{
	t_token *c_token = command->token;
	char *new_content;
	char *key;

	while (c_token)
	{
		while (ft_strchr(c_token->content, '$'))
		{
			*ft_strchr(c_token->content, '$') = 0;
			c_token->content = ft_expand_join(c_token->content, ft_strchr(c_token->content, 0) + 1, command->env);
		}
		c_token = c_token->next;
	}
}

char	*ft_expand_join(char *s1, char *s2, t_env *env)
{
	char	*var;
	t_env	*tmp;
	int 	len;

	len = ft_strchr(s2, ' ') - s2;
	if(!ft_strchr(s2, ' '))
		len = ft_strchr(s2, DOUBLE_QUOTE) - s2;
	var = ft_substr(s2, 0, len);
	tmp = ft_fnd_env(env, var);
	free(var);
	if (tmp)
		var = ft_strdup(tmp->value);
	else
		var = ft_strdup("");
	var = ft_strjoin(var, s2 + len);
	var = ft_strjoin(s1, var);
	return var;
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
