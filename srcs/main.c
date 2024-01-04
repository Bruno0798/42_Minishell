/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:35:39 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/04 15:23:26 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int argc, char **argv, char **env)
{
	t_shell *shell;
	t_env *envp = NULL;
	
    pid_t	pid;
    (void) argv;
    (void) argc;
	char **parmList = (char **)malloc(sizeof(char *) * 4);

	shell = (t_shell *)malloc(sizeof(t_shell));
	shell->var = (t_variables *)malloc(sizeof(t_variables));
	
	get_env(&envp, env);
	shell->var->home = ft_search_key(envp, "HOME");
	shell->var->path = ft_search_key(envp, "PATH");
	parmList[0] = "/bin/uptime";
	parmList[1] = "-p";
	parmList[2] = shell->var->home;
	parmList[3] = NULL;

	shell->var->paths = ft_split(shell->var->path, ':');

    if ((pid = fork()) == -1)
        perror("fork error");
    else if (pid == 0)
    {
        execve("/bin/uptime", parmList, env);
        perror("execve error"); // execve only returns if an error occurs
        exit(EXIT_FAILURE);     // Exit the child process on failure
    }
    else
	{
        waitpid(pid, NULL, 0); // Wait for the child process to finish
	}
	

    return 0;
}


