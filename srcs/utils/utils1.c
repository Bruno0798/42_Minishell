/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 10:29:46 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/16 19:05:52 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_print_token_list(t_token *head) 
{
	t_token	*token = head;

	while (token != NULL) {
		printf("Token: '%s' \nType: '%d'\n\n", token->content, token->type);
		token = token->next;
	}
}

// char	*ft_search_key(t_env *env, char *search)
// {
// 	t_env	*current = env;

// 	while (current != NULL)
// 	{
// 		if (!ft_strcmp(current->key, search))
// 			return (current->value);
// 		current = current->next;
// 	}
// 	return (NULL);
// } 

// bool	is_space(char c)
// {
// 	if (c == 32)
// 		return true;
// 	return false;
// }

// int	find_next_space(char *str)
// {
// 	int	i = 0;

// 	while (str[i] && str[i] != 0)
// 		i++;
// 	return i;
	
// }

t_type ft_token_type(char *word)
{
	if (!ft_strcmp(word, "|"))
		return pipes;
	else if (!ft_strcmp(word, ">>"))
		return redir_out2;
	else if (!ft_strcmp(word, ">"))
		return redir_out;
	else if (!ft_strcmp(word, "<<"))
		return redir_in2;
	else if (!ft_strcmp(word, "<"))
		return redir_in;
	return command;
}

// void create_token(t_token token, char *str)
// {
	
// }

int count_pipes(char *str)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (str[++i])
		if (str[i] == '|')
			count++;
	return (count);
}

// ft_fix_spaces(char **words)
// {
// 	int i;
// 	int j;

// 	i = -1;
// 	j = -1;
// 	while(words[++i])
// 	{
// 		ft_split(words[i], ' ');
// 	}
// }

t_token *ft_new_token(char *str)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	token->content = str;
	token->type = ft_token_type(str);	
	token->next = NULL;
	return (token);
}

void	pipe_commands(char *str, t_commands **command)
{
	char	**words;
	int		count;
	int		i;

	i = 0;
	count = count_pipes(str);
	if (!count)
	{
		t_token *temp;
		words = ft_split(str, ' '); //! Refacture: Function to not split spaces in quotes
		temp = ft_new_token(words[0]);
		(*command)->token = temp;  
		while (words[++i])
		{
			temp->next = ft_new_token(words[i]);
			temp = temp->next;
		}
	}
	// else
	// {
		
	// 	token->next = ft_new_token();
	// }
					
	// words = ft_split(str, '|');
	// ft_fix_spaces(words);
	// return (words ? words : 0);
}


