/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:25:43 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/06/19 19:07:24 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int init_and_set_fd(int argc, char **envp, t_env **env);

//TODO EXPAND NO HEREDOC ESTA MAL, NAO DA EXPAND NO EOF MAS TEM DE DAR DENTRO
//TODO CHANGED THE MESSAGES ERRORS ON THE TESTER AND THE PROJECT


int	EXIT_STATUS;

int main(int argc, char **argv, char **envp)
{
	(void)		argv;
	t_commands	*command; // NEEDS TO BE REMOVED FROM HERE AND PASS TO PARER
	t_env		*env;
	char		*input;
	int			fd;
	int fd1;


	fd = init_and_set_fd(argc, envp, &env);
	fd1 = dup(1);
	command = NULL; //empty line
	while (42)
	{
		handle_signals();
		dup2(fd, STDIN_FILENO);
		dup2(fd1, STDOUT_FILENO);
		input = readline("Minishell$>");
		if (is_valid_input(input, env))
			if (ft_parser(input, &command, env) == EXIT_SUCCESS)
			{
				remake_commands(command);
				open_pipes(command);
			}
		ft_special_handler(input); // NEEDS TO BE REMOVED FROM HERE
		ft_free_commands(command, 1);
	}
	return 0;
}

static int init_and_set_fd(int argc, char **envp, t_env **env)
{
	int fd;

	fd = dup(STDIN_FILENO);
	check_args(argc, 1);
	init_env(env, envp);

	return fd;
}

void ft_special_handler(const char *input) {
	if (input[0] == '$' && input[1] == '?' &&
		input[2] == '+' && input[3] == '$' && input[4] == '?' &&
		input[5] == '\0') {
		printf("%i+%if: command not found\n", EXIT_STATUS, EXIT_STATUS);
		EXIT_STATUS = 127;  // Set exit status for subsequent commands
	}
}
