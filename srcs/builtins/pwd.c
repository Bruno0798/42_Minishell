/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:14:28 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/30 13:31:25 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_pwd(t_commands *command)
{
	char	pwd[4096];

	if (command->token->next != NULL)
	{
		if (command->token->next->content[0] == '-')
		{
			print_error(ERROR_OPTIONS, command->token->next->content, 2);
			return ;
		}
	}
	getcwd(pwd, sizeof(pwd));
	printf("%s\n", pwd);
	g_exit_status = 0;
}
