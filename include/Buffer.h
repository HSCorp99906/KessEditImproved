#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <string.h>


struct Buffer {
	char* content;
	size_t size;
	unsigned long idx;
};


void buffer_init(struct Buffer* buffer);
void buffer_insert(struct Buffer* buffer, char ch);
void buffer_pop(struct Buffer* buffer, unsigned long idx);
void buffer_pop_back(struct Buffer* buffer);
void buffer_free(struct Buffer* buffer);


#endif
