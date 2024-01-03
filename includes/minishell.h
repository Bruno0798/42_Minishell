/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:39:03 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/03 14:34:35 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <signal.h>
# include <sys/types.h>

typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
}				t_env;

typedef struct minishell
{
	t_list	*env;
}				t_minishell;
#endif