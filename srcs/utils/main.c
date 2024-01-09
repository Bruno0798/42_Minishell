/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:35:39 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/09 01:57:52 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int count_quotes(char *str);
void parser(t_shell *shell);

int main(int argc, char **argv, char **env)
{
	t_shell *shell = NULL;
	(void)argv;
	(void)env;
	
	if (argc != 1)
	{
		ft_printf(RED"Wrong Arguments\nUse './minishell' to start!\n"ENDC);
		return (0);
	}
	
	while (1)
	{
		shell->line = readline("Minishell$>");
		parser(shell);
	}
    return 0;
}


void parser(t_shell *shell)
{
	char pwd[4096];
	if(count_quotes(shell->line))
		exit(1);
	if(!(strcmp(shell->line, "pdw")))
	{
		getcwd(pwd, sizeof(pwd));
		ft_printf("%s\n", pwd);
		exit(1);
	}
	
}

int count_quotes(char *str)
{
	int i;
	int count = 0;
	int count_double = 0;
	
	i = 0;
	while(str[i])
	{
		if(str[i] == 34)
			count_double++;
		if(str[i] == 39)
			count++;
		i++;
	}
	if(count_double % 2 != 0 || count % 2 != 0)
		return 1;
	return 0; 
}
