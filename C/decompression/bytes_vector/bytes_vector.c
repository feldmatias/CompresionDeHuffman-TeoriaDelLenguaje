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

int bytes_vector_add_bytes(const bytes_vector_t* self, const char* bytes) {

}

const char* bytes_vector_get_ptr(const bytes_vector_t* self) {
    return self->vec;
}

int bytes_vector_len(const bytes_vector_t* self) {
    return self->len;
}

int bytes_vector_release(bytes_vector_t* self) {
    free(self->vec);
    return SUCCESS;
}