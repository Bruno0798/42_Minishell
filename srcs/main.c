/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:25:43 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/25 20:17:44 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" //TODO CHANGE THE NAME OF THE BURRO BRPEREIR

static int	init_and_set_fd(int argc, char **envp, t_env **env);
static void	main_cycle(int fd, int fd1, t_env *env);
void		reset_fd_signals(int fd, int fd1);

int	g_exit_status;

//TODO cat | cat is printing 2 times core dump

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	int		fd;
	int		fd1;

	(void) argv;
	fd = init_and_set_fd(argc, envp, &env);
	fd1 = dup(1);
	main_cycle(fd, fd1, env);
	return (0);
}

void	main_cycle(int fd, int fd1, t_env *env)
{
	t_commands	*command;
	char		*input;

	while (42)
	{
		command = NULL;
		reset_fd_signals(fd, fd1);
		input = readline("Minishell$>");
		add_history(input);
		if (is_valid_input(input, env))
		{
			if (ft_parser(input, &command, env) == EXIT_SUCCESS)
			{
				remake_commands(command);
				open_pipes(command);
			}
		}
		ft_free_commands(command, 1);
	}
}

void	reset_fd_signals(int fd, int fd1)
{
	ft_handle_signals(MAIN);
	dup2(fd, STDIN_FILENO);
	dup2(fd1, STDOUT_FILENO);
}

static int	init_and_set_fd(int argc, char **envp, t_env **env)
{
	int	fd;

	fd = dup(STDIN_FILENO);
	check_args(argc, 1);
	init_env(env, envp);
	return (fd);
}
