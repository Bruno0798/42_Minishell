/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 21:14:51 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/10 17:20:48 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	while(str[++i])
	{
		if(str[i] == '>' && str[i + 1] == '>' && str[i + 2] == '>')
			return false;
		if(str[i] == '<' && str[i + 1] == '<' && str[i + 2] == '<')
			return false;
	}
	return (true);
}

t_type	ft_token_type(char *word)
{
	if (!ft_strcmp(word, "|"))
		return (pipes);
	if (!ft_strcmp(word, ">>"))
		return (redir_out2);
	if (!ft_strcmp(word, ">"))
		return (redir_out);
	if (!ft_strcmp(word, "<<"))
		return (redir_in2);
	if (!ft_strcmp(word, "<"))
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

t_token	*ft_new_token(char *str, int flag)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->content = str;
	token->type = ft_token_type(str);
	token->next = NULL;
	if (!redir_valid(token))
		exit(g_exit_status);
	if ((ft_strchr(str, '<') || ft_strchr(str, '>')) && token->type == command && flag)
		token = check_redir_syntax(token);
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
	head = ft_new_token(words[0], 1);
	current = head;
	while (current->next)
			current = current->next;
	while (words[++i])
	{
		current->next = ft_new_token(words[i], 1);
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
