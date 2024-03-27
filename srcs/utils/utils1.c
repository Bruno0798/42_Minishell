/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 21:14:51 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/03/25 15:20:18 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 10:29:46 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/03/07 12:54:06 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	token->next = NULL;
	return (token);
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
		current =  current->next;  /* Move to the next token in the list */
	}
	command->token = head;  /* Assign the head of the list of tokens to the token field of the t_commands structure */
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
		current = current->next;  /* Move to the next command in the list */
	}
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

// t_token *ft_has_redirection(t_token *token)
// {
// 	while(token)
// 	{
// 		if (token->type == redir_out)
// 			return token;
// 		if (token->type == redir_out)
// 			return token;
// 		token = token->next;
// 	}
// 	return NULL;
// }

int ft_redirect(t_commands *command)
{
	t_token *temp;

	temp = command->token;
	while(temp)
	{
		if(temp->type == redir_out2 || temp->type == redir_out)
		{
			command->redir_fd = open(temp->next->content, O_CREAT | O_RDWR, 0666);
			return (1);
		}
		// if(temp->type == redir_in2 || temp->type == redir_in)

		if(command->redir_fd == -1)
		{
			perror("Invalid file redirection");
			return (0);
		}
		temp = temp->next;
	}
	return (0);
}
