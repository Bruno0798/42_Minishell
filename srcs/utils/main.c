/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:35:39 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/09 03:23:01 by brunolopes       ###   ########.fr       */
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
	
	shell = malloc(sizeof(t_shell));
	if (argc != 1)
	{
		ft_printf(RED"Wrong Arguments\nUse './minishell' to start!\n"ENDC);
		return (0);
	}
	
	while (1)
	{
		shell->input = readline("Minishell$>");
		parser(shell);
	}
    return 0;
}


void parser(t_shell *shell)
{
	if(count_quotes(shell->input))
		exit(1);
	if(!(strcmp(shell->input, "pwd")))
        ft_pwd();
}

int count_quotes(char *str)
{
	int i;
	int count;
    int s_quote;
    int d_quote;
	
	i = -1;
    count = 0;
    d_quote = 0;
    s_quote = 0;
	while(str[++i])
		if(str[i] == 34 && !s_quote)
        {
            d_quote = !d_quote;
			count++;
        }
		else if(str[i] == 39 && !d_quote)
        {
            s_quote =  !s_quote;
			count++;
        }
	if(count % 2 != 0)
		return 1;
	return 0; 
}
