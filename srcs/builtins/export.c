/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:42:09 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/05/27 19:27:15 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env *dup_list(t_env *env)
{
	t_env   *dup;
	t_env   *head;
	t_env   *current;

	head = NULL; // Initialize to NULL
	current = env;
	while (current != NULL)
	{
		t_env *new_node = malloc(sizeof(t_env));
		if (new_node == NULL)
		{
			// Handle memory allocation failure
			// You may want to free allocated memory before returning NULL
			return NULL;
		}
		new_node->key = ft_strdup(current->key);
		new_node->value = ft_strdup(current->value);
		new_node->visible = current->visible; // Copy the visibility flag
		new_node->next = NULL;
		if (head == NULL)
			head = new_node;
		else
			dup->next = new_node;
		current = current->next;
		dup = new_node;
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
	while (current)
	{
		printf("declare -x %s=\"%s\"\n",current->key, current->value);
		current = current->next;
	}
	free_env(dup);
}

void ft_export(t_commands *command)
{
	char    *word;
	t_token	*token;

	token = command->token;
	if(!command->token->next) /* If there is no token to export, print the environment variables */
		print_env(command->env);
	else
		while(token->next)
		{
			token = token->next;
			if(token->content[0] == '-')
			{
				//dup2(STDERR_FILENO,STDOUT_FILENO);
				print_error(ERROR_OPTIONS, token->content, 1);
			}
			else if (!ft_strcmp(token->content, "") || !ft_strcmp(token->content, "="))
				printf("export: not valid in this context: %s\n", token->content);
			else if (ft_fnd_env(command->env, *ft_split(token->content, '='))) /* If the key already exists, update the value */
			{
				if(!ft_strchr(token->content, '='))
					continue;
				word = ft_strchr(token->content, '=') + 1; /* Get the value */
				*ft_strchr(token->content, '=') = 0; /* Set the equal sign to 0 */
				ft_update_env(command->env, token->content, word, 1); /* Update the environment variable */
			}
			else if(ft_strchr(token->content, '=')) /* If the token has an equal sign, add the environment variable */
			{
				if (ft_isdigit(token->content[0]) || ft_hasSpecialChar(*ft_split(token->content, '=')) || !ft_strcmp(token->content, "") || !ft_strcmp(token->content, "="))
				{
					printf("export: not valid in this context: %s\n", token->content);
					continue;
				}
				word = ft_strchr(token->content, '=') + 1;
				*ft_strchr(token->content, '=') = 0;
				if(!*(token->content))
					printf("Error\n");
				ft_add_env_back(&command->env, ft_new_env(token->content, word, 1));
			} else
			{
				if (!ft_strcmp(token->content, "") || !ft_strcmp(token->content, "=") || ft_isdigit(token->content[0]) || ft_hasSpecialChar(*ft_split(token->content, '=')) )
				{
					printf("export: not valid in this context: %s\n", token->content);
					continue ;
				}
				if(!ft_fnd_env(command->env, token->content))
					ft_add_env_back(&command->env,ft_new_env(token->content, "", 0));
			}
		}

}
