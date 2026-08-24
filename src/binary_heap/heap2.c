/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 18:01:07 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/24 18:39:29 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	init_heap(t_heap *heap, int capacity)
{
	heap->size = 0;
	heap->capacity = capacity;
	heap->data = malloc(capacity * sizeof(t_request));
	if (!heap->data)
		return (1);
	return (0);
}
