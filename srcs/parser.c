/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:11:34 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 12:01:33 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_empty_nodes(t_commands *commands);

bool	check_syntax(t_commands *commands)
{
	t_commands	*temp;
	t_token		*curr;

	temp = commands;
	while (temp)
	{
		curr = commands->token;
		while (curr)
		{
			if (curr->type == redir_in || curr->type == redir_out)
			{
				if (curr->next && curr->next->type != command
					&& (curr->next->type != files
						|| curr->next->type == redir_in2
						|| curr->next->type == redir_out2))
				{
					ft_printf("error token: %s \n", curr->next->content);
					return (true);
				}
			}
			curr = curr->next;
		}
		temp = temp->next;
	}
	return (false);
}

int	ft_parser(char *input, t_commands **commands, t_env *env)
{
	*commands = pipe_commands(input, env);
	if (*commands == NULL)
		return (EXIT_FAILURE);
	if (check_syntax(*commands))
		return (EXIT_FAILURE);
	if (!has_here_doc(*commands))
		ft_expander(*commands);
	else
		ft_expander_heredoc(*commands);
	ft_empty_nodes(*commands);
	ft_remove_quotes(*commands);
	ft_expand_others(*commands);
	return (EXIT_SUCCESS);
}

void	ft_empty_nodes(t_commands *commands)
{
	t_commands	*head;
	t_token		*token_temp;
	t_token		*temp;
	t_token		*prev;

	head = commands;
	while (commands)
	{
		token_temp = commands->token;
		prev = NULL;
		while (commands->token != NULL)
		{
			if (commands->token->content != NULL
				&& ft_strcmp(commands->token->content, "") == 0)
			{
				temp = commands->token;
				if (prev != NULL)
					prev->next = commands->token->next;
				else
					token_temp = commands->token->next;
				free(temp->content);
				free(temp);
				if (prev == NULL)
					commands->token = token_temp;
				else
					commands->token = prev->next;
			}
			else
			{
				prev = commands->token;
				commands->token = commands->token->next;
			}
		}
		commands->token = token_temp;
		commands = commands->next;
	}
	commands = head;
}

void	ft_expand_others(t_commands *commands)
{
	int		i;
	t_token	*curr;
	char	*str;

	curr = commands->token;
	while (curr)
	{
		i = -1;
		str = curr->content;
		while (str[++i])
		{
			if ((str[i] == '~') && (i == 0)
				&& ft_get_value(commands->env, "HOME") && ((str[i + 1] == ' ')
					|| (curr ->content[i + 1] == '\0') || (str[i + 1] == '/')))
				str = ft_strjoin(ft_get_value(commands->env, "HOME"), str + 1);
			else if ((str[i] == '~') && (i == 0) && (str[i + 1] == '+'))
				str = ft_strjoin(ft_get_value(commands->env, "PWD"), str + 2);
			else if ((str[i] == '~') && (i == 0) && (str[i + 1] == '-'))
				str = ft_strjoin(ft_get_value(commands->env, "OLDPWD"), str + 2);
		}
		curr = curr->next;
	}
	free (curr);
}

char	*ft_delete_quotes(char *input)
{
	int		i;
	int		j;
	char	*new_str;
	char	quote;

	new_str = malloc(ft_strlen(input) + 1);
	i = -1;
	j = 0;
	while (input[++i])
	{
		if (input[i] != '"' && input[i] != '\'')
			new_str[j++] = input[i];
		else if (input[i] == '"' || input[i] == '\'')
		{
			quote = input[i++];
			while (input[i] != quote)
				new_str[j++] = input[i++];
		}
	}
	new_str[j] = '\0';
	free(input);
	return (new_str);
}
