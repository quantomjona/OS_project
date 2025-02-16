#include "bitmap.h"
static inline size_t bitmap_elem_offset(size_t idx) {
    return idx % bitmap_elem_size();
  }
static inline uint8_t bitmap_elem_mask(size_t idx) {
    return 1 << bitmap_elem_offset(idx);
}
static inline bool bitmap_in_bounds(bitmap_t *bm, size_t idx) {
    return idx < bm->size;
}
void bitmap_mark(bitmap_t *bm, size_t i)
{
    if (!bitmap_in_bounds(bm, i))
        return;
    bm->bits[bitmap_elem_index(i)] |= bitmap_elem_mask(i);
}

void bitmap_reset(bitmap_t *bm, size_t i)
{
    if (!bitmap_in_bounds(bm, i))
        return;
    bm->bits[bitmap_elem_index(i)] &= ~bitmap_elem_mask(i);
}
void bitmap_flip(bitmap_t *bm, size_t i)
{
    if (!bitmap_in_bounds(bm, i))
        return;
    bm->bits[bitmap_elem_index(i)] ^= bitmap_elem_mask(i);
}

bool bitmap_test(const bitmap_t *bm, size_t i)
{
    if (!bitmap_in_bounds(bm, i))
        return 0;
    return (bm->bits[bitmap_elem_index(i)] & bitmap_elem_mask(i)) != 0;
}

void bitmap_set(bitmap_t *bm, bool v, size_t idx)
{
    v &= 0x1;
    if (v)
        bitmap_mark(bm, idx);
    else
        bitmap_reset(bm, idx);
}

void bitmap_setmulti(bitmap_t *bm, bool v, size_t idx, size_t c)
{
    for (int i = 0; i < bm->size - idx && i < c; i++)
        bitmap_set(bm, v, idx + i);
}

