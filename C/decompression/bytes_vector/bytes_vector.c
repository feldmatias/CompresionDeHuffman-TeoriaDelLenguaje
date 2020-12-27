#include "bytes_vector.h"

#include <stdlib.h>

#define INITIAL_SIZE 10
#define MEMORY_ERROR -1
#define SUCCESS 0

int bytes_vector_init(bytes_vector_t* self) {
    self->vec = malloc(sizeof(char) * INITIAL_SIZE);
    if (!self->vec) {
        return MEMORY_ERROR;
    }
    self->vec[0] = 0; //String vacio
    self->size = INITIAL_SIZE;
    self->len = 0;
    return SUCCESS;
}

int bytes_vector_add_byte(bytes_vector_t* self, char byte) {
    if (self->len == self->size - 1) {
        char* aux_ptr = realloc(self->vec, self->size * 2);
        if (!aux_ptr) {
            return MEMORY_ERROR;
        } else {
            self->vec = aux_ptr;
            self->size *= 2;
        }
    }
    self->vec[self->len] = byte;
    self->len++;
    self->vec[self->len] = 0;
    return SUCCESS;
}

const char* bytes_vector_get_ptr(const bytes_vector_t* self) {
    return self->vec;
}

int bytes_vector_len(const bytes_vector_t* self) {
    return self->len;
}

void bytes_vector_release(bytes_vector_t* self) {
    free(self->vec);
}

void bytes_vector_replace_byte(bytes_vector_t* self, int pos, char replacement) {
    if (pos < self->size) self->vec[pos] = replacement;//Ver si devuelvo un error o no hago nada
}
