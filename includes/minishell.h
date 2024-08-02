/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:57:05 by brpereir          #+#    #+#             */
/*   Updated: 2024/08/02 02:37:57 by bsousa-d         ###   ########.fr       */
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
# define ERROR_TITLE "Minishell"
# define ERROR_FORK " fork error"
# define ERROR_QUOTE " unclosed quotes"
# define ERROR_PIPE " syntax error near unexpected token `|'"
# define ERROR_PIPE_2 " syntax error near unexpected token '||'"
# define ERROR_PROMPT " no support for pipe prompt"
# define ERROR_DIR " No such file or directory"
# define ERROR_CMD "command not found"
# define ERROR_EXP " not a valid identifier"
# define ERROR_HOME "HOME not set"
# define ERROR_PID " pipe error"
# define ERROR_NUM " numeric argument required"
# define ERROR_ARG " too many arguments"
# define ERROR_SUPPORT "Arguments and options aren't supported"
# define ERROR_OPTIONS " invalid option"
# define ERROR_REDIR " syntax error near unexpected token `newline'"
# define ERROR_SYNTAX " syntax error near unexpected token"
# define EXPORT_NOTE "too few argumnts"
# define EXECUTION_PERMISSION "permission denied"
# define DIRECTORY_EXISTS "is a directory"
# define COMMAND_NOT_FOUND "command not found"

/* Global variable for the exit status */
extern int	g_exit_status;

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
	error,
	MAIN,
	CHILD,
	PIPE,
	HERE_DOC,
	IGNORE
}		t_type;

typedef struct s_token
{
	char			*content;
	t_type			type;
	struct s_token	*next;
}		t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				visible;
	struct s_env	*next;
}				t_env;

typedef struct s_commands
{
	t_env				*env;
	t_token				*token;
	struct s_commands	*next;	
}		t_commands;

/* Function prototypes */

/*********************************** ENV ************************************/

/**
 * \brief Initializes the environment variables from the given environment
 * array or the system.
 *
 * This function initializes the environment linked list from the provided
 * environment array `envp`.
 * If `envp` is empty, it initializes the environment from the system.
 * It also updates the shell level (`SHLVL`) environment variable.
 *
 * \param env A double pointer to the environment linked list.
 * \param envp An array of strings representing the environment variables in the
 *			format "KEY=VALUE".
 */
void		init_env(t_env **env, char **envp);

/**
 * \brief Creates a new environment variable node.
 *
 * This function allocates memory for a new `t_env` node and initializes it
 * with the provided key,
 * value, and visibility status. If the allocation fails or the key is NULL,
 * it returns NULL.
 *
 * \param key A string representing the key of the environment variable.
 * \param value A string representing the value of the environment variable.
 *				If NULL, the value is set to NULL.
 * \param visible An integer indicating whether the environment variable is
 *				visible (1) or not (0).
 * \return A pointer to the newly created `t_env` node, or NULL if the allocation
 *				fails or the key is NULL.
 */
t_env		*ft_new_env(char *key, char *value, int visible);

/**
 * \brief Adds a new environment variable node to the end of the environment
 *		list.
 *
 * This function appends a new `t_env` node to the end of the linked list of
 * environment variables.
 * If the list is empty, the new node becomes the first node in the list.
 *
 * \param env_lst A double pointer to the head of the environment list.
 * \param new_env A pointer to the new environment variable node to be added.
 */
void		ft_add_env_back(t_env **env_lst, t_env *new_env);

/**
 * \brief Retrieves the value of an environment variable by its key.
 *
 * This function searches through the linked list of environment variables
 * (`t_env`)
 * and returns the value associated with the specified key.
 *
 * \param env A pointer to the head of the environment variables linked list.
 * \param key A constant string representing the key of the environment variable
 *			to search for.
 * \return A pointer to the value string of the environment variable if found,
 *			or NULL if the key is not found.
 */
char		*ft_get_value(t_env *env, char const *key);

/**
 * \brief Updates the value and visibility of an environment variable.
 *
 * This function searches for an environment variable by its key in the linked
 * list of environment variables.
 * If the key is found, it updates the value and visibility status of the
 * environment variable.
 *
 * \param env_lst A pointer to the head of the environment variables linked
 * list.
 * \param key A string representing the key of the environment variable to
 * update.
 * \param replace A string representing the new value for the environment
 * variable.
 * \param visible An integer indicating whether the environment variable
 * is visible (1) or not (0).
 */
void		ft_update_env(t_env *env_lst, char *key,
				char *replace, int visible);

/**
 * \brief Frees the memory allocated for the environment variables linked list.
 *
 * This function iterates through the linked list of environment variables
 * (`t_env`),
 * freeing the memory allocated for each node, including the key and value
 * strings.
 *
 * \param env A pointer to the head of the environment variables linked list.
 */
void		free_env(t_env *env);

/**
 * \brief Prints the list of tokens.
 *
 * This function iterates through the linked list of tokens (`t_token`)
 * and prints the content and type of each token.
 *
 * \param head A pointer to the head of the token linked list.
 */
void		ft_print_token_list(t_token *head);

/**
 * \brief Prints the environment variables.
 *
 * This function prints the environment variables stored in the `command`
 * structure.
 * It first checks if the environment list is empty or if the PATH variable is
 * missing.
 * If there are additional tokens, it handles errors related to options and
 * directories.
 * Finally, it iterates through the environment list and prints each visible
 * environment variable.
 *
 * \param command A pointer to the `t_commands` structure containing the
 *				environment list and tokens.
 */
void		ft_print_env(t_commands *command);

/********************************** UTILS ***********************************/

/**
 * \brief Checks the number of command-line arguments.
 *
 * This function verifies if the number of command-line arguments (`argc`)
 * matches the expected number (`valid_argc`). If the numbers do not match,
 * it prints an error message and exits the program.
 *
 * \param argc The number of command-line arguments passed to the program.
 * \param valid_argc The expected number of command-line arguments.
 */
void		check_args(int argc, int valid_argc);

/**
 * \brief Prints an error message and sets the exit status.
 *
 * This function prints an error message to the standard error stream.
 * If a key is provided, it includes the key in the error message.
 * If a message is provided, it includes the message in the error message.
 * It also sets the global exit status to the provided exit code.
 *
 * \param msg A string containing the error message.
 * \param key A string containing the key related to the error (can be NULL).
 * \param exit_code An integer representing the exit code to set.
 */
void		print_error(char *msg, char *key, int exit_code);

/********************************** PARSER **********************************/

/**
 * \brief Checks if a string is enclosed in quotes.
 *
 * This function iterates through the given string and toggles the state of
 * single and double quotes.
 * It returns true if the string is properly enclosed in quotes, and false
 * otherwise.
 *
 * \param str A constant character pointer to the string to be checked.
 * \return true if the string is properly enclosed in quotes, false otherwise.
 */
