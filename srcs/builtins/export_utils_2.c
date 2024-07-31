/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 18:17:14 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/31 12:24:51 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_key(char *equal_sign, char *key);

int	starts_with_special_char(const char *content)
{
	return (content[0] == '=' || content[0] == '-'
		|| ft_strlen(content) == 0);
}

char	*extract_key(char *content)
{
	char	*equal_sign;

	equal_sign = ft_strchr(content, '=');
	if (equal_sign != NULL)
		return ((char *)content);
	else
		return (ft_strdup(content));
}

void	handle_error_and_cleanup(char *str, char *key, int exit_code)
{
	if (str && str[0] == '-')
		exit_code = 2;
	print_error(ERROR_EXP, str, exit_code);
	if (key != NULL)
		free(key);
}

void	handle_export_token(t_commands *command, t_token *token)
{
	char	*key;
	char	*equal_sign;

	key = NULL;
	if (starts_with_special_char(token->content))
	{
		handle_error_and_cleanup(token->content, key, 1);
		return ;
	}
	key = extract_key(token->content);
	if (key == NULL)
	{
		handle_error_and_cleanup(NULL, key, 1);
		return ;
	}
	equal_sign = ft_strchr(token->content, '=');
	if (equal_sign != NULL)
		*equal_sign = '\0';
	if (!validate_and_handle_key(&key, equal_sign, token))
		return ;
	handle_env_update(command, key, equal_sign);
	cleanup_key(equal_sign, key);
}

void	cleanup_key(char *equal_sign, char *key)
{
	if (equal_sign == NULL && key != NULL)
		free(key);
}
