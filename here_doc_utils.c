/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:59:00 by mminet            #+#    #+#             */
/*   Updated: 2024/05/16 17:49:18 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
