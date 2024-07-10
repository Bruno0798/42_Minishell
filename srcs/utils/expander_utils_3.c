/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:50:52 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/10 14:50:52 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	replace_value_with_key(char **new_string,
		char *value, char *key, char *string)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (string[i])
	{
		if (string[i] == '$'
			&& ft_strncmp(&string[i + 1], value, ft_strlen(value)) == 0)
		{
			copy_key_to_new_string(*new_string, &j, key);
			i += ft_strlen(value) + 1;
			while (string[i])
				*new_string[(j)++] = string[(i)++];
			break ;
		}
		*new_string[j++] = string[i++];
	}
	*new_string[j] = '\0';
}
