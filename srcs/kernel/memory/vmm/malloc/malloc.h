/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 13:55:29 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/27 13:20:28 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <stddef.h>
# include <stdint.h>

# define PAGESIZE 16384
# define MAGIC_NUMBER 0x7A12
# define TINY 16384
# define SMALL 24576
# define LARGE 24577
# define ALLOC_FREE 1
# define ALLOC_USED 2

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void	*calloc(size_t nitems, size_t size);

struct s_block {
	char			buffer[3];
	unsigned int	pid;
	unsigned int	total_node;
	unsigned int	total_size;
	struct s_block	*next;
	struct s_block	*prev;
}			__attribute__((packed));

typedef struct s_block t_block;

# define STRUCT_BLOCK_SIZE sizeof(t_block)

struct s_alloc {
	short	buffer_overflow;
	int		size;
	char	is_busy;
	struct	s_alloc *next;
	struct	s_alloc *prev;
	struct	s_block *block;
}			__attribute__((packed));

typedef struct s_alloc t_alloc;

# define STRUCT_SIZE sizeof(t_alloc)

# define TOTAL_STRUCT_SIZE STRUCT_SIZE + STRUCT_BLOCK_SIZE

// Internal functions
t_alloc	*split_node(t_alloc *node, size_t size_of_block);
void	*real_malloc(size_t size);
void	real_free(void *ptr);

#endif
