/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 18:08:16 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/03/18 17:01:04 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool is_between_quotes(char *str)
{
	int s_quote;  /* Counter for single quotes */
	int d_quote;  /* Counter for double quotes */
	int i;  /* Index for iterating over the string */

	i =-1;
	s_quote = 0;
	d_quote = 0;
	while (str[++i])  /* Iterate over each character in the string */
	{
		if (str[i] == DOUBLE_QUOTE && !s_quote)  /* If the character is a double quote and the single quote counter is 0 */
			d_quote = !d_quote;  /* Toggle the double quote counter */
		else if (str[i] == SINGLE_QUOTE && !d_quote)  /* If the character is a single quote and the double quote counter is 0 */
			s_quote = !s_quote;  /* Toggle the single quote counter */
	}
	if (s_quote != 0 || d_quote != 0)  /* If either the single quote or double quote counter is not 0 */
		return true;  /* Return true */
	return false;  /* Otherwise, return false */
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
	int	i;  /* Counter for the number of tokens and the current index in the array */
	char	**arr;  /* Array of strings representing the content of each token */
	t_token	*tmp;  /* Temporary pointer to the current token */

	i = 1;
	tmp = token;
	while(tmp)  /* Iterate over each token in the linked list */
	{
		if(tmp->type == redir_out)
			break;	
		i++;
		tmp = tmp->next;
	}
	arr = malloc(sizeof(char *) * i);  /* Allocate memory for the array of strings */
	tmp = token;
	i = 0;
	while(tmp)  /* Iterate over each token in the linked list again */
	{
		if(tmp->type == redir_out)
			break;	
		arr[i++] = tmp->content;  /* Assign the content of the current token to the corresponding element in the array */
		tmp = tmp->next;
	}
	arr[i] = NULL;  /* Set the last element of the array to NULL */
	return arr;  /* Return the array of strings */
}
