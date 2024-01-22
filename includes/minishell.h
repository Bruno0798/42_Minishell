/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:39:03 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/20 19:29:52 by bsousa-d         ###   ########.fr       */
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

/**
 * @brief Checks the number of command line arguments 
 * and exits if it is not equal to the expected number.
 *
 * @param argc The number of command line arguments.
 * @param valid_argc The expected number of command line arguments.
 */
void 	check_args(int argc, int valid_argc);

/**
 * Initializes the environment structure with the given environment variables.
 *
 * @param env   The environment structure to initialize.
 * @param envp  The array of environment variables.
 */
void	init_env(t_env **env,char **envp);

/**
 * Creates a new environment variable node.
 *
 * @param key   The key of the environment variable.
 * @param value The value of the environment variable.
 *
 * @return The new environment variable node.
 */
t_env	*ft_new_env(char *key, char *value);

/**
 * Prints the content and type of each token in a token list.
 *
 * @param head The head of the token list.
 */
void	ft_print_token_list(t_token *head);

/**
 * Adds a new environment variable node to the end of the environment list.
 *
 * @param env_lst The head of the environment list.
 * @param new_env The new environment variable node.
 */
void	ft_add_env_back(t_env **env_lst, t_env *new_env);

/**
 * Retrieves the value associated with a given key in the environment.
 *
 * @param env The environment linked list.
 * @param key The key to search for.
 * @return The value associated with the key, or NULL if the key is not found.
 */
char	*ft_get_value(t_env *env, char *search);

/**
 * Prints the current working directory.
 * 
 * This function retrieves the current working directory using the `getcwd` function
 * and prints it using the `ft_printf` function.
 * 
 * @note The maximum length of the current working directory is limited to 4096 characters.
 * 
 * @return void
 */
void	ft_pwd();


void ft_echo(t_token *token);

/**
 * Retrieves the PATH environment variable from the given environment list.
 * 
 * @param env_lst The linked list of environment variables.
 * @return An array of strings representing the directories in the PATH variable, 
 *         or NULL if the PATH variable is not found.
 */
char	**get_path(t_env *env_lst);
t_commands	*pipe_commands(char *str, t_env *env);
char	**ft_split2(char const *s, char c);
bool is_beetwenn_quotes(char *str);
void ft_expander(t_commands *command);

/**
 * Checks if a string consists only of spaces.
 *
 * @param str The string to check.
 * @return true if the string consists only of spaces, false otherwise.
 */
bool is_everything_space(char *str);

#endif