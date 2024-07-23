/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:57:05 by brpereir          #+#    #+#             */
/*   Updated: 2024/07/23 14:22:27 by bsousa-d         ###   ########.fr       */
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
# define ERROR_OPTIONS " illegal option"
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

void		init_env(t_env **env, char **envp);
t_env		*ft_new_env(char *key, char *value, int visible);
void		ft_add_env_back(t_env **env_lst, t_env *new_env);
char		*ft_get_value(t_env *env, char *search);
void		ft_update_env(t_env *env_lst, char *key,
				char *replace, int visible);
void		free_env(t_env *env);
void		check_args(int argc, int valid_argc);
void		ft_print_token_list(t_token *head);
void		ft_print_env(t_commands *command);
void		print_error(char *msg, char *key, int exit_code);
int			count_pipes(char *str);
bool		is_between_quotes(char *str);
bool		is_everything_space(char *str);
void		ft_unset(t_commands *command);
void		ft_pwd(t_commands *command);
void		ft_echo(t_token *token);
void		ft_export(t_commands *command);
void		ft_handler_ctrl(int signum);
void		free_all(t_commands *command, int option);
void		ft_exit(t_commands *command, t_commands *exit);
void		ft_free_token(t_token *token);
void		ft_free_commands(t_commands *command, int option);
void		ft_free_env(t_env *env);
t_commands	*pipe_commands(char *str, t_env *env);
void		ft_expander(t_commands *command);
char		**get_path(t_env *env_lst);
char		**ft_split2(char const *s, char c);
char		**ft_lst_to_arr(t_token *token);
char		**ft_env_to_arr(t_env *env);
int			ft_execution(t_commands *command, t_commands *head);
int			env_size(t_env *env);
char		*ft_expand_join(char *s1, t_env *env);
char		*trim_right(const char *input_string);
char		*replace_substr(char *original, const char *substr, char
				*replacement, int i);
void		ft_special_handler(const char *input);
char		*ft_delete_quotes(char *input);
void		ft_remove_quotes(t_commands *commands);
int			ft_parser(char *input, t_commands **commands, t_env *env);
void		ft_execute(t_commands *command, t_commands *head);
bool		is_valid_input(char *input, t_env *env);
bool		syntax_checker(char *input);
void		ft_cleans_special_chars(t_commands *commands);
void		ft_cd(t_commands *command);
bool		can_execute(t_commands *command);
t_env		*ft_fnd_env(t_env *env, char *search);
bool		ft_has_special_char(char *str);
void		ft_create_file(char *filename);
int			ft_redirect(t_commands *command);
void		ft_handle_redirect(t_commands *command);
int			ft_check_redirect(t_commands *command);
int			ft_count_redirects(t_commands *commands);
void		check_fork(t_commands *command);
void		open_pipes(t_commands *command);
bool		has_double_pipe(char *input);
int			check_heredoc(t_commands *command);
void		ft_heredoc(char *delimiter, t_commands *command);
t_commands	*delete_redir(t_commands *command);
void		heredoc_cycle(char *line, t_commands *commands);
bool		has_here_doc(t_commands *commands);
void		remake_commands(t_commands *command);
bool		files_exist(t_token *token);
char		*extract_home_path(const char *pwd);
void		heredoc_files(t_commands *command, int flag);
void		ft_expand_others(t_commands *commands);
void		parent_process(int fd_in, int count_pipes, int fork_pid);
int			child_process(t_commands *command, int fd_in, int command_count);
int			count_commands(t_commands *command);
void		free_double_pointer_array(char **arr);
bool		is_between_single_quotes(char *str);
void		check_permissions(t_commands *command, char *string);
void		ft_token_addback(t_token **token, t_token *new);
void		free_tokens(t_token *token);
void		ft_expander_heredoc(t_commands *commands);
char		*needs_expansion(char *input, t_commands *command);
char		*expand_variable(char *string, int i, t_commands *commands);
char		*expand_exit_code(char *string);
bool		is_dollar_outside_single_quotes(char *str);
void		main_signal(int signum);
void		ft_handle_signals(int id);
t_token		*check_redir_syntax(t_token *token);
bool		redir_valid(t_token *token);
t_type		ft_token_type(char *word);
t_token		*ft_new_token(char *str, int flag);
void		update_or_add_env(t_commands *command, char *key, char *value);
bool		is_invalid_token(char *str);
int			compare_keys(const char *a, const char *b);
void		swap_nodes(t_env *a, t_env *b);
t_env		*order_list(t_env *env);
t_env		*dup_list(t_env *env);
void		handle_export_token(t_commands *command, t_token *token);
void		print_env(t_env *env);
void		handle_env_update(t_commands *command, char *key, char *equal_sign);
bool		validate_and_handle_key(char **key,
				char *equal_sign, t_token *token);
int			check_unset(t_token *head);
void		cleanup_exec(char **arr_command, char **arr_env, char **arr);
void		handle_child_process(t_commands *command, t_commands *head);
int			is_relative_path(const char *path);
int			is_absolute_path(const char *path);
void		ft_exec_command(t_commands *command, t_commands *head);
void		exec_exit_status(int status);
void		execute_with_path(char **path_arr, char *token_content,
				char **arr_command, char **arr_env);
void		free_all_memory(char **arr_env, char **arr, char **arr_command);
int			handle_redir_in(t_token *token);
int			handle_redir_out2(t_token *token);
int			handle_redir_out(t_token *token);
void		handle_parent_pipes(int pipes[], int *fd_in);
char		*store_value(char *string);
char		*expand_new_string(char *value, char *key, char *string);
int			calculate_extra_length(char *string, int num_len);
void		process_token(t_token *token, t_commands *commands);
char		*process_token_content(char *content, t_commands *commands);
size_t		get_new_string_length(char *value, char *key, char *string);
void		replace_value_with_key(char **new_string, char *value,
				char *key, char *string);
bool		check_quotes(char *str);
char		*replace_variable(char *string, char *key, char *value, int i);
void		copy_key_to_new_string(char *new_string, int *j, char *key);
char		*perform_variable_replacement(char *string, char *key,
				char *value, int i);
bool		is_dollar_outside_single_quotes(char *str);
char		*ft_strpbrk(char *s, char *delims);
int			ft_token_size(t_token *token);
void		ft_empty_nodes(t_commands *commands);
char		*expand_number(char *content);
void		copy_value_to_new_string(char *new_string, char *value, int *j);
void		copy_remaining_string(char *new_string, char *string,
				int *h, int *j);
char		*allocate_new_string(char *string, char *key, char *value);
char		*ft_allocate_new_string(size_t length);
char		*process_expansion(char *input, size_t *i, t_commands *command);
bool		should_expand(char c, bool single_quotes);
void		handle_quotes(char c, bool *single_quotes, bool *double_quotes);
size_t		word_count(char const *s, char c);
void		free_split(char **arr, size_t j);
size_t		letters_in_word(char const *s, char c, size_t i);
t_commands	*ft_new_commands(char *str, t_env *env);
void		ft_free_command(t_commands *command);
void		cleanup(t_commands *command, char *str, char **words);
void		set_null(t_commands **commands, t_token **head,
				t_token **current, char ***words);
bool		ft_has_letters(char *str);
#endif
