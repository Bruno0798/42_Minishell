/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:21:30 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/29 18:23:12 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	check_pipes(char *input)
{
	char	*str;
	char	*next;
	char	*content;

	str = input;
	while (str)
	{
		next = ft_strpbrk(str, "|");
		if (!next)
			return (1);
		content = ft_substr(str, 0, next - str);
		if (is_everything_space(content))
		{
			free(content);
			free(input);
			return (0);
		}
		free(content);
		str = next + 1;
	}
	return (1);
}

bool	is_valid_input(char *input, t_env *env)
{
	if (!input)
	{
		free_env(env);
		printf("exit\n");
		exit(g_exit_status);
	}
	if (is_everything_space(input))
		return (false);
	if (!syntax_checker(input))
		return (false);
	if (check_quotes(input))
		return (false);
	if (!check_pipes(input))
	{
		print_error(ERROR_PIPE, NULL, 2);
		return (false);
	}
	return (true);
}

bool	is_between_single_quotes(char *str)
{
	int	s_quote;
	int	d_quote;
	int	i;

	i = -1;
	s_quote = 0;
	d_quote = 0;
	while (str[++i])
	{
		if (str[i] == DOUBLE_QUOTE && !s_quote)
			d_quote = !d_quote;
		else if (str[i] == SINGLE_QUOTE && !d_quote)
			s_quote = !s_quote;
	}
	if (s_quote != 0)
		return (true);
	return (false);
}
