/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PS2_keyboard.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 17:01:06 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/11 10:54:30 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PS2_KEYBOARD_H
# define PS2_KEYBOARD_H

typedef unsigned short          uint16_t;

# define DELETE_KEY 0x0E
# define F1_KEY 0x3B
# define F2_KEY 0x3C
# define F3_KEY 0x3D

typedef struct kbd_event {
	unsigned char key_typed;
	uint16_t key_typed_raw;
	// Certains key send 2 keycodes.
	// Keyboard arrows etc
	uint16_t key_typed_raw_two;
	short is_key_special;
}				kbd_event_t;

typedef struct kbd_listener {
	struct kbd_listener *prev;
	void (*listener)(kbd_event_t *kbd_event);
	struct kbd_listener *next;
}				kbd_listener_t;

char *set_language(int language);
void get_last_typed_key(kbd_event_t *key);
void register_kbd_listener(kbd_listener_t *listener);
void init_kbd();

static char *g_kbd_list[] = {"QWERTY", "AZERTY", 0};

#endif
