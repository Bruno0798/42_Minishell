/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:35:39 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/02/26 15:03:10 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *ft_delete_quotes(char *input, size_t len)
{
	size_t i = 0;
	size_t j = 0;
	char *new_cmd;
	char flag;

	flag = input[i];
	new_cmd = ft_calloc(len + 1, sizeof(char));
	if (!new_cmd)
	    return NULL;
	while (input[j])
	{
		if (input[j] == flag)
		    j++;
		else
			new_cmd[i++] = input[j++];
	}
	new_cmd[i] = '\0';
	return new_cmd;
}

void ft_remove_quotes(t_commands *commands)
{
	char *old_cmd = NULL;
	t_token *curr = commands->token;

	while (curr)
	{
	    if (curr->type == command && (curr->content[0] == SINGLE_QUOTE || curr->content[0] == DOUBLE_QUOTE))
	    {
			old_cmd = curr->content;
			curr->content = ft_delete_quotes(curr->content, ft_strlen(curr->content - 2));
			free(old_cmd);
	    }
		curr = curr->next;
	}
}

int ft_parser(char *input, t_commands **commands, t_env *env)
{
	if (is_between_quotes(input))
		return (EXIT_FAILURE);
	*commands = pipe_commands(input, env);
	ft_expander(*commands);
	ft_remove_quotes(*commands);
	return EXIT_SUCCESS;
}

static void ft_execute(t_commands *command)
{
	if(!(ft_strcmp(command->token->content, "pwd")))
		ft_pwd();
	else if(!(ft_strncmp(command->token->content, "echo", 4)))
		ft_echo(command->token->next);
	else if(!(ft_strcmp(command->token->content, "cd")))
		ft_cd(command);
	else if(!(ft_strcmp(command->token->content, "env")))
		ft_env(command);
	else if(!(ft_strcmp(command->token->content, "unset")))
		ft_unset(command);
	else if(!(ft_strcmp(command->token->content, "export")))
		ft_export(command);
	else
		ft_execution(command);
}


int main(int argc, char **argv, char **envp)
{
	(void)		argv;
	t_commands	*command;
	t_env		*env;
	char		*input;

	check_args(argc, 1);
	init_env(&env, envp);
	while (42)
	{
		input = readline("Minishell$>");
		add_history(input);
		//ft_special_handler(input);
		if (!is_everything_space(input))
		{
			if (ft_parser(input, &command, env) == EXIT_SUCCESS)
				ft_execute(command);
//			command = pipe_commands(input, env);
//			parser(command);
		}
	}
	return 0;
}

void check_args(int argc, int valid_argc)
{
	if (argc != valid_argc)
	{
		printf(RED"Wrong Arguments\nUse './minishell' to start!\n"ENDC);
		exit(0);
	}
}

//void ft_special_handler(const char *input)
//{
//	if (input[0] == '$' && input[1] == '?')
//	{
//		if (input[2] == '+' && input[3] == '$' && input[4] == '?')
//		{
//			printf("%i+%if: command not found\n", g_exit_status, g_exit_status);
//			g_exit_status = 127;
//		}
//	}
//}