bool		is_between_quotes(char const *str);

/**
 * \brief Checks if a string contains only spaces or tabs.
 *
 * This function iterates through the given string and replaces tabs with spaces
 * if they are not enclosed in quotes. It then checks if the string contains
 * only spaces or tabs and returns true if it does, and false otherwise.
 *
 * \param str A pointer to the string to be checked.
 * \return true if the string contains only spaces or tabs, false otherwise.
 */
bool		is_everything_space(char *str);

/********************************** BUILTINS ********************************/

/**
 * \brief Unsets an environment variable.
 *
 * This function removes an environment variable from the environment list
 * stored in the `command` structure. It first checks if the command is invalid
 * using the `is_invalid_unset_command` function. If the command is valid, it
 * iterates through the environment list to find the variable to unset and
 * removes it using the `remove_env_node` function.
 *
 * \param command A pointer to the `t_commands` structure containing the
 *				environment list and tokens.
 */
void		ft_unset(t_commands *command);

/**
 * \brief Prints the current working directory.
 *
 * This function retrieves and prints the current working directory.
 * If the next token in the command contains an option (starts with '-'),
 * it prints an error message and exits the function.
 *
 * \param command A pointer to the `t_commands` structure containing the command
 * tokens.
 */
void		ft_pwd(t_commands *command);

/**
 * \brief Executes the echo command.
 *
 * This function processes the given token list to handle the echo command.
 * It checks for options, prints the token content, and appends a newline
 * if no options are provided.
 *
 * \param token A pointer to the `t_token` structure containing the command
 * tokens.
 */
void		ft_echo(t_token *token);

/**
 * \brief Handles the export command.
 *
 * This function processes the export command by iterating through the tokens
 * and handling each token appropriately. If no tokens are provided, it prints
 * the current environment variables.
 *
 * \param command A pointer to the `t_commands` structure containing the command
 * tokens and environment list.
 */
void		ft_export(t_commands *command);

/************************************ FREE **********************************/

/**
 * \brief Exits the program with the appropriate status.
 *
 * This function handles the exit command by checking the next token and
 * determining the exit status. If no next token is present, it frees the
 * commands and exits with the global exit status.
 *
 * \param command A pointer to the `t_commands` structure containing the command
 * tokens.
 * \param head A pointer to the `t_commands` structure representing the head of
 * the command list.
 */
void		ft_exit(t_commands *command, t_commands *head);

/**
 * \brief Frees all allocated memory for commands and optionally exits.
 *
 * This function frees all allocated memory for the given commands structure
 * and its associated tokens and environment variables based on the specified
 * option.
 *
 * \param command A pointer to the `t_commands` structure containing the
 *	commands.
 * \param option An integer specifying the level of memory to free:
 *               - 0: Free tokens only.
 *               - 1: Free tokens and commands.
 *               - 2: Free tokens, commands, and environment variables.
 *               - 3: Free tokens, commands, environment variables, and exit the
 *               program.
 */
void		ft_free_commands(t_commands *command, int option);

/**
 * \brief Splits a command string into individual commands based on pipes.
 *
 * This function takes a command string, splits it by the pipe character '|',
 * and creates a linked list of `t_commands` structures for each command.
 *
 * \param str The command string to be split.
 * \param env A pointer to the `t_env` structure containing the environment
 *			variables.
 * \return A pointer to the head of the linked list of `t_commands` structures.
 */
t_commands	*pipe_commands(char *str, t_env *env);

/**
 * \brief Expands tokens within a linked list of commands.
 *
 * This function iterates through a linked list of `t_commands` structures,
 * and for each command, it processes and expands its tokens.
 *
 * \param commands A pointer to the head of the linked list of `t_commands`
 *				structures.
 */
void		ft_expander(t_commands *commands);

/**
 * \brief Retrieves the system PATH variable from the environment list.
 *
 * This function searches through a linked list of environment variables
 * to find the PATH variable and splits its value into an array of paths.
 *
 * \param env_lst A pointer to the head of the linked list of environment
 *				variables.
 * \return An array of strings representing the paths in the PATH variable, or
 * NULL if not found.
 */
char		**get_path(t_env *env_lst);

/**
 * \brief Splits a string into an array of words based on a delimiter.
 *
 * This function takes a string and a delimiter character, and splits the string
 * into an array of words. Memory for the array and the words is dynamically
 * allocated.
 *
 * \param s The input string to be split.
 * \param c The delimiter character used to split the string.
 * \return An array of strings (words) or NULL if memory allocation fails or
 * if the input string is NULL.
 */
char		**ft_split2(char const *s, char c);

/**
 * \brief Converts a linked list of tokens to an array of strings.
 *
 * This function takes a linked list of `t_token` structures and converts it
 * into an array of strings, where each string is a duplicate of the token's
 * content.
 * Memory for the array and the strings is dynamically allocated.
 *
 * \param token A pointer to the head of the linked list of tokens.
 * \return An array of strings representing the token contents, or NULL if
 * memory allocation fails.
 */
char		**ft_lst_to_arr(t_token *token);

/**
 * \brief Converts a linked list of environment variables to an array of strings.
 *
 * This function takes a linked list of `t_env` structures and converts it
 * into an array of strings, where each string is a key-value pair in the format
 * "key=value".
 * Memory for the array and the strings is dynamically allocated.
 *
 * \param env A pointer to the head of the linked list of environment variables.
 * \return An array of strings representing the environment variables, or NULL
 * if memory allocation fails.
 */
char		**ft_env_to_arr(t_env *env);

/**
 * \brief Executes a command based on its token content.
 *
 * This function determines whether the command should be executed as an
 * absolute path
 * or as a regular command by checking the content of the command's token.
 *
 * \param command A pointer to the `t_commands` structure representing the
 * command to be executed.
 * \param head A pointer to the head of the linked list of commands.
 * \return Always returns 1.
 */
int			ft_execution(t_commands *command, t_commands *head);

/**
 * \brief Calculates the size of the environment linked list.
 *
 * This function iterates through a linked list of `t_env` structures
 * and counts the number of elements in the list.
 *
 * \param env A pointer to the head of the linked list of environment variables.
 * \return The number of elements in the linked list.
 */
int			env_size(t_env *env);

/**
 * \brief Removes quotes from a given string.
 *
 * This function takes an input string and removes all single and double quotes
 * from it.
 * Memory for the new string is dynamically allocated.
 *
 * \param input The input string from which quotes are to be removed.
 * \return A new string with all quotes removed. The original input string is
 * freed.
 */
char		*ft_delete_quotes(char *input);

/**
 * \brief Removes quotes from the content of specific tokens in a linked list of
 * commands.
 *
 * This function iterates through a linked list of `t_commands` structures and
 * removes quotes
 * from the content of tokens that are of type `command` or `files` and are
 * enclosed in quotes.
 *
 * \param commands A pointer to the head of the linked list of commands.
 */
