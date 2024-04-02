/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:14:28 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/04/02 15:14:28 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 02:12:30 by brunolopes        #+#    #+#             */
/*   Updated: 2024/01/23 18:17:58 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_pwd(t_commands *command)
{
	char pwd[4096]; /* Create a buffer to store the current working directory */
	
	if (command->token->next != NULL)
	{
		printf("bash: pwd %s: invalid option\n",command->token->next->content);
		return ;
	}
	getcwd(pwd, sizeof(pwd)); /* Get the current working directory */
	printf("%s\n", pwd); /* Print the current working directory */
}
