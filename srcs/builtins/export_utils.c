/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 18:01:13 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/30 16:52:41 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	swap_nodes(t_env *a, t_env *b)
{
	char	*key;
	char	*value;
	int		visible;

	key = a->key;
	value = a->value;
	visible = a->visible;
	a->key = b->key;
	a->value = b->value;
	a->visible = b->visible;
	b->key = key;
	b->value = value;
	b->visible = visible;
}

bool	is_invalid_token(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[0]) && ft_isdigit(str[0]))
		return (false);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_'
			&& str[i] != '=')
			return (false);
		i++;
	}
	return (true);
}

void	update_or_add_env(t_commands *command, char *key, char *value)
{
	if (ft_fnd_env(command->env, key))
		ft_update_env(command->env, key, value, 1);
	else
		ft_add_env_back(&command->env, ft_new_env(key, value, 1));
}

t_env	*order_list(t_env *env)
{
	t_env	*current;
	t_env	*tmp;

	current = env;
	while (current && current->next)
	{
		tmp = current->next;
		while (tmp)
		{
			if (ft_strcmp(current->key, tmp->key) > 0)
				swap_nodes(current, tmp);
			tmp = tmp->next;
		}
		current = current->next;
	}
	return (env);
}