void		ft_remove_quotes(t_commands *commands);

/**
 * \brief Parses the input command string and processes it into a list of
 * commands.
 *
 * This function takes an input string representing a command, parses it into a
 * linked list
 * of `t_commands` structures, and performs various processing steps such as
 * syntax checking,
 * expanding variables, and removing quotes.
 *
 * \param input The input command string to be parsed.
 * \param commands A double pointer to the head of the linked list of commands.
 * \param env A pointer to the head of the linked list of environment variables.
 * \return Returns `EXIT_SUCCESS` on successful parsing and processing, or
 * `EXIT_FAILURE` on error.
 */
int			ft_parser(char *input, t_commands **commands, t_env *env);

/**
 * \brief Executes a command based on its token content.
 *
 * This function checks for redirections and handles them if present. It then
 * determines the type of command (built-in or external) and executes it
 * accordingly.
 *
 * \param command A pointer to the `t_commands` structure representing the
 * command to be executed.
 * \param head A pointer to the head of the linked list of commands.
 */
void		ft_execute(t_commands *command, t_commands *head);

/**
 * \brief Validates the input command string.
 *
 * This function checks if the input command string is valid by performing
 * several checks:
 * - Ensures the input is not NULL.
 * - Checks if the input contains only whitespace.
 * - Validates the syntax of the input.
 * - Checks for unmatched quotes in the input.
 * - Ensures there are no invalid pipe characters in the input.
 *
 * \param input The input command string to be validated.
 * \param env A pointer to the head of the linked list of environment variables.
 * \return Returns `true` if the input is valid, otherwise returns `false`.
 */
bool		is_valid_input(char *input, t_env *env);

/**
 * \brief Checks the syntax of the input command string.
 *
 * This function validates the syntax of the input command string by performing
 * several checks:
 * - Trims leading and trailing whitespace and tabs.
 * - Ensures the input does not end with a pipe or redirection character.
 * - Ensures the input does not start with a pipe.
 * - Ensures there are no consecutive pipe characters.
 * - Ensures there are no unmatched redirection characters.
 *
 * \param input The input command string to be checked.
 * \return Returns `true` if the syntax is valid, otherwise returns `false`.
 */
bool		syntax_checker(char *input);

/**
 * \brief Changes the current working directory.
 *
 * This function changes the current working directory based on the provided
 * command.
 * It handles various cases such as navigating to the home directory,
 * handling errors,
 * and updating environment variables.
 *
 * \param commands A pointer to the `t_commands` structure containing the
 * command information.
 */
void		ft_cd(t_commands *commands);

/**
 * \brief Determines if the command can be executed and changes the directory
 * accordingly.
 *
 * This function checks if the command can be executed by evaluating the content
 * of the next token.
 * It handles special cases such as navigating to the previous directory (`-`)
 * or the home directory (`--`).
 * If the command can be executed, it changes the current working directory.
 *
 * \param command A pointer to the `t_commands` structure containing the command
 * information.
 * \return Returns `true` if the directory change was successful, otherwise
 * returns `false`.
 */
bool		can_execute(t_commands *command);

/**
 * \brief Finds an environment variable in the linked list.
 *
 * This function searches for an environment variable in the linked list by its
 * key.
 * If the key is found, it returns a pointer to the corresponding `t_env`
 * structure.
 * If the key is not found or the search key is NULL, it returns NULL.
 *
 * \param env A pointer to the head of the linked list of environment variables.
 * \param search The key of the environment variable to search for.
 * \return A pointer to the `t_env` structure if the key is found, otherwise
 * NULL.
 */
t_env		*ft_fnd_env(t_env *env, char *search);

/**
 * \brief Checks if a string contains any special characters.
 *
 * This function iterates through the given string and checks if it contains any
 * characters
 * that are not alphanumeric or underscores. If such a character is found, the
 * function returns `true`.
 *
 * \param str The string to be checked for special characters.
 * \return Returns `true` if the string contains special characters, otherwise
 * returns `false`.
 */
bool		ft_has_special_char(char *str);

/**
 * \brief Handles the redirection tokens in the command structure.
 *
 * This function processes the tokens in the `commands` structure, specifically
 * handling
 * command and option tokens. It creates a new linked list of tokens containing
 * only
 * command and option tokens, and then updates the `commands` structure to use
 * this new list.
 *
 * \param commands A pointer to the `t_commands` structure containing the
 * command information.
 */
void		ft_handle_redirect(t_commands *commands);

/**
 * \brief Checks and handles redirection tokens in the command structure.
 *
 * This function iterates through the tokens in the `command` structure and
 * checks for redirection tokens.
 * If an error token is found, it prints an error message and returns 0.
 * If a redirection token is found, it calls the `handle_redirection` function
 * to process it.
 *
 * \param command A pointer to the `t_commands` structure containing the command
 * information.
 * \return Returns 1 if all redirection tokens are handled successfully,
 * otherwise returns 0.
 */
int			ft_check_redirect(t_commands *command);

/**
 * \brief Opens pipes and manages the execution of commands.
 *
 * This function determines the number of commands to be executed and sets up
 * pipes accordingly.
 * If there is more than one command, it duplicates the standard input file
 * descriptor,
 * creates a child process to handle the commands, and then manages the parent
 * process.
 * If there is only one command, it executes the command directly.
 *
 * \param command A pointer to the `t_commands` structure containing the command
 * information.
 */
void		open_pipes(t_commands *command);

/**
 * \brief Checks for heredoc redirection in the command structure.
 *
 * This function iterates through the tokens in the `command` structure to check
 * for heredoc redirection tokens.
 * If a heredoc redirection token is found, it calls the `ft_heredoc` function
 * to process it and sets the flag to 1.
 * After processing, it deletes the redirection tokens from the command
 * structure.
 *
 * \param command A pointer to the `t_commands` structure containing the command
 * information.
 * \param head A pointer to the head of the `t_commands` structure.
 * \return Returns 1 if a heredoc redirection token is found and processed,
 * otherwise returns 0.
 */
int			check_heredoc(t_commands *command, t_commands *head);

/**
 * \brief Handles the heredoc redirection for a given command.
 *
 * This function sets up the necessary signals and forks a new process to handle
 * the heredoc redirection.
 * The child process runs the `heredoc_cycle` function to process the heredoc
 * input and then exits.
 * The parent process waits for the child process to complete and updates the
 * global exit status.
 *
 * \param delimiter The delimiter string used to end the heredoc input.
 * \param command A pointer to the `t_commands` structure containing the command
 * information.
 * \param head A pointer to the head of the `t_commands` structure.
 */
void		ft_heredoc(char *delimiter, t_commands *command, t_commands *head);

