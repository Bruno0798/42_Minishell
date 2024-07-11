/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 18:17:14 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/08 18:17:14 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_key(char *equal_sign, char *key);

int	starts_with_special_char(const char *content)
{
	return (content[0] == '=' || content[0] == '-');
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

void	handle_error_and_cleanup(char *key)
{
	print_error(ERROR_EXP, NULL, 1);
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
		handle_error_and_cleanup(key);
		return ;
	}
	key = extract_key(token->content);
	if (key == NULL)
	{
		handle_error_and_cleanup(key);
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
