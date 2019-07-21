/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_content.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielmoudn <ielmoudn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 01:11:00 by ielmoudn          #+#    #+#             */
/*   Updated: 2019/07/21 20:51:17 by ielmoudn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void	read_content_helper1(struct dirent *dp, t_info **info,
	t_node **all, t_node **dirs)
{
	if (!(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")
		|| is_hidden(dp->d_name)))
	{
		normal_fill(info, dp);
		(*info)->insert_func((char*)(all), (char*)info);
		((*info)->list_len) += 1;
		if (is_dir(dp->d_type))
			(*info)->insert_func((char*)(dirs), (char*)info);
	}
}

void	read_content_helper2(t_node *all, t_node **dirs, t_node **head,
	t_info **info)
{
	t_node *tbf;

	tbf = all;
	if (all)
		(*info)->print_func(all, *info);
	else
		ft_printf("\n");
	if (tbf != NULL)
		free_nodes(tbf);
	tbf = *dirs;
	if ((*info)->flags & FLAG_RCAP)
	{
		while (*dirs)
		{
			(*info)->path_tbi = (*head)->path;
			(*info)->name_tbi = (*dirs)->name;
			read_content(dirs, info, (*info)->counter + 1);
			*dirs = (*dirs)->next;
		}
	}
	if (tbf != NULL)
		free_nodes(tbf);
}

void	read_content(t_node **head, t_info **info, int tracker)
{
	DIR				*dir;
	struct dirent	*dp;
	t_node			*dirs;
	t_node			*all;

	dirs = NULL;
	all = NULL;
	dir = opendir((*head)->path);
	if (!((*head)->perm & S_IXUSR))
		return ;
	if (!dir)
	{
		handle_error((*head)->name);
		return ;
	}
	if (tracker != 0)
		ft_printf("%s:\n", (*head)->path);
	(*info)->path_tbi = (*head)->path;
	(*info)->list_len = 0;
	(*info)->max_len = 0;
	while ((dp = readdir(dir)) != NULL)
		read_content_helper1(dp, info, &all, &dirs);
	(*info)->counter = tracker;
	read_content_helper2(all, &dirs, head, info);
	closedir(dir);
}