/**
 * \brief Deletes heredoc redirection tokens and updates the command structure.
 *
 * This function iterates through the tokens in the `command` structure to find
 * heredoc redirection tokens.
 * When a heredoc redirection token is found, it changes the token type to a
 * standard input redirection,
 * frees the content of the next token, and sets it to "heredoc.txt".
 *
 * \param command A pointer to the `t_commands` structure containing the command
 * information.
 * \return Returns the updated `t_commands` structure.
 */
t_commands	*delete_redir(t_commands *command);

/**
 * \brief Handles the heredoc input cycle for a given command.
 *
 * This function opens a file named "heredoc.txt" for writing and enters a loop
 * to read lines from the user.
 * It continues to read lines until the user inputs the delimiter string or an
 * end-of-file (EOF) is encountered.
 * If a line contains a variable (indicated by a '$' character), it expands the
 * variable before writing the line to the file.
 *
 * \param line The delimiter string used to end the heredoc input.
 * \param commands A pointer to the `t_commands` structure containing the
 * command information.
 */
void		heredoc_cycle(char *line, t_commands *commands);

/**
 * \brief Checks if the command structure contains a heredoc redirection.
 *
 * This function iterates through the tokens in the `commands` structure to
 * check for heredoc redirection tokens.
 * If a heredoc redirection token is found, it returns true.
 *
 * \param commands A pointer to the `t_commands` structure containing the
 * command information.
 * \return Returns true if a heredoc redirection token is found, otherwise
 * returns false.
 */
bool		has_here_doc(t_commands *commands);

/**
 * \brief Remakes the command structure by processing heredoc files.
 *
 * This function processes the heredoc files for the given command structure.
 * It calls the `heredoc_files` function twice with different parameters to
 * handle
 * the heredoc files appropriately.
 *
 * \param command A pointer to the `t_commands` structure containing the command
 * information.
 */
void		remake_commands(t_commands *command);

/**
 * \brief Checks if the files specified in the tokens exist.
 *
 * This function iterates through the tokens to check if the files specified by
 * input redirection tokens exist.
 * It opens each file in read-only mode and sets the `exist` flag to false if
 * any file cannot be opened.
 *
 * \param token A pointer to the `t_token` structure containing the tokens.
 * \return Returns true if all files exist, otherwise returns false.
 */
bool		files_exist(t_token *token);

/**
 * \brief Processes heredoc files for the given command structure.
 *
 * This function processes the heredoc files for the given command structure
 * based on the provided flag.
 * If the flag is 1, it checks for heredoc redirections and processes them.
 * Otherwise, it checks if the files exist.
 * If a heredoc redirection is found, it duplicates the file descriptor of
 * "heredoc.txt" to the standard input.
 *
 * \param command A pointer to the `t_commands` structure containing the command
 * information.
 * \param flag An integer flag indicating the operation mode (1 for checking
 * heredoc, otherwise checking file existence).
 */
void		heredoc_files(t_commands *command, int flag);

/**
 * \brief Expands variables in the command tokens.
 *
 * This function iterates through the tokens in the `commands` structure and
 * expands any variables found in the token content.
 * If the content of a token is changed after expansion, the old content is
 * freed and replaced with the new expanded content.
 *
 * \param commands A pointer to the `t_commands` structure containing the
 * command information.
 */
void		ft_expand_others(t_commands *commands);

/**
 * \brief Handles the parent process after forking.
 *
 * This function is responsible for handling the parent process after a fork.
 * It closes the input file descriptor, waits for the child process to finish,
 * and updates the global exit status based on the child's exit status.
 *
 * \param fd_in The file descriptor for the input.
 * \param count_pipes The number of pipes to wait for.
 * \param fork_pid The process ID of the forked child process.
 */
void		parent_process(int fd_in, int count_pipes, int fork_pid);

/**
 * \brief Handles the child process creation and execution for a series of
 * commands.
 *
 * This function creates a child process for each command in the command list.
 * It sets up the necessary pipes, forks the process, and executes the command
 * in the child process.
 * The function also handles signal management and resource cleanup.
 *
 * \param command A pointer to the `t_commands` structure containing the command
 * information.
 * \param fd_in The file descriptor for the input.
 * \param command_count The total number of commands to be executed.
 * \return Returns the process ID of the last forked child process.
 */
int			child_process(t_commands *command, int fd_in, int command_count);

/**
 * \brief Counts the number of commands in the command list.
 *
 * This function iterates through the linked list of commands and counts the
 * total number of commands.
 *
 * \param command A pointer to the `t_commands` structure containing the command
 * list.
 * \return Returns the total number of commands in the list.
 */
int			count_commands(t_commands *command);

/**
 * \brief Frees a double pointer array.
 *
 * This function iterates through a double pointer array and frees each element.
 * After freeing all elements, it frees the array itself.
 *
 * \param arr A double pointer array to be freed.
 */
void		free_double_pointer_array(char **arr);

/**
 * \brief Checks if a string is between single quotes.
 *
 * This function iterates through the given string and checks if there are any
 * unmatched single quotes.
 * It also considers double quotes and ensures that single quotes within double
 * quotes are not counted.
 *
 * \param str The string to be checked.
 * \return Returns true if there are unmatched single quotes, otherwise false.
 */
bool		is_between_single_quotes(char *str);

/**
 * \brief Checks the permissions of a given file or command.
 *
 * This function checks the existence and execution permissions of a given file
 * or command.
 * It prints appropriate error messages based on the permission checks and exits
 * the program
 * with the corresponding exit status.
 *
 * \param command A pointer to the `t_commands` structure containing the command
 * information.
 * \param string The file or command string to be checked.
 */
void		check_permissions(t_commands *command, char *string);

/**
 * \brief Adds a new token to the end of the token list.
 *
 * This function appends a new token to the end of the linked list of tokens.
 * If the token list is empty, the new token becomes the head of the list.
 *
 * \param token A double pointer to the head of the token list.
 * \param new A pointer to the new token to be added.
 */
void		ft_token_addback(t_token **token, t_token *new);

/**
 * \brief Frees a linked list of tokens.
 *
 * This function iterates through a linked list of tokens and frees the memory
 * allocated for each token and its content.
 *
 * \param token A pointer to the head of the token list to be freed.
 */
void		free_tokens(t_token *token);

/**
 * \brief Expands heredoc tokens in the command list.
 *
 * This function iterates through the list of commands and processes each
 * command's
 * heredoc tokens by expanding any variables found within them.
 *
 * \param commands A pointer to the head of the command list.
 */
void		ft_expander_heredoc(t_commands *commands);

/**
 * \brief Checks if the input string needs expansion.
 *
 * This function iterates through the input string and checks if any part of it
 * needs to be expanded. It handles single and double quotes to determine the
 * context in which expansion should occur.
 *
 * \param input The input string to be checked and potentially expanded.
 * \param command A pointer to the `t_commands` structure containing the command
 * information.
 * \return Returns the potentially expanded input string.
 */
