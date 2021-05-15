typedef struct shell {
	uint16_t buffer[2000];
	unsigned short start_cmd_line;
	unsigned short cmd_size;
	unsigned short cursor_pos;
	unsigned short is_shell_init;
}				shell_t;

typedef struct terminal {
			shell_t *first;
			shell_t *second;
			shell_t *third;
			unsigned short active_shell;
}				terminal_t;

void init_shell();
