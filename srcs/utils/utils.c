/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 18:08:16 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/23 17:27:23 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool is_between_quotes(char *str)
{
	int s_quote;
	int d_quote;
	int i;

	i =-1;
	s_quote = 0;
	d_quote = 0;
	while (str[++i])
	{
		if (str[i] == DOUBLE_QUOTE && !s_quote)
			d_quote = !d_quote;
		else if (str[i] == SINGLE_QUOTE && !d_quote)
			s_quote = !s_quote;
	}
	if (s_quote != 0 || d_quote != 0)
		return true;
	return false;
}

bool is_everything_space(char *str)
{
	int i;

	i = -1;
	while (str[++i])
		if (str[i] != ' ')
			return false;
	return true;
}

bool	is_space(char c)
{
	if (c == 32)
		return true;
	return false;
}

char **ft_lst_to_arr(t_token *token)
{
	int	i;
	char	**arr;
	t_token	*tmp;

	i = 1;
	tmp = token;
	while(tmp)
	{	
		i++;	
		tmp = tmp->next;
	}
	arr = malloc(sizeof(char *) * i);
	tmp = token;
	i = 0;
	while(tmp)
	{
		arr[i++] = tmp->content;
		tmp = tmp->next;
	}
	arr[i] = NULL;
	return arr;
}
