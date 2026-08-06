/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 17:27:48 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/06 19:06:47 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


typedef struct s_heap
{
    int data[64];
    int size;
    int capacity;
}               t_heap;


void swap(int *a, int *b)
{
    int tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

void init_heap(t_heap *heap)
{
    heap->size = 0;
}

void sift_up(t_heap *heap, int i)
{
    int parent;

    parent = (i - 1) / 2;
    while (i > 0 && heap->data[i] < heap->data[parent])
    {
        swap(&heap->data[i], &heap->data[parent]);
        i = parent;
        parent = (i - 1) / 2;
    }
}

void heap_insert(t_heap *heap, int value)
{
    heap->data[heap->size] = value;
    sift_up(heap, heap->size);
    heap->size++;
}

void	sift_down(t_heap *heap, int i)
{
	int	smallest;

	while (1)
	{
		smallest = i;
		if (2 * i + 1 < heap->size
			&& heap->data[2 * i + 1] < heap->data[smallest])
			smallest = 2 * i + 1;
		if (2 * i + 2 < heap->size
			&& heap->data[2 * i + 2] < heap->data[smallest])
			smallest = 2 * i + 2;
		if (smallest == i)
			break ;
		swap(&heap->data[i], &heap->data[smallest]);
		i = smallest;
	}
}