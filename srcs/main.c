/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:35:39 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/03 18:05:30 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void get_env(t_env **env,char **envp);
t_env	*ft_new_env(char *key, char *value);
void printLinkedList(t_env *head);
void	ft_add_env_back(t_env **env_lst, t_env *new_env);
char *ft_search_key(t_env *env, char *search);

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

void get_env(t_env **env,char **envp)
{
	int i;
	char *key;
	char *value;
	char *equal_sign;

	i = 0;
	while(envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		key = ft_substr(envp[i], 0, equal_sign - envp[i]);
		value = equal_sign + 1;
		ft_add_env_back(env, ft_new_env(key, value));
		i++;
	}
}

t_env	*ft_new_env(char *key, char *value)
{
	t_env	*new_node;

	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	if (!new_node->key)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}


void printLinkedList(t_env *head) 
{
    t_env *current = head;

    while (current != NULL) {
        printf("Key: %s, Value: %s\n", current->key, current->value);
        current = current->next;
    }
}

char *ft_search_key(t_env *env, char *search)
{
	t_env *current = env;

	while(current != NULL)
	{
		if (!ft_strcmp(current->key, search))
			return (current->value);
		current = current->next;
	}
	return (NULL);
} 

void	ft_add_env_back(t_env **env_lst, t_env *new_env)
{
	t_env	*current;

	if (!*env_lst)
	{
		*env_lst = new_env;
		return ;
	}
	current = *env_lst;
	while (current && current->next)
		current = current->next;
	current->next = new_env;
}