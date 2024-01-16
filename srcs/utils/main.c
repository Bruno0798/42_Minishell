/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:35:39 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/16 17:00:56 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int count_quotes(char *str);
void parser(t_shell shell);

int main(int argc, char **argv, char **envp)
{
	(void)	argv;
	t_shell	shell;
	t_env *env;

	if (argc != 1)
	{
		ft_printf(RED"Wrong Arguments\nUse './minishell' to start!\n"ENDC);
		return (0);
	}
	ft_bzero(&shell, sizeof(t_shell));
	get_env(env, envp);
	shell.paths = get_path(env);
	while (1)
	{
		shell.input = readline("Minishell$>");
		char **something = split_pipe(shell.input);
		ft_printf("%s \n%s \n", something[0], something[1]);
		parser(shell);

	}
	return 0;
}


void parser(t_shell shell)
{
	if(count_quotes(shell.input))
		exit(1);
	if(!(strcmp(shell.input, "pwd")))
		ft_pwd();
	if(!(strncmp(shell.input, "echo", 4)))
		ft_echo(shell.input + 4);
}

int count_quotes(char *str)
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