char		*needs_expansion(char *input, t_commands *command);

/**
 * \brief Expands a variable in the given string.
 *
 * This function takes a string and an index, extracts the variable name
 * starting
 * from the given index, retrieves its value from the environment, and replaces
 * the variable in the string with its value.
 *
 * \param string The input string containing the variable to be expanded.
 * \param i The index in the string where the variable starts.
 * \param commands A pointer to the `t_commands` structure containing the
 * environment.
 * \return Returns the new string with the variable expanded.
 */
char		*expand_variable(char *string, int i, t_commands *commands);

/**
 * \brief Expands the exit code in the given string.
 *
 * This function replaces occurrences of the exit code variable `$?` in the
 * input string
 * with the actual exit code value. It allocates a new string with the expanded
 * value.
 *
 * \param string The input string containing the exit code variable to be
 * expanded.
 * \return Returns the new string with the exit code expanded, or NULL if an
 * error occurs.
 */
char		*expand_exit_code(char *string);

/**
 * \brief Checks if a dollar sign is outside single quotes in a string.
 *
 * This function iterates through the input string and determines if there is a
 * dollar sign ('$')
 * that is not enclosed within single quotes. It also handles double quotes to
 * ensure proper context.
 *
 * \param str The input string to be checked.
 * \return Returns true if a dollar sign is found outside single quotes, false
 * otherwise.
 */
bool		is_dollar_outside_single_quotes(char *str);

/**
 * \brief Handles the SIGINT signal for the main process.
 *
 * This function is called when the SIGINT signal is received by the main
 * process.
 * It clears the current input line, writes a newline to the standard error,
 * and updates the global exit status to 130.
 *
 * \param signum The signal number received.
 */
void		main_signal(int signum);

/**
 * \brief Handles signal setup based on the given identifier.
 *
 * This function sets up signal handlers for different contexts identified by
 * the `id` parameter.
 * It configures the appropriate signal handlers for the main process, child
 * processes, here-documents,
 * pipes, and cases where signals should be ignored.
 *
 * \param id The identifier for the context in which to set up signal handlers.
 *            - MAIN: Sets up signal handlers for the main process.
 *            - CHILD: Sets up signal handlers for child processes.
 *            - HERE_DOC: Sets up signal handlers for here-documents.
 *            - PIPE: Sets up signal handlers for pipes.
 *            - IGNORE: Ignores SIGINT and SIGQUIT signals.
 */
void		ft_handle_signals(int id);

/**
 * \brief Checks and splits the redirection syntax in a token list.
 *
 * This function processes the content of the given token to check for
 * redirection syntax.
 * It splits the content into separate tokens based on redirection
 * characters ('<' and '>').
 * If the head token has an error type, it returns the head token as is.
 *
 * \param head The head of the token list to be checked and split.
 * \return Returns a new token list with the redirection syntax split into
 * separate tokens.
 */
t_token		*check_redir_syntax(t_token *head);

/**
 * \brief Determines the type of a token based on its content.
 *
 * This function analyzes the given word and returns the corresponding token
 * type.
 * It checks for various types of tokens such as pipes, redirections, options,
 * and commands.
 *
 * \param word The input string representing the token to be analyzed.
 * \return Returns the token type as an enumeration value of type \c t_type.
 *         - \c pipes: If the word is a pipe symbol ("|").
 *         - \c redir_out2: If the word is an append redirection symbol (">>").
 *         - \c redir_out: If the word is an output redirection symbol (">").
 *         - \c redir_in2: If the word is a here-document redirection symbol
 *				("<<").
 *         - \c redir_in: If the word is an input redirection symbol ("<").
 *         - \c option: If the word is an option (e.g., "-n") or a valid option
 *				checked by \c ft_check_option.
 *         - \c error: If the word contains invalid redirection syntax.
 *         - \c command: If the word does not match any of the above types.
 */
t_type		ft_token_type(char *word);

/**
 * \brief Creates a new token from a given string.
 *
 * This function allocates memory for a new token, sets its content and type
 * based on the input string,
 * and initializes the next pointer to NULL. If the string contains redirection
 * characters ('<' or '>'),
 * and the token type is command, it checks and splits the redirection syntax
 * if the flag is set.
 *
 * \param str The input string to be converted into a token.
 * \param flag A flag indicating whether to check and split redirection syntax.
 * \return Returns a pointer to the newly created token.
 */
t_token		*ft_new_token(char *str, int flag);

/**
 * \brief Updates or adds an environment variable to the command's environment
 * list.
 *
 * This function checks if the given key exists in the command's environment
 * list.
 * If the key exists, it updates the corresponding environment variable with the
 * new value.
 * If the key does not exist, it adds a new environment variable to the end of
 * the list.
 *
 * \param command A pointer to the command structure containing the environment
 * list.
 * \param key The key of the environment variable to be updated or added.
 * \param value The value of the environment variable to be updated or added.
 */
void		update_or_add_env(t_commands *command, char *key, char *value);

/**
 * \brief Checks if a given string is a valid token.
 *
 * This function verifies whether the input string is a valid token.
 * A valid token must start with an alphabetic character or an underscore,
 * and can contain alphanumeric characters, underscores, and equal signs.
 *
 * \param str The input string to be checked.
 * \return Returns \c true if the string is a valid token, \c false otherwise.
 */
bool		is_invalid_token(char *str);

/**
 * \brief Swaps the contents of two environment variable nodes.
 *
 * This function exchanges the key, value, and visibility status between
 * two given environment variable nodes.
 *
 * \param a A pointer to the first environment variable node.
 * \param b A pointer to the second environment variable node.
 */
void		swap_nodes(t_env *a, t_env *b);

/**
 * \brief Orders the environment variable list alphabetically by key.
 *
 * This function sorts the linked list of environment variables in alphabetical
 * order
 * based on their keys. It uses a simple bubble sort algorithm to swap nodes
 * until
 * the list is ordered.
 *
 * \param env A pointer to the head of the environment variable list.
 * \return Returns a pointer to the head of the sorted environment variable
 * list.
 */
t_env		*order_list(t_env *env);

/**
 * \brief Duplicates a linked list of environment variables.
 *
 * This function creates a deep copy of the given linked list of environment
 * variables.
 * Each node in the new list contains a copy of the key, value, and visibility
 * status
 * from the corresponding node in the original list.
 *
 * \param env A pointer to the head of the environment variable list to be
 * duplicated.
 * \return A pointer to the head of the newly created duplicate list, or \c NULL
 * if
 *         memory allocation fails.
 */
t_env		*dup_list(t_env *env);

/**
 * \brief Handles the export token in a shell command.
 *
 * This function processes an export token by extracting the key and handling
 * any errors that may occur. It validates the key and updates the environment
 * variables accordingly.
 *
 * \param command A pointer to the command structure.
 * \param token A pointer to the token structure containing the export token.
 */
