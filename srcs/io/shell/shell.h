typedef struct terminals {
	unsigned char buffer[VGA_WIDTH * VGA_HEIGHT];
	size_t terminal_row;
	size_t terminal_column;
	unsigned int cursor_pos;
	int fg_color;
	int bg_color;
}				terminals_t;
