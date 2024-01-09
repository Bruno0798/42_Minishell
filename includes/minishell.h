/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:39:03 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/09 03:07:30 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# define RED	"\033[1m\033[31m"
# define GREEN	"\033[1m\033[32m"
# define ENDC	"\033[0m"

typedef enum s_type
{
	pipes,
	command,
	files
} 		t_type;

typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
}				t_env;

typedef struct s_variables
{
	char	*home;
	char	*path;
	char	**paths;
}				t_variables;

typedef struct s_shell
{
	t_list		*env;
	t_variables	*var;
	char		*input;
}				t_shell;


void get_env(t_env **env,char **envp);
t_env	*ft_new_env(char *key, char *value);
void printLinkedList(t_env *head);
void	ft_add_env_back(t_env **env_lst, t_env *new_env);
char *ft_search_key(t_env *env, char *search);
void ft_pwd();

#endif