void		handle_export_token(t_commands *command, t_token *token);

/**
 * \brief Prints the environment variables.
 *
 * This function duplicates the environment variable list, sorts it
 * alphabetically by key,
 * and prints each environment variable in a specific format. It skips
 * variables with a
 * visibility status of 3. If the variable has a visibility status of 1,
 * it prints the key
 * and value; otherwise, it prints only the key.
 *
 * \param env A pointer to the head of the environment variable list.
 */
void		print_env(t_env *env);

/**
 * \brief Updates or adds an environment variable.
 *
 * This function updates the value of an existing environment variable or adds
 * a new one
 * if it does not exist. If an equal sign is present, it extracts the value and
 * updates
 * the environment variable. If no equal sign is present, it adds a new
 * environment variable
 * with an empty value.
 *
 * \param command A pointer to the command structure.
 * \param key The key of the environment variable to update or add.
 * \param equal_sign A pointer to the equal sign in the token content, or \c
 * NULL if not present.
 */
void		handle_env_update(t_commands *command, char *key, char *equal_sign);

/**
 * \brief Validates and handles an environment variable key.
 *
 * This function checks if the given key is valid. If the key is invalid,
 * it prints an error
 * message and returns false. If the key is valid, it handles the equal sign if
 * present,
 * or frees the key if not.
 *
 * \param key A double pointer to the key string.
 * \param equal_sign A pointer to the equal sign in the token content, or
 * \c NULL if not present.
 * \param token A pointer to the token structure containing the key.
 * \return \c true if the key is valid, \c false otherwise.
 */
bool		validate_and_handle_key(char **key,
				char *equal_sign, t_token *token);

/**
 * \brief Checks the validity of the unset command.
 *
 * This function iterates through the tokens in the linked list and checks
 * if each token
 * is a valid unset command. A valid unset command must start with a digit and
 * contain
 * only alphanumeric characters or underscores.
 *
 * \param head A pointer to the head of the token linked list.
 * \return \c 0 if all tokens are valid unset commands, \c 1 otherwise.
 */
int			check_unset(t_token *head);

/**
 * \brief Executes a command in a child process.
 *
 * This function forks a new process to execute a command. It sets up the
 * necessary
 * environment variables and command arguments, handles signals, and checks
 * permissions.
 * The parent process waits for the child process to finish and updates the
 * global exit status.
 *
 * \param command A pointer to the command structure containing the command
 * to execute.
 * \param head A pointer to the head of the command list.
 */
void		ft_exec_command(t_commands *command, t_commands *head);

/**
 * \brief Updates the global exit status based on the child process status.
 *
 * This function checks the status of a child process to determine if it exited
 * normally
 * or was terminated by a signal. It then updates the global exit status
 * accordingly.
 *
 * \param status The status code returned by the child process.
 */
void		exec_exit_status(int status);

/**
 * \brief Executes a command using the provided path array.
 *
 * This function attempts to execute a command by iterating through an array of
 * paths.
 * For each path, it constructs the full path to the executable by concatenating
 * the path
 * with the command token content. It then attempts to execute the command using
 * `execve`.
 * If the command is not found in any of the provided paths, the function
 * returns without
 * executing the command.
 *
 * \param path_arr An array of strings representing the paths to search for the
 * executable.
 * \param token_content The command token content to be executed.
 * \param arr_command An array of strings representing the command arguments.
 * \param arr_env An array of strings representing the environment variables.
 */
void		execute_with_path(char **path_arr, char *token_content,
				char **arr_command, char **arr_env);

/**
 * \brief Frees all allocated memory for the given arrays.
 *
 * This function frees the memory allocated for the environment variables array,
 * the path array, and the command arguments array by calling the
 * `free_double_pointer_array` function on each of them.
 *
 * \param arr_env An array of strings representing the environment variables.
 * \param arr An array of strings representing the paths.
 * \param arr_command An array of strings representing the command arguments.
 */
void		free_all_memory(char **arr_env, char **arr, char **arr_command);

/**
 * \brief Handles input redirection for a command.
 *
 * This function opens the file specified in the next token for reading and
 * redirects
 * the standard input (STDIN) to this file. If the file is successfully opened,
 * the file
 * descriptor is duplicated to STDIN and the original file descriptor is closed.
 *
 * \param token A pointer to the current token, which contains the redirection
 * information.
 * \return The file descriptor of the opened file, or -1 if the file could not
 * be opened.
 */
int			handle_redir_in(t_token *token);

/**
 * \brief Handles output redirection with append mode for a command.
 *
 * This function opens the file specified in the next token for writing in
 * append mode
 * and redirects the standard output (STDOUT) to this file. If the file is
 * successfully
 * opened, the file descriptor is duplicated to STDOUT.
 *
 * \param token A pointer to the current token, which contains the redirection
 * information.
 * \return The file descriptor of the opened file, or -1 if the file could not
 * be opened.
 */
int			handle_redir_out2(t_token *token);

/**
 * \brief Handles output redirection for a command.
 *
 * This function opens the file specified in the next token for writing in
 * truncate mode
 * and redirects the standard output (STDOUT) to this file. If the file is
 * successfully
 * opened, the file descriptor is duplicated to STDOUT.
 *
 * \param token A pointer to the current token, which contains the redirection
 * information.
 * \return The file descriptor of the opened file, or -1 if the file could not
 * be opened.
 */
int			handle_redir_out(t_token *token);

/**
 * \brief Extracts and stores a value from a string.
 *
 * This function scans the input string starting from the second character,
 * and extracts a substring that represents a key. The key is defined as a
 * sequence of alphanumeric characters that does not include the '$' character
 * or spaces. The extracted key is then stored in a newly allocated string.
 *
 * \param string The input string from which the key is to be extracted.
 * \return A newly allocated string containing the extracted key.
 */
char		*store_value(char *string);

/**
 * \brief Expands a string by replacing a key with a value.
 *
 * This function calculates the new length of the string after replacing the
 * key with the value,
 * allocates memory for the new string, and performs the replacement.
 *
 * \param value The value to replace the key with.
 * \param key The key to be replaced in the string.
 * \param string The original string containing the key.
 * \return A newly allocated string with the key replaced by the value, or NULL
 * if memory allocation fails.
 */
char		*expand_new_string(char *value, char *key, char *string);

/**
 * \brief Calculates the extra length needed for a string after replacing
 * certain patterns.
 *
 * This function scans the input string for occurrences of the pattern "$?"
 * and calculates
 * the additional length required to replace each occurrence with a string of
 * length `num_len`.
 *
 * \param string The input string to be scanned.
 * \param num_len The length of the string that will replace each occurrence of
 * the pattern "$?".
 * \return The total extra length needed for the new string.
 */
