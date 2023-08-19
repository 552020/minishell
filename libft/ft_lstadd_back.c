/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 00:08:38 by slombard          #+#    #+#             */
/*   Updated: 2023/01/17 18:32:34 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*first_node;
	t_list	*last_node;

	if (!lst)
		return ;
	if (*lst)
	{
		first_node = *lst;
		last_node = ft_lstlast(first_node);
		last_node->next = new;
	}
	else
		*lst = new;
}
