/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:35:39 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/22 15:04:38 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"



static void parser(t_commands *command)
{	
	ft_expander(command);
	if(!(ft_strcmp(command->token->content, "pwd")))
		ft_pwd();
	if(!(ft_strncmp(command->token->content, "echo", 4)))
		ft_echo(command->token->next);
		
}


static bool check_input(char *input)
{
	if (is_beetwenn_quotes(input))
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

	check_args(argc, 1);
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


void check_args(int argc, int valid_argc)
{
	if (argc != valid_argc)
	{
		ft_printf(RED"Wrong Arguments\nUse './minishell' to start!\n"ENDC);
		exit(0);
	}
}