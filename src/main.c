#include <ncurses.h>
#include <stdio.h>
#include <unistd.h> 
#include <stdbool.h>
#include <signal.h>
#include "../include/Buffer.h"


void putcur(WINDOW* win, unsigned long r, unsigned long c) {
	wattron(win, A_STANDOUT);
	mvwprintw(win, r, c, "|");
	wattroff(win, A_STANDOUT);
}


void sigint_handler(int dummy) {
	// DO NOTHING ON SIGINT.	
}


void number_lines(WINDOW* win, unsigned long rows) {
	for (int i = 1; i <= rows; ++i) {
		mvwprintw(win, i, 0, "~%d ", i);
	}
}


int main(int argc, char* argv[]) {
	signal(SIGINT, sigint_handler);

	if (argc < 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 0;
	}

	if (access(argv[1], F_OK) == 0) {
		printf("Editing existing files is not supported yet.\n");
		return 1;
	}

	initscr();
	noecho();
	curs_set(0);

	FILE* fp = fopen(argv[1], "w");

	WINDOW* win = newwin(1000, 1000, 0, 0);
	
	unsigned long row = 1;
	unsigned long col = 5;

	bool run = true;
	
	size_t lccSize = 1;  // Last col collection.
	unsigned long lccIdx = 0;  // Last col collection.
	unsigned int* lastColCollection = (unsigned int*)malloc(sizeof(unsigned int));

	number_lines(win, 1);

	struct Buffer buffer;
	buffer_init(&buffer);

	while (run) {
		putcur(win, row, col);
		char ch = wgetch(win);
		mvwprintw(win, row, col, "  ");

		switch (ch) {
			case '`':
				run = false;
				continue;
			case 127:
				if (col > 5) {
					buffer_pop_back(&buffer);
					--col;
				} else if (row > 1) {
					mvwprintw(win, row, 0, "    ");
					buffer_pop_back(&buffer);
					--row;
					col = lastColCollection[lccIdx - 1];
					--lccIdx;
					--lccSize;
					lastColCollection = (unsigned int*)realloc(lastColCollection, sizeof(unsigned int) * lccSize);
				}
				mvwprintw(win, row, col, "  %c  ", 0x08);
				continue;
			case 10:
				lastColCollection[lccIdx] = col;
				++lccIdx;
				++lccSize;
				lastColCollection = (unsigned int*)realloc(lastColCollection, sizeof(unsigned int) * lccSize);

				buffer_insert(&buffer, '\n');
				col = 5;
				++row;
				number_lines(win, row);
				continue;
		}

		if (col > 70) {
			col = 0;
			++row;
		}

		buffer_insert(&buffer, ch);
		mvwprintw(win, row, col, "%c", ch);
		++col;
	}

	
	fprintf(fp, "%s\n", buffer.content);
	buffer_free(&buffer);

	free(lastColCollection);
	endwin();
	delwin(win);
	fclose(fp);

	return 0;
}
