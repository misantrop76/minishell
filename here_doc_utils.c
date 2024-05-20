/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:59:00 by mminet            #+#    #+#             */
/*   Updated: 2024/05/20 17:28:16 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_here_doc(t_pipex *pipex, t_list **env)
{
	t_list	*tmp;
	t_token	*token;

	tmp = pipex->tmp;
	while (tmp)
	{
		token = tmp->content;
		if (ft_strncmp(token->type, "READ", 4) == 0 && g_sig_check == 0)
			heredoc(token->value, pipex, env);
		tmp = tmp->next;
	}
}

void	wait_child(pid_t pid)
{
	waitpid(pid, NULL, 0);
	waitpid(pid, NULL, 0);
	waitpid(pid, NULL, 0);
	waitpid(pid, NULL, 0);
	waitpid(pid, NULL, 0);
	waitpid(pid, NULL, 0);
	waitpid(pid, NULL, 0);
}
