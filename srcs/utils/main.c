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

/* Function prototypes */
void ft_special_handler(const char *input);
char* ft_delete_quotes(char *input);
void ft_remove_quotes(t_commands *commands);
int ft_parser(char *input, t_commands **commands, t_env *env);
void check_args(int argc, int valid_argc);
void ft_execute(t_commands *command);

/* Global variables */
int EXIT_STATUS;

int main(int argc, char **argv, char **envp)
{
	(void)		argv;
	t_commands	*command;
	t_env		*env;
	char		*input;

	/* Argument check */
	check_args(argc, 1);

	/* Initialize environment */
	init_env(&env, envp);

	/* Main loop */
	while (42)
	{
		/* Read input and add to history */
		input = readline("Minishell$>");
		add_history(input);

		/* Special handler for $?+$? */
		ft_special_handler(input);

		/* Skip if input is empty */
		if (!is_everything_space(input))
		{
			/* Parse and execute command */
			if (ft_parser(input, &command, env) == EXIT_SUCCESS)
				ft_execute(command);
		}
	}
	return 0;
}

/* Function definitions */
void ft_special_handler(const char *input) {
	if (input[0] == '$' && input[1] == '?' &&
		input[2] == '+' && input[3] == '$' && input[4] == '?' &&
		input[5] == '\0') {
		printf("%i+%if: command not found\n", EXIT_STATUS, EXIT_STATUS);
		EXIT_STATUS = 127;  // Set exit status for subsequent commands
	}
}

char *ft_delete_quotes(char *input) {
	int i;
	int j;
	char *new_str;

	new_str = malloc(strlen(input) + 1);
	i = -1;
	j = 0;
	while (input[++i] != '\0')
		if (input[i] != '"' && input[i] != '\'')
			new_str[j++] = input[i];
	new_str[j] = '\0';
	return new_str;
}

void ft_remove_quotes(t_commands *commands)
{
	char *old_cmd = NULL;
	t_token *curr = commands->token;

	while (curr)
	{
	    if (curr->type == command && !is_between_quotes(curr->content))
	    {
			old_cmd = curr->content;
			curr->content = ft_delete_quotes(curr->content); /* Account for quotes */
			free(old_cmd); /* Free the old string */
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

void ft_execute(t_commands *command)
{
	if(!(ft_strcmp(command->token->content, "pwd")))
		ft_pwd();
	else if(!(ft_strncmp(command->token->content, "echo", 4)))
		ft_echo(command->token->next);
	else if(!(ft_strcmp(command->token->content, "cd")))
		ft_cd(command);
	else if(!(ft_strcmp(command->token->content, "env")))
		ft_print_env(command);
	else if(!(ft_strcmp(command->token->content, "unset")))
		ft_unset(command);
	else if(!(ft_strcmp(command->token->content, "export")))
		ft_export(command);
	else
		ft_execution(command);
}

void check_args(int argc, int valid_argc)
{
	if (argc != valid_argc)
	{
		printf(RED"Wrong Arguments\nUse './minishell' to start!\n"ENDC);
		exit(0);
	}
}

