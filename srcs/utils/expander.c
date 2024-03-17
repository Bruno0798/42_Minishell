/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 15:09:50 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/03/17 14:49:50 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

char *ft_expand_join(char *s1, t_env *env);
char* trim_right(const char *input_string);
char *replace_substr(const char *original, const char *substr, const char *replacement);

bool needs_expansion(const char *input, char c) {
	bool single_quotes;
	bool double_quotes;

	single_quotes = false;
	double_quotes = false;

	while (*input) {
		if (*input == '\'' && !double_quotes)
			single_quotes = !single_quotes;
		else if (*input == '"' && !single_quotes)
			double_quotes = !double_quotes;
		else if (*input == c && (!single_quotes || double_quotes))
			return true;
		input++;
	}
	return false;
}


void ft_expander(t_commands *command)
{
	t_token *token;
	int i;

	token = command->token;
	i=0;
	while (token->next)
	{
		i++;
		while(ft_strchr(token->next->content, '$'))
		{
			if (needs_expansion(token->next->content, '$'))
				token->next->content = ft_expand_join(token->next->content, command->env);
			else
				break;
		}
		token = token->next;
	}
}

char *ft_expand_join(char *s1, t_env *env)
{
	char *value;
	char *key;

	key = trim_right(ft_strchr(s1, '$'));
	value = ft_get_value(env, key + 1);
	if (value)
		s1 = replace_substr(s1, key, value);
	else
		s1 = replace_substr(s1, key, "");
	return (s1);
}

char *replace_substr(const char *original, const char *substr, const char *replacement) {
	if (!original || !substr || !replacement)
		return NULL;

	size_t original_len = strlen(original);
	size_t substr_len = strlen(substr);
	size_t replacement_len = strlen(replacement);

	// Find the position of the substring in the original string
	const char *substr_pos = strstr(original, substr);
	if (!substr_pos)
		return NULL; // Substring not found

	// Calculate the length of the resulting string
	size_t result_len = original_len - substr_len + replacement_len;

	// Allocate memory for the resulting string
	char *result = malloc(result_len + 1);
	if (!result)
		return NULL;

	// Copy the part of the original string before the substring
	strncpy(result, original, substr_pos - original);
	result[substr_pos - original] = '\0';

	// Concatenate the replacement string
	strcat(result, replacement);

	// Concatenate the part of the original string after the substring
	strcat(result, substr_pos + substr_len);

	return result;
}

char* trim_right(const char *input_string)
{
	const char *ptr = input_string;
	char *result;
	int length = 0;

	// Skip initial spaces
	while (*ptr && isspace(*ptr)) {
		ptr++;
	}

	// Find the end of the first word
	while (*ptr && !isspace(*ptr) && *ptr != SINGLE_QUOTE && *ptr != DOUBLE_QUOTE) {
		ptr++;
		length++;
	}

	// Allocate memory for the result string
	result = (char*)malloc((length + 1) * sizeof(char));
	if (result == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}

	// Copy the first word to the result string
	strncpy(result, input_string, length);
	result[length] = '\0';  // Null-terminate the result string

	return result;
}



char *ft_get_value(t_env *env, char *key)
{
	t_env *current;  /* Pointer to the current environment variable */

	current = env;  /* Initialize the current environment variable */
	while(current)  /* Iterate over each environment variable */
	{
		if (!ft_strcmp(current->key, key))  /* If the key of the current environment variable matches the provided key */
			return current->value;  /* Return the value of the current environment variable */
		current = current->next;  /* Move to the next environment variable */
	}
	return NULL;  /* If no matching key is found, return NULL */
}
