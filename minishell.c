/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 23:32:42 by mminet            #+#    #+#             */
/*   Updated: 2024/04/23 02:26:39 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    del(void    *del)
{   
    free(del);
}

void   *get_env(t_list **lst, char **env)
{
    int     i;
    char    *content;
    
    i = 0;
    while (env[i] != NULL)
    {
        content = ft_strdup(env[i]);
        ft_lstadd_back(lst, ft_lstnew(content));
        i++;
    }
    return (NULL);
}

char    *get_var(char *cmp, t_list *env)
{
    t_list *tmp;
    int     i;
    char    *cpy;

    tmp = env;
    while (tmp && ft_strncmp(cmp, (char *)tmp->content, ft_strlen(cmp)))
        tmp = tmp->next;
    if (!tmp)
        return (NULL);
    cpy = (char *)tmp->content;
    i = 0;
    while (cpy[i] && cpy[i] != '=')
        i++;
    if (!cpy)
        return (NULL);
    return (cpy + i + 1);
}

char    *my_input(char *input, t_list *env)
{
    int i;
    char    *my_input;

    i = 0;
}

void    minishell(t_list *env)
{
    char    *input;
  //  t_list  *token;
    
    (void)env;
    ft_putstr_fd(">", 1);
    input = get_next_line(0);
  //  token = get_token(my_input(input, env));
    printf("%s\n", my_input(input, env));
    free(input);
}

int main(int ac, char **av, char **env)
{
    t_list   *my_env;

    (void)av;
    if (ac != 1)
        return (1);
    my_env = NULL;
    get_env(&my_env, env);
    minishell(my_env);
    ft_lstclear(&my_env, del);
}
