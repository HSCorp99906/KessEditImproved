#include "../include/Buffer.h"


void buffer_init(struct Buffer* buffer) {
	buffer->content = (char*)calloc(2, sizeof(char));
	buffer->size = 2;
	buffer->idx = 0;
}


void buffer_insert(struct Buffer* buffer, char ch) {
	buffer->content[buffer->idx] = ch;
	++buffer->idx;
	++buffer->size;
	buffer->content = (char*)realloc(buffer->content, sizeof(char) * buffer->size);
}


void buffer_pop(struct Buffer* buffer, unsigned long idx) {
	char* bufferCpy = (char*)calloc(buffer->size, sizeof(char));
	unsigned long bufferCpyIdx = 0;

	for (int i = 0; i < buffer->size; ++i) {
		if (i == idx) {
			continue;
		}

		bufferCpy[bufferCpyIdx] = buffer->content[i];
		++bufferCpyIdx;
	}

	--buffer->size;
	--buffer->idx;
	buffer->content = (char*)realloc(buffer->content, sizeof(buffer->size));
	memset(buffer->content, '\0', buffer->size);
	strcpy(buffer->content, bufferCpy);
	free(bufferCpy);
}


void buffer_pop_back(struct Buffer* buffer) {
	buffer->content[buffer->idx - 1] = '\0';
	--buffer->size;
	--buffer->idx;
	buffer->content = (char*)realloc(buffer->content, sizeof(char) * buffer->size);
	buffer->content[buffer->size] = '\0';
}


void buffer_free(struct Buffer* buffer) {
	free(buffer->content);
	buffer->content = NULL;
}
