/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:39:03 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/20 11:43:57 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>



# define RED	"\033[1m\033[31m"
# define GREEN	"\033[1m\033[32m"
# define ENDC	"\033[0m"

# define DOUBLE_QUOTE 34
# define SINGLE_QUOTE 39

typedef enum s_type
{
	pipes,
	command,
	option,
	files,
	redir_in,
	redir_in2,
	redir_out,
	redir_out2,
	error
} 		t_type;

typedef struct s_token
{
	char *content;
	t_type type;
	struct s_token *next;
}		t_token;

typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
}				t_env;

typedef struct s_commands
{
	t_env	*env;
	t_token	*token;
	struct	s_commands *next;	
}		t_commands;


void	init_env(t_env **env,char **envp);
t_env	*ft_new_env(char *key, char *value);
void	ft_print_token_list(t_token *head);
void	ft_add_env_back(t_env **env_lst, t_env *new_env);
// char	*ft_search_key(t_env *env, char *search);
void	ft_pwd();
void ft_echo(t_token *token);
char	**get_path(t_env *env_lst);
t_commands	*pipe_commands(char *str, t_env *env);
char	**ft_split2(char const *s, char c);

#endif