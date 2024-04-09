/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:42:09 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/04/09 16:52:39 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
t_env *dup_list(t_env *env)
{
	t_env   *dup;
	t_env   *head;
	t_env   *current;
	head = malloc(sizeof(t_env));
	current = env;
	dup = head;
	dup->key = ft_strdup(current->key);
	dup->value = ft_strdup(current->value);
	while(current->next)
	{
		current = current->next;
		dup->next = malloc(sizeof(t_env));
		dup = dup->next;
		dup->key = ft_strdup(current->key);
		dup->value = ft_strdup(current->value);
	}
	return head;
}
t_env *order_list(t_env *env)
{
	t_env *current;
	t_env *tmp;
	char *key;
	char *value;
	current = env;
	while(current->next)
	{
		tmp = current->next;
		while(tmp)
		{
			if(ft_strcmp(current->key, tmp->key) > 0)
			{
				key = current->key;
				value = current->value;
				current->key = tmp->key;
				current->value = tmp->value;
				tmp->key = key;
				tmp->value = value;
			}
			tmp = tmp->next;
		}
		current = current->next;
	}
	return env;
}
void print_env(t_env *env)
{
	t_env *current;
	t_env   *dup;
	dup = dup_list(env);
	current = order_list(dup);
	while(current)
	{
		printf("declare -x %s=\"%s\"\n", current->key, current->value);
		current = current->next;
	}
	free_env(dup);
}
void ft_export(t_commands *command)
{
	char    *word;

	if(!command->token->next) /* If there is no token to export, print the environment variables */
		print_env(command->env);
	else if(command->token->next->content[0] == '-')
	{
		dup2(STDERR_FILENO,STDOUT_FILENO);
		printf("minishell: %s: invalid option\n", command->token->next->content);
	}
	else if (ft_fnd_env(command->env, *ft_split(command->token->next->content, '='))) /* If the key already exists, update the value */
	{
		word = ft_strchr(command->token->next->content, '=') + 1; /* Get the value */
		*ft_strchr(command->token->next->content, '=') = 0; /* Set the equal sign to 0 */
		ft_update_env(command->env, command->token->next->content, word); /* Update the environment variable */
	}
	else if(ft_strchr(command->token->next->content, '=')) /* If the token has an equal sign, add the environment variable */
	{
		if (ft_isdigit(command->token->next->content[0]) || ft_hasSpecialChar(*ft_split(command->token->next->content, '=')))
		{
			printf("export: not valid in this context: %s\n", command->token->next->content);
			return ;
		}
		word = ft_strchr(command->token->next->content, '=') + 1;
		*ft_strchr(command->token->next->content, '=') = 0;
		if(!*(command->token->next->content))
			printf("Error\n");
		ft_add_env_back(&command->env, ft_new_env(command->token->next->content, word));
	} else
	{
		if (ft_isdigit(command->token->next->content[0]) || ft_hasSpecialChar(*ft_split(command->token->next->content, '=')))
		{
			printf("export: not valid in this context: %s\n", command->token->next->content);
			return ;
		}
		ft_add_env_back(&command->env,ft_new_env(command->token->next->content, ""));
	}
}
