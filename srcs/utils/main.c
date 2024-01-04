/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:35:39 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/04 15:26:06 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


int main(int argc, char **argv, char **env)
{
	t_shell *shell;
	t_env *envp = NULL;
	
    pid_t pid;
    (void)argv;
    (void)argc;
	char **parmList = (char **)malloc(sizeof(char *) * 4);
	// char *const parmList[] = {"/bin/ls", "-l", "/home/brunolopes", NULL};
	// char *const parmList[] = {"/bin/ls", "-l", "/Users/Bruno", NULL};

	shell = (t_shell *)malloc(sizeof(t_shell));
	shell->var = (t_variables *)malloc(sizeof(t_variables));
	
	get_env(&envp, env);
	shell->var->home = ft_search_key(envp, "HOME");
	shell->var->path = ft_search_key(envp, "PATH");
	parmList[0] = "/bin/ls";
	parmList[1] = "-l";
	parmList[2] = shell->var->home;
	parmList[3] = NULL;

	shell->var->paths = ft_split(shell->var->path, ':');
	printf("%s\n", shell->var->home);
	printf("%s\n", shell->var->path);
	for(int i = 0; shell->var->paths[i]; i++)
		printf("oui: %s\n", shell->var->paths[i]);
	// printLinkedList(envp);
	// paths =  ft_split(paths[], ':');

    if ((pid = fork()) == -1)
    {
        perror("fork error");
    }
    else if (pid == 0)
    {
        execve("/bin", parmList, env);
        perror("execve error"); // execve only returns if an error occurs
        exit(EXIT_FAILURE);     // Exit the child process on failure
    }
    else
	{
        waitpid(pid, NULL, 0); // Wait for the child process to finish
	}
	

    return 0;
}


