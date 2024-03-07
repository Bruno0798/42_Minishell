/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:30:52 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/23 18:19:02 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_print_env(t_commands *command)
{
	t_env *current;

	current = command->env;
	while(current)
	{
		printf("%s=%s\n", current->key, current->value); /* Print the environment variable */
		current = current->next;
	}
}
