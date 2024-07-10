/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:42:09 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/10 17:56:46 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_export(t_commands *command)
{
	t_token	*token;

	token = command->token;
	if (!token->next)
	{
		print_env(command->env);
		return ;
	}
	while (token->next)
	{
		token = token->next;
		handle_export_token(command, token);
	}
}

void	print_env(t_env *env)
{
	t_env	*current;
	t_env	*dup;

	dup = dup_list(env);
	current = order_list(dup);
	while (current)
	{
		if (current->visible != 3)
		{
			if (current->visible == 1)
				printf("declare -x %s=\"%s\"\n", current->key, current->value);
			else
				printf("declare -x %s\n", current->key);
		}
		current = current->next;
	}
	free_env(dup);
}

bool	validate_and_handle_key(char **key, char *equal_sign, t_token *token)
{
	if (!is_invalid_token(*key))
	{
		if (equal_sign)
			*equal_sign = '=';
		else
			free(*key);
		print_error(ERROR_EXP, token->content, 1);
		return (false);
	}
	return (true);
}

void	handle_env_update(t_commands *command, char *key, char *equal_sign)
{
	char	*value;

	if (equal_sign)
	{
		value = equal_sign + 1;
		update_or_add_env(command, key, value);
		*equal_sign = '=';
	}
	else
	{
		if (!ft_fnd_env(command->env, key))
			ft_add_env_back(&command->env, ft_new_env(key, "", 0));
	}
}

t_env	*dup_list(t_env *env)
{
	t_env	*dup;
	t_env	*head;
	t_env	*current;
	t_env	*new_node;

	head = NULL;
	current = env;
	while (current != NULL)
	{
		new_node = malloc(sizeof(t_env));
		if (new_node == NULL)
			return (NULL);
		new_node->key = ft_strdup(current->key);
		new_node->value = ft_strdup(current->value);
		new_node->visible = current->visible;
		new_node->next = NULL;
		if (head == NULL)
			head = new_node;
		else
			dup->next = new_node;
		current = current->next;
		dup = new_node;
	}
	return (head);
}
