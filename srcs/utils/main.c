/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:25:43 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/03/26 18:26:00 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int init_and_set_fd(int argc, char **envp, t_env **env)
{
	int fd;

	fd = dup(STDIN_FILENO);
	check_args(argc, 1);
	init_env(env, envp);

	return fd;
}

int main(int argc, char **argv, char **envp)
{
	(void)		argv;
	t_commands	*command; // NEEDS TO BE REMOVED FROM HERE AND PASS TO PARER
	t_env		*env;
	char		*input;
	int fd;

	fd = init_and_set_fd(argc, envp, &env);
	while (42)
	{
		dup2(fd, STDIN_FILENO);
		input = readline("Minishell$>");
		add_history(input);
		if (is_valid_input(input))
			if (ft_parser(input, &command, env) == EXIT_SUCCESS)
			{
				remake_commands(command);
				open_pipes(command);
			}
		handle_signals();
		if (!input)
			free_env(env);
		ft_special_handler(input); // NEEDS TO BE REMOVED FROM HERE
	}
	return 0;
}

void heredoc_files(t_commands *command, int flag)
{
	while (command)
	{
		if (flag == 1)
			check_heredoc(command);
		else
			files_exist(command->token);
		command = command->next;
	}
}

bool files_exist(t_token *token)
{
	bool exist;
	int fd;
	t_token *curr;

	exist = true;
	curr = token;
	while(curr)
	{
		if (curr->type == redir_in && curr->next)
		{
			fd = open(curr->next->content, O_RDONLY, 0644);
			if (fd < 0)
			{
				exist = false;
				printf("minishell: %s: No such file or directory\n", curr->next->content);
			}
		}
		curr = curr->next;
	}
	return exist;
}

void remake_commands(t_commands *command)
{
	t_commands *temp;

	temp = command;
	heredoc_files(temp, 1);
	heredoc_files(temp, 2);
}

void check_heredoc(t_commands *command)
{
	t_token *token;

	token = command->token;

	while (token)
	{
		if (token->type == redir_in2 && token->next)
		{
			printf("delimiter: %s\n", token->next->content);
			ft_heredoc(token->next->content);
			dup2(open("heredoc.txt", O_RDONLY), STDIN_FILENO);
		}
		token = token->next;
	}
	delete_redir(command);
}

t_commands *delete_redir(t_commands *command)
{
	t_token *curr;
	t_token *prev;

	curr = command->token;
	prev = NULL;
	while (curr)
	{
		if (curr->type == redir_in2)
		{
			if (prev)
				prev->next = curr->next->next;
			else
				command->token = curr->next->next;
			free(curr->content);
			free(curr->next->content);
			free(curr->next);
			free(curr);
			return command;
		}
		prev = curr;
		curr = curr->next;
	}
	return command;
}

void ft_heredoc(char *delimiter)
{
	int temp;
	pid_t pid;
	char *line;

	handle_signals();
	pid = fork();
	if (pid == 0)
	{
	    line = ft_strjoin(delimiter, "\n");
		heredoc_cycle(line);
		free(line);
		exit(EXIT_STATUS);
	}
	waitpid(pid, &temp, 0);
	EXIT_STATUS = temp >> 8;
}

