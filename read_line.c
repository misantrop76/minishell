/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:20:23 by mminet            #+#    #+#             */
/*   Updated: 2024/05/10 15:43:22 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*print_prompt(int status)
{
	char	*str;
	char	pwd[100];
	char	*tmp;
	int		i;

	getcwd(pwd, 99);
	i = ft_strlen(pwd) - 1;
	while (i - 1 >= 0 && pwd[i - 1] != '/')
		i--;
	(void)status;
	if (status)
		str = ft_strdup("\001\e[00;31m\002> \001\e[00;1m\002 \001\e[36m\002");
	else
		str = ft_strdup("\001\e[00;32m\002> \001\e[00;1m\002 \001\e[36m\002");
	tmp = str;
	str = ft_strjoin(str, pwd + i);
	free(tmp);
	tmp = str;
	str = ft_strjoin(str, "\001\e[00;0m\002 ");
	free(tmp);
	return (str);
}

void	handler_ignor(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", 1);
}

void	handler(int sig, siginfo_t *info, void *ptr)
{
	struct SharedData *s = (struct SheredData *)info->info->si_value.sival_ptr;
	(void)sig;
	*(s->status) = 130;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	get_input(t_list **my_env)
{
	char				*input;
	int					status;
	char				*tmp;
	struct sigaction	sa;
    struct SharedData	shared_data;

	sa.sa_handler = &handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_flags |= SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	
	status = 0;
	shared_data.status = &status;
	tmp = print_prompt(status);
	input = NULL;
	input = readline(tmp);
	free(tmp);
	while (input != NULL)
	{
		if (ft_strlen(input))
		{
			sa.sa_handler = &handler_ignor;
			sigaction(SIGINT, &sa, NULL);
			add_history(input);
			status = check_input(input, my_env, status);
			sa.sa_handler = &handler;
			sigaction(SIGINT, &sa, NULL);
		}
		free(input);
		tmp = print_prompt(status);
		input = readline(tmp);
		free(tmp);
	}
	rl_clear_history();
}
