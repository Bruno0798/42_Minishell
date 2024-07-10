/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 22:01:58 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/08 17:03:21 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_everything_space(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] != ' ')
			return (false);
	return (true);
}

bool	is_between_quotes(char *str)
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
	if (s_quote != 0 || d_quote != 0)
		return (false);
	return (true);
}

bool	check_quotes(char *str)
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
	if (s_quote != 0 || d_quote != 0)
	{
		print_error(ERROR_QUOTE, NULL, 1);
		return (true);
	}
	return (false);
}

bool	syntax_checker(char *input)
{
	input = ft_strtrim(input, " \t");
	if (input && !ft_strchr("|<>", input[ft_strlen(input) - 1])
		&& *input != '|')
	{
		if (ft_strcmp("<", input) && ft_strcmp(">", input)
			&& !(ft_strchr(input, '|') && *(ft_strchr(input, '|') + 1) == '|'))
		{
			free(input);
			return (true);
		}
	}
	if (ft_strchr(input, '|')
		&& *(ft_strchr(input, '|') + 1) == '|')
		print_error(ERROR_PIPE_2, NULL, 2);
	else if (*input == '|')
		print_error(ERROR_PIPE, NULL, 2);
	else if (input[ft_strlen(input) - 1] == '|')
		print_error(ERROR_PROMPT, NULL, 2);
	else if (ft_strchr("<>", input[ft_strlen(input) - 1]))
		print_error(ERROR_REDIR, NULL, 2);
	free(input);
	return (false);
}

void	check_args(int argc, int valid_argc)
{
	if (argc != valid_argc)
	{
		printf(RED"Wrong Arguments\nUse './minishell' to start!\n"ENDC);
		exit(0);
	}
}
