/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:25:43 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/06/25 17:47:37 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int init_and_set_fd(int argc, char **envp, t_env **env);

int	EXIT_STATUS;

int main(int argc, char **argv, char **envp)
{
	(void)		argv;
	t_commands	*command; // NEEDS TO BE REMOVED FROM HERE AND PASS TO PARER
	t_env		*env;
	char		*input;
	int			fd;
	int			fd1;


	fd = init_and_set_fd(argc, envp, &env);
	fd1 = dup(1);
	while (42)
	{
		command = NULL;
		handle_signals();
		dup2(fd, STDIN_FILENO);
		dup2(fd1, STDOUT_FILENO);
		input = readline("Minishell$>");
		add_history(input);
		if (is_valid_input(input, env))
			if (ft_parser(input, &command, env) == EXIT_SUCCESS)
			{
				remake_commands(command);
				open_pipes(command);
			}
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
