/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_bytes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlobunet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 19:40:00 by vlobunet          #+#    #+#             */
/*   Updated: 2020/01/27 19:40:01 by vlobunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/nm_otool.h"

uint16_t	get_2b(uint16_t n)
{
	if (g_f.is_64)
		return ((n >> 8) | (n << 8));
	return (n);
}

uint32_t	get_4b(uint32_t n)
{
	if (!g_f.is_64)
		return ((n >> 24) | ((n & 0xff0000) >> 8) | \
			((n & 0xff00) << 8) | (n << 24));
		return (n);
}

uint64_t	get_8b(uint64_t n)
{
	if (!g_f.is_64)
		return ((n & 0xff00000000000000) >> 56 \
			| (n & 0x00ff000000000000) >> 40 \
			| (n & 0x0000ff0000000000) >> 24 \
			| (n & 0x000000ff00000000) >> 8 \
			| (n & 0x00000000ff000000) << 8 \
			| (n & 0x0000000000ff0000) << 24 \
			| (n & 0x000000000000ff00) << 40 \
			| (n & 0x00000000000000ff) << 56);
		return (n);
}

void		*get_struct(const uint64_t offset, const size_t size)
{
	if (offset + size < offset)
		return (NULL);
	return ((void *)((size_t)(g_f.p + g_f.ofset + offset) * \
		(g_f.ofset + offset + size <= g_f.size)));
}
