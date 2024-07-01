/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:42:09 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/06/18 23:01:47 by bsousa-d         ###   ########.fr       */
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

bool is_invalid_token(char *str)
{
	int i;

	i = 0;
	if (!ft_isalpha(str[0]) && ft_isdigit(str[0]))
		return false;

	while(str[i])
	{
		if(!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '=' && str[i] != ' ')
			return false;
		i++;
	}
	return true;
}

void handle_invalid_token(const char *content) {
	ft_fprintf(2, " not a valid identifier\n");  //TODO DELETE THIS FUNCTION TO REPLACE WITH PRINT ERROR
	g_exit_status = 1;
}

void update_or_add_env(t_commands *command, char *key, char *value) {
	if (ft_fnd_env(command->env, key)) {
		ft_update_env(command->env, key, value, 1);
	} else {
		ft_add_env_back(&command->env, ft_new_env(key, value, 1));
	}
}

void handle_export_token(t_commands *command, t_token *token) {
	if (token->content[0] == '=' || token->content[0] == '-') {
		ft_fprintf(2, " not a valid identifier\n", token->content);
		g_exit_status = 1;
		return;
	}

	char *equal_sign = ft_strchr(token->content, '=');
	char *key = equal_sign ? token->content : ft_strdup(token->content);

	if (equal_sign) {
		*equal_sign = '\0';
	}

	if (!is_invalid_token(key))
	{
		if (equal_sign) {
			*equal_sign = '='; // Restore the original string
		}else
			free(key);
		handle_invalid_token(token->content); //TODO CHANGE TO PRINT ERROR
		return;
	}

	if (equal_sign) {
		char *value = equal_sign + 1;
		update_or_add_env(command, key, value);
		*equal_sign = '='; // Restore the original string
	} else {
		if (!ft_fnd_env(command->env, key)) {
			ft_add_env_back(&command->env, ft_new_env(key, "", 0));
		}
	}

	if (!equal_sign) {
		free(key); // Free the copied key if no '=' was found
	}
}

void ft_export(t_commands *command) {
	t_token *token = command->token;

	if (!token->next) {
		print_env(command->env);
		return;
	}
	while (token->next) {
		token = token->next;
		handle_export_token(command, token);
	}
}