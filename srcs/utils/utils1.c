/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 21:14:51 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/08 16:32:54 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*check_redir_syntax(t_token *token);

void	ft_print_token_list(t_token *head)
{
	t_token	*token;

	token = head;
	while (token)
	{
		printf("Token: '%s' \nType: '%d'\n\n", token->content, token->type);
		token = token->next;
	}
}

static	bool	check_echo_option(char *str)
{
	int	i;

	i = 0;
	while (str[++i])
	{
		if (str[i] != 'n')
			return (false);
	}
	return (true);
}

static	bool	check_redirection(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '>' && str[i + 1] == '>' && str[i + 2] == '>')
			return (false);
		if (str[i] == '<' && str[i + 1] == '<' && str[i + 2] == '<')
			return (false);
	}
	return (true);
}

t_type	ft_token_type(char *word)
{
	if (!ft_strcmp(word, "|"))
		return (pipes);
	if (!ft_strncmp(word, ">>", 2))
		return (redir_out2);
	if (!ft_strncmp(word, ">", 1))
		return (redir_out);
	if (!ft_strncmp(word, "<<", 2))
		return (redir_in2);
	if (!ft_strncmp(word, "<", 1))
		return (redir_in);
	if (!ft_strncmp(word, "-n", 2) && check_echo_option(word))
		return (option);
	if (!check_redirection(word))
		return (error);
	return (command);
}

int	count_pipes(char *str)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (str[++i])
		if (str[i] == '|')
			count++;
	return (count);
}

t_token	*ft_new_token(char *str)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->content = str;
	token->type = ft_token_type(str);
	token->next = NULL;
	if (token->type == redir_in || token->type == redir_in2 || token->type == redir_out || token->type == redir_out2)
		token = check_redir_syntax(token);
	return (token);
}

t_token	*check_redir_syntax(t_token *token)
{
	t_token	*next;
	char	*new_content;
	int		i;

	i = 0;
	while (token->content[i] && token->content[0] == token->content[i])
		i++;
	if (!token->content[i])
		return (token);
	next = malloc(sizeof(t_token));
	if (!next)
		return (NULL);
	next->content = ft_substr(token->content, i, ft_strlen(token->content) - i);
	if (!next->content)
	{
		free(next);
		return (NULL);
	}
	next->next = NULL;
	next->type = files;
	new_content = ft_substr(token->content, 0, i);
	if (!new_content)
	{
		free(next->content);
		free(next);
		return (NULL);
	}
	free(token->content);
	token->content = new_content;
	token->next = next;
	token->type = ft_token_type(token->content);
	token->next->type = files;
	token->content = ft_delete_quotes(token->content);
	token->next->content = ft_delete_quotes(token->next->content);
	return (token);
}

t_commands	*ft_new_commands(char *str, t_env *env)
{
	t_commands	*command;
	t_token		*head;
	t_token		*current;
	char		**words;
	int			i;

	i = 0;
	command = malloc(sizeof(t_commands));
	command->env = env;
	words = ft_split2(str, SPACE);
	head = ft_new_token(words[0]);
	current = head;
	while (words[++i])
	{
		current->next = ft_new_token(words[i]);
		current = current->next;
		while (current->next)
			current = current->next;
	}
	command->token = head;
	command->next = NULL;
	free(str);
	free(words);
	return (command);
}

t_commands	*pipe_commands(char *str, t_env *env)
{
	t_commands	*command;
	t_commands	*current;
	char		**splitted_command;
	int			i;

	i = 0;
	splitted_command = ft_split2(str, '|');
	command = ft_new_commands(splitted_command[0], env);
	current = command;
	while (splitted_command[++i])
	{
		current->next = ft_new_commands(splitted_command[i], env);
		current = current->next;
	}
	free(splitted_command);
	return (command);
}

bool	ft_has_special_char(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (true);
	return (false);
}
