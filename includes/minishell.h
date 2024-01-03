/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:39:03 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/03 17:36:11 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
}				t_env;

typedef struct s_variables
{
	char	*home;
	char	*path;
	char	**paths;
}				t_variables;

typedef struct s_shell
{
	t_list	*env;
	t_variables *var;
}				t_shell;

#endif