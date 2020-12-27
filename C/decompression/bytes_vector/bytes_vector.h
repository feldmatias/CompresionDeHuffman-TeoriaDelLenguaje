#ifndef C_BYTES_VECTOR_H
#define C_BYTES_VECTOR_H

//Tries to emulate a limited string
//The len can be up to size - 1 due to the null byte

//All attributes are private
typedef struct bytes_vector {
    char* vec;
    int size;
    int len;
} bytes_vector_t;

//Returns 0 if successful, otherwise returns -1
int bytes_vector_init(bytes_vector_t* self);

int bytes_vector_add_byte(bytes_vector_t* self, char byte);

const char* bytes_vector_get_ptr(const bytes_vector_t* self);

int bytes_vector_len(const bytes_vector_t* self);

void bytes_vector_release(bytes_vector_t* self);

void bytes_vector_replace_byte(bytes_vector_t* self, int pos, char replacement);

#endif //C_BYTES_VECTOR_H
