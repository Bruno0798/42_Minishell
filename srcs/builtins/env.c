/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:30:52 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/23 12:37:37 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_env(t_commands *command)
{
	t_env *current;

	current = command->env;
	while(current)
	{
		ft_printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}