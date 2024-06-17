/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 15:23:03 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/06/17 17:18:03 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brperer <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 15:09:50 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/06/17 15:22:38 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *expand_exit_code(char *string);
char *expand_variables(t_commands *commands, char *string);
char *store_value(char *string);
int value_length(char *string);
char *expand_new_string(char *value, char*key, char *string);

void ft_expander(t_commands *commands)
{
	t_token *token = commands->token;

	while (token)
	{
		if(ft_strchr(token->content, '$') && *(ft_strchr(token->content, '$') + 1) != '\0')
		{
			if(*(ft_strchr(token->content, '$') + 1) == '?')
				token->content = expand_exit_code(token->content);
			else
				token->content = expand_variables(commands, token->content);
		}
		token = token->next;
	}
}

int calculate_extra_length(char *string, int num_len)
{
	int i;
	int extra_length;

	i = 0;
	extra_length = 0;
	while (string[i]) {
		if (string[i] == '$' && string[i + 1] == '?') {
			extra_length += num_len - 2;
			i++;
		}
		i++;
	}
	return extra_length;
}

void replace_exit_code(char *string, char *new_string, char *num, int num_len) {
	int i;
	int j;
	int k;

	i = 0;
	k = 0;
	while (string[i]) {
		if (string[i] == '$' && string[i + 1] == '?') {
			j = 0;
			while (j < num_len) {
				new_string[k++] = num[j++];
			}
			i += 2; // Skip the "$?"
		} else {
			new_string[k++] = string[i++];
		}
	}
	new_string[k] = '\0';
}

char *expand_exit_code(char *string) {
	char *num;
	char *new_string;
	int num_len;
	int extra_length;

	num = ft_itoa(EXIT_STATUS);
	num_len = ft_strlen(num);
	extra_length = calculate_extra_length(string, num_len);

	new_string = malloc(ft_strlen(string) + extra_length + 1);
	replace_exit_code(string, new_string, num, num_len);

	free(num);
	return new_string;
}

char *expand_variables(t_commands *commands, char *string)
{
	char *value;
	char *key;
	char *new_string;

	while (ft_strchr(string, '$'))
	{
		value = store_value(string);
		if (!value)
			return NULL; // Handle allocation failure
		key = ft_get_value(commands->env, value);
		if (key)
			new_string = expand_new_string(value, key, string);
		else
			new_string = expand_new_string(value, "", string);

		free(value); // Free the allocated value

		if (!new_string)
			return NULL; // Handle allocation failure

		// Replace the old string with the new one if different
		if (new_string != string) {
			free(string);
			string = new_string;
		}
	}

	return string;
}


char *expand_new_string(char *value, char* key, char *string)
{
	int i = 0;
	int j = 0;
	int k = 0;
	char *new_string;

	new_string = malloc(sizeof(char) * (ft_strlen(string) - ft_strlen(value) + ft_strlen(key) + 1));
	if (!new_string) // Check for malloc failure
		return NULL;

	while (string[i])
	{
		if (string[i] == '$' && ft_strncmp(&string[i + 1], value, ft_strlen(value)) == 0)
		{
			while (key[k])
				new_string[j++] = key[k++];
			i += ft_strlen(value) + 1; // Skip the value part in the original string
			while (string[i])
				new_string[j++] = string[i++];
			break;
		}
		else
		{
			new_string[j++] = string[i++];
		}
	}
	new_string[j] = '\0'; // Null-terminate the new string

	return new_string;
}


char *store_value(char *string)
{
	int i;
	int j;
	char *value;

	i = 0;
	j = 0;
	value = malloc(sizeof(char) * (value_length(string) + 1));
	if (!value) // Check for malloc failure
		return NULL;

	while (string[i])
	{
		if (string[i] == '$')
		{
			i++;
			while (string[i] != '$' && string[i] != ' ' && string[i] != '\0' && string[i] != SINGLE_QUOTE && string[i] != DOUBLE_QUOTE)
				value[j++] = string[i++];
			break; // Stop after capturing the value
		}
		i++;
	}
	value[j] = '\0'; // Null-terminate the value

	return value;
}

int value_length(char *string) {
	int i;
	int length;

	i = 0;
	length = 0;

	while (string[i] && string[i] != '$') {
		i++;
	}

	if (string[i] == '\0') {
		return 0;
	}

	i++;

	while (string[i] && string[i] != ' ' && string[i] != '$') {
		length++;
		i++;
	}

	return length;
}

char *ft_get_value(t_env *env, char *key) {
	t_env *current = env; // Initialize the current environment variable

	while (current) { // Iterate over each environment variable
		if (!strcmp(current->key, key)) { // If the key of the current environment variable matches the provided key
			return current->value; // Return the value of the current environment variable
		}
		current = current->next; // Move to the next environment variable
	}
	return NULL; // If no matching key is found, return NULL
}
