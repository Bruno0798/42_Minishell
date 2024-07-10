/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 10:25:44 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/08 19:34:14 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_shlvl(t_env **env);
t_env	*get_env_node(t_env *env, char *key);

char	*extract_home_path(const char *pwd)
{
	char	*home;
	char	*third_slash;
	size_t	length;

	home = NULL;
	third_slash = ft_strchr(pwd, '/');
	if (third_slash)
	{
		third_slash = ft_strchr(third_slash + 1, '/');
		if (third_slash)
		{
			third_slash = ft_strchr(third_slash + 1, '/');
			if (third_slash)
			{
				length = third_slash - pwd;
				home = malloc(length + 1);
				if (!home)
					return (NULL);
				ft_strlcpy(home, pwd, length);
				home[length] = '\0';
			}
		}
	}
	return (home);
}

void	init_env_from_system(t_env **env)
{
	char	pwd[4086];
	char	*value;
	int		fd;
	char	*path_value;

	value = ft_strdup(getcwd(pwd, sizeof(pwd)));
	ft_add_env_back(env, ft_new_env("_", "/usr/bin/env", 1));
	ft_add_env_back(env, ft_new_env("PWD", value, 1));
	fd = open("/etc/environment", O_RDONLY);
	if (fd != -1)
	{
		path_value = get_next_line(fd);
		ft_add_env_back(env, ft_new_env("PATH", path_value, 3));
		free(path_value);
		close(fd);
	}
	free(value);
}

void	init_env(t_env **env, char **envp)
{
	int		i;
	char	*equal_sign;
	char	*key;
	char	*value;

	ft_bzero(env, sizeof(t_env));
	if (envp[0] == NULL)
		init_env_from_system(env);
	else
	{
		i = 0;
		while (envp[i])
		{
			equal_sign = ft_strchr(envp[i], '=');
			key = ft_substr(envp[i], 0, equal_sign - envp[i]);
			value = equal_sign + 1;
			ft_add_env_back(env, ft_new_env(key, value, 1));
			free(key);
			i++;
		}
	}
	update_shlvl(env);
}

void	update_shlvl(t_env **env)
{
	t_env	*shlvl_node;
	int		curr_shlvl_int;
	char	*next_shlvl;

	shlvl_node = get_env_node(*env, "SHLVL");
	if (shlvl_node == NULL)
	{
		ft_add_env_back(env, ft_new_env("SHLVL", "1", 1));
		return ;
	}
	curr_shlvl_int = ft_atoi(shlvl_node->value);
	curr_shlvl_int++;
	next_shlvl = ft_itoa(curr_shlvl_int);
	if (next_shlvl == NULL)
		return ;
	free(shlvl_node->value);
	shlvl_node->value = next_shlvl;
}

t_env	*get_env_node(t_env *env, char *key)
{
	while (env != NULL)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}
