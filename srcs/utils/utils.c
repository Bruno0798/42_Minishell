/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 18:08:16 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/03/29 17:04:45 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**ft_lst_to_arr(t_token *token)
{
	int		i;		/* Counter for the number of tokens and the current index in the array */
	char	**arr;	/* Array of strings representing the content of each token */
	t_token	*tmp;	/* Temporary pointer to the current token */

	i = 1;
	tmp = token;
	while (tmp)	/* Iterate over each token in the linked list */
	{
		if (tmp->type == redir_out || tmp->type == redir_out2
			|| tmp->type == redir_in)
			break ;
		i++;
		tmp = tmp->next;
	}
	arr = malloc(sizeof(char *) * i);	/* Allocate memory for the array of strings */
	tmp = token;
	i = 0;
	while (tmp)  /* Iterate over each token in the linked list again */
	{
		if (tmp->type == redir_out || tmp->type == redir_out2
			|| tmp->type == redir_in)
			break ;
		arr[i++] = tmp->content;	/* Assign the content of the current token to the corresponding element in the array */
		tmp = tmp->next;
	}
	arr[i] = NULL;  /* Set the last element of the array to NULL */
	return (arr);  /* Return the array of strings */
}