int			calculate_extra_length(char *string, int num_len);

/**
 * \brief Processes a token to handle variable expansion.
 *
 * This function checks if the token's content contains a dollar sign ('$')
 * that is not followed by a null character and is outside single quotes.
 * If such a dollar sign is found, the function processes the token's content
 * to expand any variables and then checks if further expansion is needed.
 *
 * \param token A pointer to the token to be processed.
 * \param commands A pointer to the commands structure containing the token.
 */
void		process_token(t_token *token, t_commands *commands);

/**
 * \brief Processes the content of a token for variable expansion.
 *
 * This function checks if the content of the token contains a dollar sign
 * ('$').
 * If the dollar sign is followed by a question mark ('?'), it expands the exit
 * code.
 * If the dollar sign is followed by a digit, it expands the number.
 * Otherwise, it checks if further expansion is needed.
 *
 * \param content The content of the token to be processed.
 * \param commands A pointer to the commands structure containing the token.
 * \return A newly allocated string with the expanded content.
 */
char		*process_token_content(char *content, t_commands *commands);

/**
 * \brief Replaces a key in a string with a value.
 *
 * This function scans the input string for a key prefixed by a dollar sign
 * ('$').
 * When the key is found, it is replaced with the provided value in the new
 * string.
 * The function then copies the remaining part of the original string to the
 * new string.
 *
 * \param new_string A pointer to the newly allocated string where the key will
 * be replaced by the value.
 * \param value The value to replace the key with.
 * \param key The key to be replaced in the string.
 * \param string The original string containing the key.
 */
void		replace_value_with_key(char **new_string, char *value,
				char *key, char *string);

/**
 * \brief Checks if the given string has balanced quotes.
 *
 * This function iterates through the input string to check for balanced single
 * and double quotes.
 * If an unbalanced quote is found, it prints an error message and returns true.
 *
 * \param str The input string to be checked for balanced quotes.
 * \return true if the string has unbalanced quotes, false otherwise.
 */
bool		check_quotes(char *str);

/**
 * \brief Replaces a variable in a string with a given value.
 *
 * This function scans the input string for a variable key prefixed by a dollar
 * sign ('$').
 * When the key is found at the specified index, it is replaced with the
 * provided value in the new string.
 * The function then copies the remaining part of the original string to the new
 * string.
 *
 * \param string The original string containing the variable key.
 * \param key The variable key to be replaced in the string.
 * \param value The value to replace the key with.
 * \param i The index at which the key is expected to be found.
 * \return A newly allocated string with the variable key replaced by the value.
 */
char		*replace_variable(char *string, char *key, char *value, int i);

/**
 * \brief Copies a key into a new string at the specified position.
 *
 * This function iterates through the key and copies each character into the new
 * string
 * starting at the position indicated by the pointer \p j. The position pointer
 * \p j is
 * incremented accordingly.
 *
 * \param new_string A pointer to the new string where the key will be copied.
 * \param j A pointer to the current position in the new string.
 * \param key The key to be copied into the new string.
 */
void		copy_key_to_new_string(char *new_string, int *j, char *key);

/**
 * \brief Checks if a dollar sign ('$') is outside single quotes in a string.
 *
 * This function iterates through the input string to determine if there is a
 * dollar sign ('$')
 * that is not enclosed within single quotes. It also handles nested double
 * quotes.
 *
 * \param str The input string to be checked.
 * \return true if a dollar sign is found outside single quotes, false
 * otherwise.
 */
bool		is_dollar_outside_single_quotes(char *str);

/**
 * \brief Searches a string for any of a set of delimiters, considering quotes.
 *
 * This function scans the input string \p s for any character from the
 * delimiter string \p delims.
 * It skips over characters enclosed in single or double quotes.
 *
 * \param s The input string to be searched.
 * \param delims The string containing delimiter characters.
 * \return A pointer to the first occurrence of any delimiter character in \p s
 * that is not within quotes, or NULL if no such character is found.
 */
char		*ft_strpbrk(char *s, char *delims);

/**
 * \brief Calculates the size of a token list.
 *
 * This function iterates through the linked list of tokens and counts the
 * number of tokens.
 *
 * \param token A pointer to the head of the token list.
 * \return The number of tokens in the list.
 */
int			ft_token_size(t_token *token);

/**
 * \brief Removes empty tokens from a list of commands.
 *
 * This function iterates through the list of commands and processes each
 * command's tokens
 * to remove any tokens that are empty strings.
 *
 * \param commands A pointer to the head of the list of commands.
 */
void		ft_empty_nodes(t_commands *commands);

/**
 * \brief Expands a number in a string by replacing a specific pattern.
 *
 * This function searches for a dollar sign ('$') in the input string
 * \p content.
 * If found, it checks if the dollar sign is within quotes and removes the
 * quotes if necessary.
 * It then replaces the pattern with either "minishell" or the remaining
 * part of the string.
 *
 * \param content The input string containing the pattern to be expanded.
 * \return A new string with the expanded pattern, or the original string
 * if no pattern is found.
 */
char		*expand_number(char *content);

/**
 * \brief Copies a value to a new string.
 *
 * This function copies the characters from the input value string to the new
 * string,
 * starting at the position indicated by the pointer \p j.
 *
 * \param new_string The destination string where the value will be copied.
 * \param value The source string containing the value to be copied.
 * \param j A pointer to the current position in the new string where the value
 * should be copied.
 */
void		copy_value_to_new_string(char *new_string, char *value, int *j);

/**
 * \brief Copies the remaining part of a string to a new string.
 *
 * This function copies characters from the source string \p string starting at
 * the position
 * indicated by the pointer \p h to the destination string \p new_string
 * starting at the position
 * indicated by the pointer \p j.
 *
 * \param new_string The destination string where the remaining part of the
 * source string will be copied.
 * \param string The source string from which characters will be copied.
 * \param h A pointer to the current position in the source string.
 * \param j A pointer to the current position in the destination string.
 */
void		copy_remaining_string(char *new_string, char *string,
				int *h, int *j);

/**
 * \brief Allocates memory for a new string.
 *
 * This function allocates memory for a new string of the specified \p length.
 * If the memory allocation fails, it returns NULL.
 *
 * \param length The length of the new string to be allocated.
 * \return A pointer to the newly allocated string, or NULL if the allocation
 * fails.
 */
char		*ft_allocate_new_string(size_t length);

/**
 * \brief Processes the expansion of a variable in the input string.
 *
 * This function expands a variable in the input string \p input at the
 * position \p i
 * using the provided command context \p command. It calculates the new
 * length of the
 * input string after expansion and updates the position \p i accordingly.
 *
 * \param input The input string containing the variable to be expanded.
 * \param i A pointer to the current position in the input string.
 * \param command The command context used for variable expansion.
 * \return A new string with the expanded variable.
 */
