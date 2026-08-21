/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 17:27:48 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/07 17:45:17 by nappasam         ###   ########.fr       */
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

void	sift_up(t_heap *heap, int i)
{
	int	parent;

	parent = (i - 1) / 2;
	while (i > 0 && heap->data[i].key < heap->data[parent].key)
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
		if (2 * i + 1 < heap->size
			&& heap->data[2 * i + 1].key < heap->data[smallest].key)
			smallest = 2 * i + 1;
		if (2 * i + 2 < heap->size
			&& heap->data[2 * i + 2].key < heap->data[smallest].key)
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
