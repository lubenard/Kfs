/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PS2_keyboard.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 17:01:06 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/14 15:23:44 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

typedef struct kbd_event {
	unsigned char key_typed;
	uint16_t key_typed_raw;
	// Certains key send 2 keycodes.
	// Keyboard arrows etc
	uint16_t key_typed_raw_two;
	short is_key_special;
}				kbd_event_t;

char *set_language(int language);
void get_last_typed_key(kbd_event_t *key);
void init_kbd();
