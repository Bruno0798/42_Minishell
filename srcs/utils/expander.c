/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 18:04:33 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/02/26 19:02:45 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_expand_join(char *s1, char *s2, t_env *env);

void ft_expander(t_commands *command) {



	t_token *c_token = command->token;
	while (c_token) {
		int i = 0, quote = 0;
		while (c_token->content[i]) {
			if ((c_token->content[i] == '"' || c_token->content[i] == '\'') && !quote) {
				quote = c_token->content[i];
			} else if (c_token->content[i] == '$' &&
					   (quote != '\'' || c_token->content[i - 1] != '\\')) {
				int len = strcspn(c_token->content + i + 1, " \t\'\"");
				char *var = ft_substr(c_token->content + i + 1, 0, len);
				t_env *tmp = ft_fnd_env(command->env, var);
				free(var);
				var = tmp ? ft_strdup(tmp->value) : ft_strdup("");
				char *new_content = ft_strjoin(ft_substr(c_token->content, 0, i), var);
				new_content = ft_strjoin(new_content, ft_substr(c_token->content, i + 1 + len,
																strlen(c_token->content) - i - 1 - len));
				free(c_token->content);
				c_token->content = new_content;
			}
			if (c_token->content[i] == quote) {
				quote = 0;
			}
			i++;
		}
		c_token = c_token->next;
	}
}

char	*ft_expand_join(char *s1, char *s2, t_env *env)
{
	char	*var;  /* Variable to be expanded */
	t_env	*tmp;  /* Temporary pointer to an environment variable */
	int 	len;  /* Length of the variable */

	len = ft_strchr(s2, ' ') - s2;  /* Find the length of the variable */
	if(!ft_strchr(s2, ' ')) /* If the variable is not followed by a space */
		len = ft_strchr(s2, SINGLE_QUOTE) - s2; /* Find the length of the variable */
	var = ft_substr(s2, 0, len);  /* Create a substring representing the variable */
	tmp = ft_fnd_env(env, var);  /* Find the value of the variable in the environment */
	free(var);
	if (tmp)
		var = ft_strdup(tmp->value);  /* If the variable is found, duplicate its value */
	else
		var = ft_strdup("");  /* If the variable is not found, duplicate an empty string */
	var = ft_strjoin(var, s2 + len);  /* Join the value of the variable with the rest of the second string */
	var = ft_strjoin(s1, var);  /* Join this with the first string */
	return var;  /* Return the resulting string */
}

char *ft_get_value(t_env *env, char *key)
{
	t_env *current;  /* Pointer to the current environment variable */

	current = env;  /* Initialize the current environment variable */
	while(current)  /* Iterate over each environment variable */
	{
		if (!ft_strcmp(current->key, key))  /* If the key of the current environment variable matches the provided key */
			return current->value;  /* Return the value of the current environment variable */
		current = current->next;  /* Move to the next environment variable */
	}
	return NULL;  /* If no matching key is found, return NULL */
}
