/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:30:08 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/30 13:32:02 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ft_check_path(t_env *env);

void	ft_print_env(t_commands *command)
{
	t_env	*current;

	current = command->env;
	if (command->env == NULL || ft_check_path(current))
		return ;
	if (command->token->next != NULL)
	{
		if (ft_strchr(command->token->next->content, '-') != NULL)
		{
			dup2(STDERR_FILENO, STDOUT_FILENO);
			print_error(ERROR_OPTIONS, command->token->next->content, 125);
			return ;
		}
		dup2(STDERR_FILENO, STDOUT_FILENO);
		print_error(ERROR_DIR, command->token->next->content, 127);
		return ;
	}
	while (current != NULL)
	{
		if (current->key != NULL && current->visible == 1)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	g_exit_status = 0;
}

bool	ft_check_path(t_env *env)
{
	if (ft_get_value(env, "PATH") == NULL)
	{
		print_error(ERROR_DIR, NULL, 127);
		return (true);
	}
	return (false);
}
