/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 19:41:35 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/08 19:41:35 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**ft_env_to_arr(t_env *env)
{
	char	**arr;
	char	*temp;
	t_env	*tmp;
	int		i;

	arr = malloc(sizeof(char *) * env_size(env));
	tmp = env;
	i = 0;
	while (tmp)
	{
		temp = ft_strjoin(tmp->key, "=");
		arr[i] = ft_strjoin(temp, tmp->value);
		free(temp);
		tmp = tmp->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

int	env_size(t_env *env)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (++i);
}

void	ft_update_env(t_env *env_lst, char *key, char *replace, int visible)
{
	t_env	*curr;

	curr = env_lst;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			free(curr->value);
			curr->value = ft_strdup(replace);
			curr->visible = visible;
			return ;
		}
		curr = curr->next;
	}
}

t_env	*ft_fnd_env(t_env *env, char *search)
{
	t_env	*current;

	current = env;
	if (!search)
		return (NULL);
	while (current)
	{
		if (!ft_strcmp(current->key, search))
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	free_double_pointer_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
