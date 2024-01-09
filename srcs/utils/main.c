/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:35:39 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/09 01:50:19 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int main(int argc, char **argv, char **env)
{
	t_shell *shell;
	t_env *envp = NULL;
	
    // pid_t	pid;
    (void) argv;
    (void) argc;
	// char **parmList = (char **)malloc(sizeof(char *) * 2);
    char *text;

	shell = (t_shell *)malloc(sizeof(t_shell));
	shell->var = (t_variables *)malloc(sizeof(t_variables));
	
	get_env(&envp, env);
    while(1)
    {
        text = readline("Shell: ");
        if (!ft_strcmp(text, "exit"))
            exit(1);
    }
	// shell->var->home = ft_search_key(envp, "HOME");
	// shell->var->path = ft_search_key(envp, "PATH");
	// parmList[0] = "uptime";
	// parmList[1] = NULL;

	// shell->var->paths = ft_split(shell->var->path, ':');

    // if ((pid = fork()) == -1)
    //     perror("fork error");
    // else if (pid == 0)
    // {
    //     execve("/bin/uptime", parmList, env);
    //     perror("execve error"); // execve only returns if an error occurs
    //     exit(EXIT_FAILURE);     // Exit the child process on failure
    // }
    // else
	// {
    //     waitpid(pid, NULL, 0); // Wait for the child process to finish
	// }
    return 0;
}


