/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 16:36:18 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/09 16:39:27 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	replace_exit_code(char *string,
	char *new_string, char *num, int num_len)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (string[i])
	{
		if (string[i] == '$' && string[i + 1] == '?')
		{
			j = 0;
			while (j < num_len)
				new_string[k++] = num[j++];
			i += 2;
		}
		else
			new_string[k++] = string[i++];
	}
	new_string[k] = '\0';
}

char	*expand_exit_code(char *string)
{
	char	*num;
	char	*new_string;
	int		num_len;
	int		extra_length;

	num = ft_itoa(g_exit_status);
	num_len = ft_strlen(num);
	extra_length = calculate_extra_length(string, num_len);
	new_string = malloc(ft_strlen(string) + extra_length + 1);
	replace_exit_code(string, new_string, num, num_len);
	free(num);
	free(string);
	return (new_string);
}

char	*expand_variables(t_commands *commands, char *string)
{
	char	*value;
	char	*key;
	char	*new_string;

	value = store_value(string);
	if (!value)
		return (NULL);
	key = ft_get_value(commands->env, value);
	if (key)
		new_string = expand_new_string(value, key, string);
	else
		new_string = expand_new_string(value, "", string);
	free(value);
	if (!new_string)
		return (NULL);
	if (new_string != string)
	{
		free(string);
		string = new_string;
	}
	return (string);
}

size_t get_new_string_length(char *value, char *key, char *string) {
	return ft_strlen(string) - ft_strlen(value) + ft_strlen(key) + 1;
}

char *allocate_new_string(size_t length) {
	char *new_string = malloc(sizeof(char) * length);
	if (!new_string) {
		return NULL;
	}
	return new_string;
}

void copy_key_to_new_string(char *new_string, int *j, char *key) {
	int k = 0;
	while (key[k]) {
		new_string[(*j)++] = key[k++];
	}
}

void replace_value_with_key(char *new_string, int *i, int *j, char *value, char *key, char *string) {
	copy_key_to_new_string(new_string, j, key);
	*i += ft_strlen(value) + 1;
	while (string[*i]) {
		new_string[(*j)++] = string[(*i)++];
	}
}

char *expand_new_string(char *value, char *key, char *string) {
	int i = 0, j = 0;
	size_t new_string_length = get_new_string_length(value, key, string);
	char *new_string = allocate_new_string(new_string_length);

	if (!new_string) {
		return NULL;
	}

	while (string[i]) {
		if (string[i] == '$' && ft_strncmp(&string[i + 1], value, ft_strlen(value)) == 0) {
			replace_value_with_key(new_string, &i, &j, value, key, string);
			break;
		}
		new_string[j++] = string[i++];
	}

	new_string[j] = '\0';
	return new_string;
}

char	*store_value(char *string)
{
	int		i;
	int		length;
	char	*key;

	i = 1;
	length = 0;
	while (string[i] != '$' && string[i] != DOUBLE_QUOTE && string[i] != '\0' && string[i] != ' ' && string[i] != SINGLE_QUOTE)
	{
		i++;
		length++;
	}
	key = malloc(sizeof(char) * length + 1);
	i = 0;
	while (i < length)
	{
		key[i] = string[i + 1];
		i++;
	}
	key[i] = '\0';
	return (key);
}
