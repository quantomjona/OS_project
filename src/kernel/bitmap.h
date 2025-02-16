#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#ifndef BITMAP_H
#define BITMAP_H
typedef uint8_t bitmap_elem_t;

typedef struct bitmap
{
    bitmap_elem_t *bits;
    size_t size;
} bitmap_t;
void bitmap_setmulti(bitmap_t *bm, bool v, size_t idx, size_t c);
void bitmap_set(bitmap_t *bm, bool v, size_t idx);
bool bitmap_test(const bitmap_t *bm, size_t i);
void bitmap_flip(bitmap_t *bm, size_t i);
void bitmap_reset(bitmap_t *bm, size_t i);

static inline size_t bitmap_elem_size() { return sizeof(bitmap_elem_t) * 8; }
static inline size_t bitmap_size(const bitmap_t *bm) { return bm->size; }
static inline size_t bitmap_elem_index(size_t idx) {
  return idx / bitmap_elem_size();
}
static inline size_t bitmap_byte_size(const bitmap_t *bm) {
  return bitmap_elem_index(bitmap_size(bm) - 1) + 1;
}
void bitmap_mark(bitmap_t *bm, size_t i);
#endif