void heredoc_cycle(char *line)
{
	int fd;
	int output;
	char buff[4095];

	ft_signals_heredoc();
	fd = open("heredoc.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	write(1, "> ", 2);
	output = read(1, buff, 4095);
	while(output > 0)
	{
		ft_signals_heredoc();
		buff[output] = '\0';
		if (!ft_strcmp(buff, line))
			break;
		write(fd, buff, output);
		write(1, "> ", 2);
		output = read(1, buff, 4095);
	}
	close(fd);
}


void ft_special_handler(const char *input) {
	if (input[0] == '$' && input[1] == '?' &&
		input[2] == '+' && input[3] == '$' && input[4] == '?' &&
		input[5] == '\0') {
		printf("%i+%if: command not found\n", EXIT_STATUS, EXIT_STATUS);
		EXIT_STATUS = 127;  // Set exit status for subsequent commands
	}
}

char *ft_delete_quotes(char *input) {
	int i;
	int j;
	char *new_str;
	char quote;

	new_str = malloc(strlen(input) + 1);
	i = -1;
	j = 0;
	while (input[++i])
	{
		if (input[i] != '"' && input[i] != '\'')
			new_str[j++] = input[i];
		else if (input[i] == '"' || input[i] == '\'')
		{
			quote = input[i++];
			while(input[i] != quote)
				new_str[j++] = input[i++];
		}
	}
	new_str[j] = '\0';
	return new_str;
}


void ft_remove_quotes(t_commands *commands)
{
	char *old_cmd = NULL;
	t_token *curr = commands->token;

	while (curr)
	{
		if (curr->type == command && !is_between_quotes(curr->content))
		{
			old_cmd = curr->content;
			curr->content = ft_delete_quotes(curr->content); /* Account for quotes */
			free(old_cmd); /* Free the old string */
		}
		curr = curr->next;
	}
	free(curr);
}

void ft_expand_others(t_commands *commands)
{
	int i;
	t_token *curr = commands->token;
	
	while(curr)
	{
		i = -1;
		while (curr->content[++i])
		{
			if ((curr->content[i] == '~') && (i == 0) && ((curr->content[i + 1] == ' ') || (curr ->content[i + 1] ==
			'\0') || (curr->content[i + 1] == '/')))
				curr->content = ft_strjoin(ft_get_value(commands->env, "HOME"), curr->content + 1);
			else if ((curr->content[i] == '~') && (i == 0) && (curr->content[i + 1] == '+'))
				curr->content = ft_strjoin(ft_get_value(commands->env, "PWD"), curr->content + 2);
			else if ((curr->content[i] == '~') && (i == 0) && (curr->content[i + 1] == '-'))
				curr->content = ft_strjoin(ft_get_value(commands->env, "OLDPWD"), curr->content + 2);
		}
		curr = curr->next;
	}
	free (curr);
}


int ft_parser(char *input, t_commands **commands, t_env *env)
{
	if (!is_between_quotes(input))
		return (printf("minishell: syntax error: unexpected end of file\n") && EXIT_FAILURE);
	*commands = pipe_commands(input, env);
	if (!has_here_doc(*commands))
		ft_expander(*commands);
	ft_expand_others(*commands);
	ft_remove_quotes(*commands);
	return EXIT_SUCCESS;
}

bool has_here_doc(t_commands *commands)
{
	t_token *curr = commands->token;

	while (curr)
	{
		if (curr->type == redir_in2)
			return true;
		curr = curr->next;
	}
	return false;
}

bool is_forkable(t_commands *command)
{
	if (!ft_strcmp(command->token->content, "cd") || !ft_strcmp
	(command->token->content, "export") || !ft_strcmp
	(command->token->content, "unsett") || !ft_strcmp
	(command->token->content, "exit"))
	{
		// HEREDOC ATTENTION
		printf("NO FORK\n");
		return false;
	}
	return true;
}

void ft_do_one_cmd(t_commands *command)
{
	pid_t pid;
	int status;

	pid = fork();

	// if para HEREDOC
	if (pid < 0)
		print_error(ERROR_FORK, NULL, 1);
	else if (pid == 0)
	{
		// SIGNAL HANDLER
		// HAS HEREDOC?
		// IF CMD HAS REDICTION
		ft_execute(command);
		free_all(command);
		// FREE COMMANDS INSIDE THE FORK
		exit(0);
	}
	else
		waitpid(pid, &status, 0);

}

int count_commands(t_commands *command)
{
	int count;
	t_commands *token;

	count = 0;
	token = command;
	while (token)
	{
		count++;
		token = token->next;
	}
	return count;
}

void child_process(t_commands *command, int fd_in, int command_count)
{
	int i;
	int pipes[2];
	pid_t  pid;

	i = 0;
	while (i < command_count)
	{
		pipe(pipes);
		pid = fork();
		if (pid == 0)
		{
			if (i != 0)
				dup2(fd_in, STDIN_FILENO);
			if ((i + 1) != command_count)
				dup2(pipes[1], STDOUT_FILENO);
			close(pipes[0]);
			close(pipes[1]);
			close(fd_in);
			ft_execution(command);
			exit(EXIT_STATUS);
		}
		dup2(pipes[0], fd_in);
		close(pipes[0]);
		close(pipes[1]);
		command = command->next;
		i++;
	}
}

void parent_process(int fd_in, int count_pipes)
{
	int i;

	i = 0;
	close(fd_in);
	while (i < count_pipes)
	{
		wait(NULL);
		i++;
	}
}

void open_pipes(t_commands *command)
{
	int fd_in;
	int pipes_count;

	pipes_count = count_commands(command);

	if (pipes_count > 1)
	{
		fd_in = dup(STDIN_FILENO);
		child_process(command, fd_in, pipes_count);
		parent_process(fd_in, pipes_count);
	}
	else
		ft_execute(command);
}


int ft_mult_cmds(t_commands *command)
{
	int status;
	int pid;
	t_token *token;

	status = 0;
	// SIGNAL HANDLER
	pid = fork();
	if (pid < 0)
		print_error(ERROR_FORK, NULL, 1);
	else if (pid == 0)
	{
		//SIGNAL AGAIN??
		// HAS HEREDOC?
		
	}
	return 0;
}

void ft_exit(t_commands *command)
{
	free_all(command);
	exit(EXIT_STATUS);
}


void ft_execute(t_commands *command)
{
	if(!(ft_strcmp(command->token->content, "pwd")))
		ft_pwd();
	else if(!(ft_strcmp(command->token->content, "echo")))
		ft_echo(command->token->next);
	else if(!(ft_strcmp(command->token->content, "cd")))
		ft_cd(command);
	else if(!(ft_strcmp(command->token->content, "env")))
		ft_print_env(command);
	else if(!(ft_strcmp(command->token->content, "unset")))
		ft_unset(command);
	else if(!(ft_strcmp(command->token->content, "export")))
		ft_export(command);
	else if(!(ft_strcmp(command->token->content, "exit")))
		ft_exit(command);
	else
		ft_execution(command);
}

void check_args(int argc, int valid_argc)
{
	if (argc != valid_argc)
	{
		printf(RED"Wrong Arguments\nUse './minishell' to start!\n"ENDC);
		exit(0);
	}
}

bool is_valid_input(char *input)
{
	if (is_everything_space(input) || !syntax_checker(input))
		return false;
	return true;
}


bool syntax_checker(char *input)
{
	input = ft_strtrim(input, " \t");
	if (!input || ft_strchr("|<>", *input) || ft_strchr("|<>", input[ft_strlen(input)- 1]))
	{
		if (input)
		{
			if (*input == '|')
			{
				if (input[1] == '|')
					print_error(ERROR_PIPE_2, NULL, 1);
				else
					print_error(ERROR_PIPE, NULL, 1);
			} else if (ft_strchr("|<>", input[ft_strlen(input) - 1]))
			{
				if (input[ft_strlen(input) - 1] == '|')
					print_error(ERROR_PROMPT, NULL, 2);
				else
					print_error(ERROR_REDIR, NULL, 2);
			}
		}
		free(input);
		return false;
	}
	free(input);
	return true;
}


void	print_error(char *msg, char *key, int exit_code)
{
	if (key)
	{
		if (msg)
			printf("%s: %s: %s\n", ERROR_TITLE, key, msg);
		else
			printf("%s: %s `%s'\n", ERROR_TITLE, ERROR_SYNTAX, key);
	}
	else
		printf("%s: %s\n", ERROR_TITLE, msg);
	EXIT_STATUS = exit_code;
}
