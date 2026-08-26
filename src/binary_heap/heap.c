/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 17:27:48 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/24 18:39:45 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_before(t_request a, t_request b)
{
	if (a.key < b.key)
		return (1);
	if (a.key == b.key && a.identity < b.identity)
		return (1);
	return (0);
}

void	sift_up(t_heap *heap, int i)
{
	int	parent;

	parent = (i - 1) / 2;
	while (i > 0 && is_before(heap->data[i], heap->data[parent]))
	{
		swap(&heap->data[i], &heap->data[parent]);
		i = parent;
		parent = (i - 1) / 2;
	}
}

void	sift_down(t_heap *heap, int i)
{
	int	smallest;

	while (1)
	{
		smallest = i;
		if (2 * i + 1 < heap->size && is_before(heap->data[2 * i + 1],
				heap->data[smallest]))
			smallest = 2 * i + 1;
		if (2 * i + 2 < heap->size && is_before(heap->data[2 * i + 2],
				heap->data[smallest]))
			smallest = 2 * i + 2;
		if (smallest == i)
			break ;
		swap(&heap->data[i], &heap->data[smallest]);
		i = smallest;
	}
}

void	heap_insert(t_heap *heap, t_request value)
{
	heap->data[heap->size] = value;
	sift_up(heap, heap->size);
	heap->size++;
}

t_request	extract_min(t_heap *heap)
{
	t_request	min;
	t_request	empty;

	if (heap->size == 0)
	{
		empty.identity = -1;
		empty.key = 0;
		return (empty);
	}
	min = heap->data[0];
	heap->data[0] = heap->data[heap->size - 1];
	heap->size--;
	sift_down(heap, 0);
	return (min);
}
