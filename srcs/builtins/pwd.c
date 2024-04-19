/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:14:28 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/04/19 15:44:38 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_pwd(t_commands *command)
{
	char pwd[4096]; /* Create a buffer to store the current working directory */
	if (command->token->next != NULL)
	{
		if (command->token->next->content[0] ==
			'-')
		{
			printf("bash: pwd %s: invalid option\n",
				   command->token->next->content);
			return;
		}
	}
	getcwd(pwd, sizeof(pwd)); /* Get the current working directory */
	printf("%s\n", pwd); /* Print the current working directory */
}
