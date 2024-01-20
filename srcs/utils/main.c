/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:35:39 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/20 14:29:45 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


static int count_quotes(char *str);

static bool syntax_checker(t_commands *command)
{
	t_token *token;

	
	
}

static void parser(t_commands *command)
{	
	
	if(!(ft_strcmp(command->token->content, "pwd")))
		ft_pwd();
	if(!(ft_strncmp(command->token->content, "echo", 4)))
		ft_echo(command->token->next);
}

static int count_quotes(char *str)
{
	int i;
	int s_quote;
	int d_quote;
	
	i = -1;
	d_quote = 0;
	s_quote = 0;
	while (str[++i])
		if (str[i] == DOUBLE_QUOTE && !s_quote)
			d_quote = !d_quote;
		else if (str[i] == SINGLE_QUOTE && !d_quote)
			s_quote =  !s_quote;
	if (s_quote != 0 || d_quote != 0)
		return 1;
	return 0; 
}

static bool is_everything_space(char *str)
{
	int i;

	i = -1;
	while (str[++i])
		if (str[i] != ' ')
			return false;
	return true;
}

static bool check_input(char *input)
{
	if (count_quotes(input))
		return false;
	if (is_everything_space(input))
		return false;
	return true;
}

int main(int argc, char **argv, char **envp)
{
	(void)		argv;
	t_commands	*command;
	t_env		*env;
	char		*input;

	if (argc != 1)
	{
		ft_printf(RED"Wrong Arguments\nUse './minishell' to start!\n"ENDC);
		return (0);
	}
	init_env(&env, envp);
	while (1)
	{
		input = readline("Minishell$>");
		if (!check_input(input))
		{
			ft_printf(RED"Error: Invalid Input\n"ENDC); // ! Error example
			continue ;
		}
		command = pipe_commands(input, env);
		parser(command);
	}
	return 0;
}
