/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:11:34 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/05/21 17:19:50 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_parser(char *input, t_commands **commands, t_env *env)
{
	add_history(input);
	if (!is_between_quotes(input))
		return (EXIT_FAILURE);
	*commands = pipe_commands(input, env);
	if (!has_here_doc(*commands))
		ft_expander(*commands);
	ft_expand_others(*commands);
	ft_remove_quotes(*commands);
	return EXIT_SUCCESS;
}

void ft_expand_others(t_commands *commands)
{
	int i;
	t_token *curr = commands->token;

	i = -1;
	while(curr)
	{
		i = -1;
		while (curr->content[++i])
		{
			if ((curr->content[i] == '~') && (i == 0) && ft_get_value(commands->env, "HOME") && ((curr->content[i + 1] == ' ') || (curr ->content[i + 1] ==
																																   '\0') || (curr->content[i + 1] == '/')))
				curr->content = ft_strjoin(ft_get_value(commands->env, "HOME"), curr->content + 1);
			else if ((curr->content[i] == '~') && (i == 0) && (curr->content[i + 1] == '+'))
				curr->content = ft_strjoin(ft_get_value(commands->env, "PWD"), curr->content + 2);
			else if ((curr->content[i] == '~') && (i == 0) && (curr->content[i + 1] == '-'))
				curr->content = ft_strjoin(ft_get_value(commands->env, "OLDPWD"), curr->content + 2);
		}
		curr = curr->next;
	}
	free (curr);
}

char *ft_delete_quotes(char *input) {
	int i;
	int j;
	char *new_str;
	char quote;

	new_str = malloc(strlen(input) + 1);
	i = -1;
	j = 0;
	while (input[++i])
	{
		if (input[i] != '"' && input[i] != '\'')
			new_str[j++] = input[i];
		else if (input[i] == '"' || input[i] == '\'')
		{
			quote = input[i++];
			while(input[i] != quote)
				new_str[j++] = input[i++];
		}
	}
	new_str[j] = '\0';
	free(input);
	return new_str;
}

void ft_remove_quotes(t_commands *commands)
{
	t_token *curr = commands->token;

	while (curr)
	{
		if (curr->type == command && is_between_quotes(curr->content))
			curr->content = ft_delete_quotes(curr->content); /* Account for quotes */
		curr = curr->next;
	}
	free(curr);
}

int count_commands(t_commands *command)
{
	int count;
	t_commands *token;

	count = 0;
	token = command;
	while (token)
	{
		count++;
		token = token->next;
	}
	return count;
}