char		*process_expansion(char *input, size_t *i, t_commands *command);

/**
 * \brief Determines if a character should trigger variable expansion.
 *
 * This function checks if the character \p c is a dollar sign ('$') and if
 * it is not within single quotes.
 *
 * \param c The character to be checked.
 * \param single_quotes A boolean indicating if the character is within single
 * quotes.
 * \return True if the character is a dollar sign and not within single quotes,
 * false otherwise.
 */
bool		should_expand(char c, bool single_quotes, char *input);

/**
 * \brief Handles the state of single and double quotes.
 *
 * This function updates the state of single and double quotes based on the
 * input character \p c.
 * If the character is a single quote and double quotes are not active, it
 * toggles the state of single quotes.
 * If the character is a double quote and single quotes are not active, it
 * toggles the state of double quotes.
 *
 * \param c The input character to check.
 * \param single_quotes A pointer to the boolean indicating the state of
 *single quotes.
 * \param double_quotes A pointer to the boolean indicating the state of
 * double quotes.
 */
void		handle_quotes(char c, bool *single_quotes, bool *double_quotes);

/**
 * \brief Counts the number of words in a string separated by a given character.
 *
 * This function counts the number of words in the input string \p s,
 * where words are separated
 * by the character \p c. It also takes into account quoted substrings,
 * which are not split by the separator.
 *
 * \param s The input string to be processed.
 * \param c The character used as a word separator.
 * \return The number of words in the input string.
 */
size_t		word_count(char const *s, char c);

/**
 * \brief Frees a dynamically allocated array of strings.
 *
 * This function frees each string in the array \p arr up to the specified
 * index \p j,
 * and then frees the array itself.
 *
 * \param arr The array of strings to be freed.
 * \param j The number of strings in the array to be freed.
 */
void		free_split(char **arr, size_t j);

/**
 * \brief Counts the number of letters in a word within a string.
 *
 * This function counts the number of letters in a word in the input
 * string \p s,
 * starting from the position \p i. The word is considered to be delimited
 * by the
 * character \p c, taking into account quoted substrings which are not split
 * by the delimiter.
 *
 * \param s The input string to be processed.
 * \param c The character used as a word delimiter.
 * \param i The starting position in the input string.
 * \return The number of letters in the word.
 */
size_t		letters_in_word(char const *s, char c, size_t i);

/**
 * \brief Creates a new command structure from a given string and environment.
 *
 * This function initializes a new \p t_commands structure by parsing the input
 * string \p str
 * and associating it with the given environment \p env. It splits the string
 * into words,
 * creates tokens for each word, and updates the command tokens accordingly.
 *
 * \param str The input string to be parsed into commands.
 * \param env The environment structure to be associated with the command.
 * \return A pointer to the newly created \p t_commands structure, or NULL if
 * an error occurs.
 */
t_commands	*ft_new_commands(char *str, t_env *env);

/**
 * \brief Frees dynamically allocated memory for a command structure, string,
 * and array of words.
 *
 * This function frees the memory allocated for the \p command structure, the
 * \p str string,
 * and the \p words array of strings if they are not NULL.
 *
 * \param command The command structure to be freed.
 * \param str The string to be freed.
 * \param words The array of strings to be freed.
 */
void		cleanup(t_commands *command, char *str, char **words);

/**
 * \brief Sets multiple pointers to NULL.
 *
 * This function sets the pointers \p commands, \p head, \p current, and \p
 * words to NULL.
 * It is useful for initializing these pointers to a known state.
 *
 * \param commands A pointer to a \p t_commands pointer to be set to NULL.
 * \param head A pointer to a \p t_token pointer to be set to NULL.
 * \param current A pointer to a \p t_token pointer to be set to NULL.
 * \param words A pointer to a pointer to a char array to be set to NULL.
 */
void		set_null(t_commands **commands, t_token **head,
				t_token **current, char ***words);

/**
 * \brief Checks if a string contains any non-digit characters.
 *
 * This function iterates through the input string \p str and checks if there
 * are any
 * characters that are not digits. If a non-digit character is found, the
 * function
 * returns \c false. If all characters are digits, the function returns \c true.
 *
 * \param str The input string to be checked.
 * \return \c true if the string contains only digits, \c false otherwise.
 */
bool		ft_has_letters(char *str);

/**
 * \brief Checks if a string represents a valid option.
 *
 * This function checks if the input string \p str represents a valid option.
 * A valid option starts with a '-' character followed by one or more 'n'
 * characters.
 * The function removes any quotes from the string before performing the check.
 *
 * \param str The input string to be checked.
 * \return \c true if the string represents a valid option, \c false otherwise.
 */
int			ft_check_option(char *str);

/**
 * \brief Prints a warning message for a here-document delimited by end-of-file.
 *
 * This function prints a warning message to the standard error output
 * indicating
 * that a here-document was delimited by end-of-file instead of the expected
 * delimiter.
 *
 * \param del The expected delimiter for the here-document.
 */
void		eof_heredoc(char *del);

/**
 * \brief Appends a newline to a string and writes it to a file descriptor.
 *
 * This function appends a newline character to the input string \p str,
 * writes the resulting string to the file descriptor \p fd, and then frees
 * the memory allocated for the input string and the new string.
 *
 * \param str The input string to be appended with a newline and written.
 * \param fd The file descriptor to which the string is written.
 */
void		end_heredoc(char *str, int fd);

/**
 * \brief Checks if a character is between quotes in a string.
 *
 * This function iterates through the input string \p str and checks if the
 * specified character \p target is between single or double quotes. It
 * returns \c true if the character is found between quotes, and \c false
 * otherwise.
 *
 * \param str The input string to be checked.
 * \param target The character to be checked if it is between quotes.
 * \return \c true if the character is between quotes, \c false otherwise.
 */
bool		is_char_between_quotes(const char *str, char target);

/**
 * \brief Checks for invalid redirection characters in a string.
 *
 * This function scans the input string \p str for invalid sequences of
 * redirection
 * characters ('<' or '>'). It ensures that these characters are not followed by
 * another redirection character without any non-space characters in between.
 * If an invalid sequence is found, an error message is printed and the function
 * returns \c 0. Otherwise, it returns \c 1.
 *
 * \param str The input string to be checked.
 * \return \c 1 if the string is valid, \c 0 if an invalid sequence is found.
 */
int			ft_check(char *str);

/**
 * \brief Expands variables in a here-document.
 *
 * This function scans the input string \p input for variables indicated by the
 * '$' character
 * and expands them using the provided \p commands structure. It ensures that
 * variables are not
 * expanded if they are enclosed in single or double quotes or followed by a
 * space.
 *
 * \param input The input string containing the here-document.
 * \param commands The structure containing the environment variables for
 * expansion.
 * \return The input string with variables expanded.
 */
char		*expand_heredoc(char *input, t_commands *commands);
#endif
