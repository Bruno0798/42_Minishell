/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 21:14:51 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/06/25 15:21:17 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token *check_redir_syntax(t_token *token);

void ft_print_token_list(t_token *head) 
{
	t_token *token = head;

	while (token)
	{
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
	else if (!ft_strncmp(word, ">>", 2))
		return redir_out2;
	else if (!ft_strncmp(word, ">", 1))
		return redir_out;
	else if (!ft_strncmp(word, "<<", 2))
		return redir_in2;
	else if (!ft_strncmp(word, "<", 1))
		return redir_in;
	else if(!ft_strncmp(word, "-n", 2) && check_echo_option(word))
		return option;
	else if (!check_redirection(word))
		return error; //! make function to check what to print and exit
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
	if(token->type == redir_in || token->type == redir_in2 || token->type == redir_out || token->type ==redir_out2)
		token = check_redir_syntax(token);
	token->next = NULL;
	return (token);
}

t_token *check_redir_syntax(t_token *token)
{
	t_token *next;
	char *new_content;
	int i;

	i = 0;
	while (token->content[i] && token->content[0] == token->content[i])
		i++;

	next = malloc(sizeof(t_token));
	if (!next)
		return NULL;
	next->content = ft_substr(token->content, i, strlen(token->content) - i);
	if (!next->content)
	{
		free(next);
		return NULL;
	}

	new_content = ft_substr(token->content, 0, i);
	if (!new_content)
	{
		free(next->content);
		free(next);
		return NULL;
	}

	free(token->content);
	token->content = new_content;
	token->next = next;
	token->next->type = files;
	token = token->next;

	return token;
}

t_commands *ft_new_commands(char *str, t_env *env)
{
	t_commands	*command;  /* Pointer to the new t_commands structure */
	t_token		*head;  /* Pointer to the head of the list of tokens */
	t_token		*current;  /* Pointer to the current token in the list */
	char		**words;  /* Array of words split from the provided string */
	int			i;  /* Counter for iterating over the array of words */

	i = 0;
	command = malloc(sizeof(t_commands));  /* Allocate memory for the new t_commands structure */
	command->env = env;  /* Assign the provided environment to the t_commands structure */
	words = ft_split2(str, SPACE);  /* Split the provided string into words */
	head = ft_new_token(words[0]);  /* Create a new token for the first word */
	current = head;  /* Assign the head of the list of tokens to the current token */
	while(words[++i])  /* Iterate over the rest of the words in the array */
	{
		current->next = ft_new_token(words[i]);  /* Create a new token for the current word and add it to the end of the list */
		current = current->next;  /* Move to the next token in the list */
	}
	command->token = head;  /* Assign the head of the list of tokens to the token field of the t_commands structure */

	command->next = NULL;
	free(str);
	free(words);
	return command;  /* Return the new t_commands structure */
}

t_commands	*pipe_commands(char *str, t_env *env)
{
	t_commands	*commands;  /* Pointer to the head of the list of commands */
	t_commands	*current;  /* Pointer to the current command in the list */
	char		**splitted_command;  /* Array of commands split from the provided string */
	int			i;  /* Counter for iterating over the array of commands */

	i = 0;
	splitted_command = ft_split2(str, '|');  /* Split the provided string into commands */
	commands = ft_new_commands(splitted_command[0], env);  /* Create a new t_commands structure for the first command */
	current = commands;  /* Assign the head of the list of commands to the current command */
	while(splitted_command[++i])  /* Iterate over the rest of the commands in the array */
	{
		current->next = ft_new_commands(splitted_command[i], env);  /* Create a new t_commands structure for the current command and add it to the end of the list */
		current = current->next; /* Move to the next command in the list */
	}
	free(splitted_command);
	return (commands);  /* Return the head of the list of commands */
}

bool	ft_hasSpecialChar(char *str)
{
	int i;

	i = -1;
	while(str[++i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_')){
			return true;
		}
	}
	return false;
}