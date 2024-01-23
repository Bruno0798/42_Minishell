/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 10:29:46 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/23 17:11:09 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_print_token_list(t_token *head) 
{
	t_token *token = head;

	while (token) {
		printf("Token: '%s' \nType: '%d'\n\n", token->content, token->type);
		token = token->next;
	}
}

static bool check_echo_option(char *str)
{
	int i;

	i = 0;

	while(str[++i])
	{
		if (str[i] != 'n')
			return false;
	}
	return true;
}

static bool check_redirection(char *str)
{
	int i;

	i = -1;
	while(str[++i] == '<')
		if(i == 2)
			return false;
	i = -1;
	while (str[++i] == '>')
		if(i == 2)
			return false;
	i = 0;
	while((str[i] == '>' && str[i + 1] == '<') ||
		(str[i] == '<' && str[i + 1] == '>'))
		return false;
	return true;

}

t_type ft_token_type(char *word)
{
	if (!ft_strcmp(word, "|"))
		return pipes;
	else if (!ft_strcmp(word, ">>"))
		return redir_out2;
	else if (!ft_strcmp(word, ">"))
		return redir_out;
	else if (!ft_strcmp(word, "<<"))
		return redir_in2;
	else if (!ft_strcmp(word, "<"))
		return redir_in;
	else if(!ft_strncmp(word, "-n", 2) && check_echo_option(word))
		return option;
	else if (!check_redirection(word) || !check_redirection(word))
		return error; //! make funtion to check what to print and exit
	return command;
}

int count_pipes(char *str)
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

t_token *ft_new_token(char *str)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	token->content = str;
	token->type = ft_token_type(str);
	token->next = NULL;
	return (token);
}

t_commands *ft_new_commands(char *str, t_env *env)
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
	while(words[++i])
	{
		current->next = ft_new_token(words[i]);
		current =  current->next;
	}
	command->token = head;
	return command;
}

t_commands	*pipe_commands(char *str, t_env *env)
{
	t_commands	*commands;
	t_commands	*current;
	char		**splitted_command;
	int			i;

	i = 0;
	splitted_command = ft_split2(str, '|');
	commands = ft_new_commands(splitted_command[0], env);
	current = commands;
	while(splitted_command[++i])
	{
		current->next = ft_new_commands(splitted_command[i], env);
		current = current->next;
	}
	return (commands);
}


