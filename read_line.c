/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:20:23 by mminet            #+#    #+#             */
/*   Updated: 2024/05/17 13:34:52 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_sig_check = 0;

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
	str = ft_strdup("\001\e[00;1m\002 \001\e[36m\002");
	tmp = str;
	str = ft_strjoin(str, pwd + i);
	free(tmp);
	tmp = str;
	str = ft_strjoin(str, "\001\e[00;0m\002 ");
	free(tmp);
	return (str);
}

void	handler_exit(int sig)
{
	(void)sig;
	g_sig_check = 1;
	ft_putstr_fd("\n", 1);
	dup2(open("/dev/null", O_RDONLY, 00664), STDIN_FILENO);
}

void	handler(int sig)
{
	(void)sig;
	g_sig_check = 1;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_signal(int *status, t_list **my_env, char *input,
		struct sigaction sa)
{
	if (g_sig_check)
	{
		g_sig_check = 0;
		*status = 130;
	}
	if (ft_strlen(input))
	{
		sa.sa_handler = &handler_exit;
		sigaction(SIGINT, &sa, NULL);
		add_history(input);
		*status = check_input(input, my_env, *status);
		sa.sa_handler = &handler;
		sigaction(SIGINT, &sa, NULL);
	}
}

void	get_input(t_list **my_env)
{
	char				*input;
	int					status;
	char				*tmp;
	struct sigaction	sa;

	sa.sa_handler = &handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	status = 0;
	tmp = print_prompt(status);
	input = NULL;
	input = readline(tmp);
	free(tmp);
	while (input != NULL)
	{
		handle_signal(&status, my_env, input, sa);
		free(input);
		tmp = print_prompt(status);
		input = readline(tmp);
		free(tmp);
	}
	rl_clear_history();
}
