#ifndef RINGBUFFER_H__
#define RINGBUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ring buffer */
struct st_ringbuffer
{
    uint8_t *buffer_ptr;
    /* use the msb of the {read,write}_index as mirror bit. You can see this as
     * if the buffer adds a virtual mirror and the pointers point either to the
     * normal or to the mirrored buffer. If the write_index has the same value
     * with the read_index, but in a different mirror, the buffer is full.
     * While if the write_index and the read_index are the same and within the
     * same mirror, the buffer is empty. The ASCII art of the ringbuffer is:
     *
     *          mirror = 0                    mirror = 1
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Full
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     *  read_idx-^                   write_idx-^
     *
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Empty
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * read_idx-^ ^-write_idx
     *
     * The tradeoff is we could only use 32KiB of buffer for 16 bit of index.
     * But it should be enough for most of the cases.
     *
     * Ref: http://en.wikipedia.org/wiki/Circular_buffer#Mirroring */
    uint16_t read_mirror : 1;
    uint16_t read_index : 15;
    uint16_t write_mirror : 1;
    uint16_t write_index : 15;
    /* as we use msb of index as mirror bit, the size should be signed and
     * could only be positive. */
    int16_t buffer_size;
};

enum ringbuffer_state
{
    RINGBUFFER_EMPTY,
    RINGBUFFER_FULL,
    /* half full is neither full nor empty */
    RINGBUFFER_HALFFULL,
};

/**
 * RingBuffer for DeviceDriver
 *
 * Please note that the ring buffer implementation
 * has no thread wait or resume feature.
 */
enum ringbuffer_state ringbuffer_status(struct st_ringbuffer *rb);
void ringbuffer_init(struct st_ringbuffer *rb, uint8_t *pool, int16_t size);
void ringbuffer_reset(struct st_ringbuffer *rb);
uint32_t ringbuffer_put(struct st_ringbuffer *rb, const uint8_t *ptr, uint16_t length);
uint32_t ringbuffer_put_force(struct st_ringbuffer *rb, const uint8_t *ptr, uint16_t length);
uint32_t ringbuffer_putchar(struct st_ringbuffer *rb, const uint8_t ch);
uint32_t ringbuffer_putchar_force(struct st_ringbuffer *rb, const uint8_t ch);
uint32_t ringbuffer_get(struct st_ringbuffer *rb, uint8_t *ptr, uint16_t length);
uint8_t ringbuffer_getchar(struct st_ringbuffer *rb, uint8_t *ch);
uint32_t ringbuffer_data_len(struct st_ringbuffer *rb);

#ifdef _USING_HEAP
struct st_ringbuffer* ringbuffer_create(uint16_t length);
void ringbuffer_destroy(struct st_ringbuffer *rb);
#endif

inline int16_t ringbuffer_get_size(struct st_ringbuffer *rb)
{
    //ASSERT(rb != NULL);
    return rb->buffer_size;
}

/** return the size of empty space in rb */
#define ringbuffer_space_len(rb) ((rb)->buffer_size - ringbuffer_data_len(rb))


#ifdef __cplusplus
}
#endif

#endif
