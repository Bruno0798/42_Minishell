/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:35:39 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/03 16:09:50 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void get_env(t_env **env,char **envp);
t_env	*ft_new_env(char *key, char *value);
void printLinkedList(t_env *head);
void	ft_add_env_back(t_env **env_lst, t_env *new_env);

int main(int argc, char **argv, char **env)
{
	t_env *envp = NULL;
	
    pid_t pid;
    (void)argv;
    (void)argc;
	char *const parmList[] = {"/bin/ls", "-l", "/Users/bruno", NULL};

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
        waitpid(pid, NULL, 0); // Wait for the child process to finish

	get_env(&envp, env);
	printLinkedList(envp);
	

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