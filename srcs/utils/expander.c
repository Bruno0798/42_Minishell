/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 15:09:50 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/06/13 12:42:27 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *expand_exit_code(char *string);

void ft_expander(t_commands *commands)
{
	t_token *token = commands->token;

	EXIT_STATUS = 4000000;

	while (token)
	{
		if(ft_strchr(token->content, '$') && *(ft_strchr(token->content, '$') + 1) != '\0')
		{
			if(*(ft_strchr(token->content, '$') + 1) == '?')
				token->content = expand_exit_code(token->content);
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
