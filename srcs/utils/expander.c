/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 15:09:50 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/05/27 17:44:57 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *expand_variable(char *input, int index, t_commands *command) {
	char *key;
	char *value;
	char *num;

	if (input[index] == '$' && input[index + 1] == '?') {
		num = ft_itoa(EXIT_STATUS);
		input = replace_substr(input, "$?", num, index);
		free(num);
	} else {
		key = trim_right(&input[index]);
		value = ft_get_value(command->env, key + 1);
		if (value)
			input = replace_substr(input, key, value, index);
		else
			input = replace_substr(input, key, "", index);
		free(key);
	}
	return input;
}

char *needs_expansion(char *input, char c, t_commands *command) {
	bool single_quotes = false;
	bool double_quotes = false;
	int i = 0;
	int length;

	while (input[i]) {
		if (input[i] == '\'' && !double_quotes) {
			single_quotes = !single_quotes;
		} else if (input[i] == '"' && !single_quotes) {
			double_quotes = !double_quotes;
		} else if (input[i] == c && !single_quotes) {
			length = strlen(input);
			input = expand_variable(input, i, command);
			i += strlen(input) - length;
		}
		if (i > ft_strlen(input))
			break ;
		i++;
	}
	return input;
}

void ft_expander(t_commands *command) {
	t_token *token = command->token;

	while (token != NULL) {
		if (ft_strchr(token->content, '$')) {
			token->content = needs_expansion(token->content, '$', command);
		}
		token = token->next;
	}
}

char *replace_substr(char *original, const char *substr, char *replacement, int i) {
	if (!original || !substr || !replacement) {
		return NULL;
	}
	size_t original_len = ft_strlen(original);
	size_t substr_len = ft_strlen(substr);
	size_t replacement_len = ft_strlen(replacement);

	// Find the position of the substring in the original string
	const char *substr_pos = ft_strnstr(original + i, substr,
										ft_strlen(original));
	if (!substr_pos) {
		return NULL; // Substring not found
	}

	// Calculate the length of the resulting string
	size_t result_len = original_len - substr_len + replacement_len;

	// Allocate memory for the resulting string
	char *result = malloc(result_len + 1);
	if (!result) {
		return NULL;
	}

	// Copy the part of the original string before the substring
	strncpy(result, original, substr_pos - original);
	result[substr_pos - original] = '\0';

	// Concatenate the replacement string
	strcat(result, replacement);

	// Concatenate the part of the original string after the substring
	strcat(result, substr_pos + substr_len);
	free(original);

	return result;
}

char *trim_right(const char *input_string) {
	const char *ptr = input_string;
	char *result;
	int length = 0;

	// Skip initial spaces
	while (*ptr && isspace(*ptr)) {
		ptr++;
	}

	// Find the end of the first word
	while (*ptr && !isspace(*ptr) && *ptr != '\'' && *ptr != '"' && *ptr != '=') {
		ptr++;
		length++;
	}

	// Allocate memory for the result string
	result = (char *)malloc((length + 1) * sizeof(char));
	if (result == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}

	// Copy the first word to the result string
	strncpy(result, input_string, length);
	result[length] = '\0'; // Null-terminate the result string

	return result;
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
