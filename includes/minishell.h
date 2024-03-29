/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:05:33 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/03/29 15:44:16 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:39:03 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/03/07 12:49:34 by brunolopes       ###   ########.fr       */
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


/* Color codes for terminal output */
# define RED	"\033[1m\033[31m"
# define GREEN	"\033[1m\033[32m"
# define ENDC	"\033[0m"

/* ASCII values for double and single quotes */
# define DOUBLE_QUOTE 34
# define SINGLE_QUOTE 39

/* Error messages */
# define ERROR_TITLE "minishell"
# define ERROR_FORK "fork error"
# define ERROR_QUOTE "unclosed quotes"
# define ERROR_PIPE "syntax error near unexpected token `|'"
# define ERROR_PIPE_2 "syntax error near unexpected token '||'"
# define ERROR_PROMPT "no support for pipe prompt"
# define ERROR_DIR "No such file or directory"
# define ERROR_CMD "command not found"
# define ERROR_EXP "not a valid identifier"
# define ERROR_HOME "HOME not set"
# define ERROR_PID "pipe error"
# define ERROR_NUM "numeric argument required"
# define ERROR_ARG "too many arguments"
# define ERROR_SUPPORT "Arguments and options aren't supported"
# define ERROR_OPTIONS "options aren't supported"
# define ERROR_REDIR "syntax error near unexpected token `\\n'"
# define ERROR_SYNTAX "syntax error near unexpected token"
# define EXPORT_NOTE "too few argumnts"

/* Global variable for the exit status */
extern int	EXIT_STATUS;
/* Enum for different token types */
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

/* Function prototypes */


/************************************ ENV ************************************/

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
 * Updates the value of an environment variable.
 *
 * @param env_lst The head of the environment list.
 * @param key     The key of the environment variable to update.
 * @param replace The new value to assign to the environment variable.
 */
void		ft_update_env(t_env *env_lst, char *key, char *replace);

void free_env(t_env *env);


/*********************************** UTILS ***********************************/

/**
 * @brief Checks the number of command line arguments
 * and exits if it is not equal to the expected number.
 *
 * @param argc The number of command line arguments.
 * @param valid_argc The expected number of command line arguments.
 */
void 	check_args(int argc, int valid_argc);

/**
 * Prints the content and type of each token in a token list.
 *
 * @param head The head of the token list.
 */
void	ft_print_token_list(t_token *head);

/**
 * Prints the content of the environment list.
 *
 * @param token The head of the environment list.
 */
void ft_print_env(t_commands *command);

/**
 * @brief Prints an error message and sets the global exit status.
 *
 * This function is used to print an error message to the console. It can handle
 * different formats of the error message based on the provided parameters.
 * After printing the error message, it sets the global exit status to the provided exit code.
 *
 * @param msg The main error message to be printed. If this is NULL and key is provided,
 *            the function will print a syntax error message with the key.
 * @param key Additional information to be included in the error message. This could be
 *            the name of a command or a token that caused the error.
 * @param exit_code The exit code to be set after printing the error message.
 */
void	print_error(char *msg, char *key, int exit_code);


/**
 * @brief Counts the number of pipes in a string.
 *
 * This function counts the number of pipes in a given string. It iterates
 * over the string and increments a counter each time it finds a pipe character.
 *
 * @param str The string to count the pipes in.
 * @return The number of pipes in the string.
 */
int count_pipes(char *str);


/**
 * @brief Checks if a string has unclosed quotes.
 *
 * This function iterates over each character in the provided string. It
 * keeps track of the status of single and double quotes. If it encounters a
 * double quote character and there are no unclosed single quotes, it toggles
 * the status of double quotes. Similarly, if it encounters a single quote
 * character and there are no unclosed double quotes, it toggles the status
 * of single quotes. At the end of the string, if there are any unclosed quotes,
 * the function returns true. Otherwise, it returns false.
 *
 * @param str The string to be checked for unclosed quotes.
 * @return true if there are unclosed quotes in the string, false otherwise.
 */
bool 		is_between_quotes(char *str);


/**
 * @brief Checks if a string contains only spaces.
 *
 * This function iterates over each character in the provided string. If it
 * encounters a character that is not a space, it returns false. Otherwise, if
 * it reaches the end of the string without finding any non-space characters,
 * it returns true.
 *
 * @param str The string to be checked for spaces.
 * @return true if the string contains only spaces, false otherwise.
 */
bool	is_everything_space(char *str);

/*********************************** PARSER ***********************************/

/********************************** COMMANDS **********************************/

void ft_unset(t_commands *command);

/**
 * Prints the current working directory.
 *
 * This function retrieves the current working directory using the `getcwd` function
 * and prints it using the `printf` function.
 *
 * @note The maximum length of the current working directory is limited to 4096 characters.
 *
 * @return void
 */
void	ft_pwd();

/**
 * Prints the content of the given token list.
 *
 * @param token The head of the token list.
 */
void ft_echo(t_token *token);

void ft_export(t_commands *command);

/*********************************** SIGNALS *********************************/

void	ft_handler_ctrl(int signum);

/************************************* FREE ***********************************/

void	free_all(t_commands *command);
void	ft_exit(t_commands *command);
void	ft_free_token(t_token *token);
void	ft_free_commands(t_commands *command);
void	ft_free_env(t_env *env);






t_commands	*pipe_commands(char *str, t_env *env);
void		ft_expander(t_commands *command);
char		**get_path(t_env *env_lst);
char		**ft_split2(char const *s, char c);
char		**ft_lst_to_arr(t_token *token);
char		**ft_env_to_arr(t_env *env);
int			ft_execution(t_commands *command);
int			env_size(t_env *env);
char *ft_expand_join(char *s1, t_env *env);
char* trim_right(const char *input_string);
char *replace_substr(const char *original, const char *substr, const char
*replacement, int i);
void ft_special_handler(const char *input);
char* ft_delete_quotes(char *input);
void ft_remove_quotes(t_commands *commands);
int ft_parser(char *input, t_commands **commands, t_env *env);
void ft_execute(t_commands *command);
bool is_valid_input(char *input);
bool syntax_checker(char *input);
void ft_cleans_special_chars(t_commands *commands);
void ft_cd(t_commands *command);
bool can_execute(t_commands *command);
t_env	*ft_fnd_env(t_env *env, char *search);
bool	ft_hasSpecialChar(char *str);

void ft_create_file(char *filename);

int ft_redirect(t_commands *command);


void ft_handle_redirect(t_commands *command);
int ft_check_redirect(t_commands *command);
int ft_count_redirects(t_commands *commands);
void ft_execute(t_commands *command);



void check_fork(t_commands *command);
void handle_signals(void);
void open_pipes(t_commands *command);
bool has_double_pipe(char *input);
void check_heredoc(t_commands *command);
void ft_heredoc(char *delimiter);
t_commands *delete_redir(t_commands *command);
void heredoc_cycle(char *line);
void ft_heredoc(char *delimiter);
bool has_here_doc(t_commands *commands);
void remake_commands(t_commands *command);
void	ft_signals_heredoc(void);
bool files_exist(t_token *token);
#endif
