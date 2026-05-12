/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    ";DROP TABLE team",
    /* First member's full name */
    "Code Minister",
    /* First member's email address */
    "example@email.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define STATE(X) ((X) & (1))
#define SIZE(X)  ((X) & (~1))
#define ALLOC (1)
#define FREE  (0)
#define ALLOC_SPACE(X) ((X) | (1))
#define FREE_SPACE(X)  ((X) & (~1))

#define WORD_SIZE (sizeof(size_t))
#define DWORD_SIZE (2*sizeof(size_t))

#define NEXT_K_WORDS(ptr, k) ((size_t *) ((char *)ptr + k * WORD_SIZE))
#define NEXT(ptr, size) ((size_t *) ((char *)ptr + size))

#define HDR(ptr) (NEXT_K_WORDS(ptr, -1))
#define FTR(ptr) (NEXT(ptr, SIZE(*HDR(ptr)) - DWORD_SIZE))
#define POST_BLOCK(ptr) (NEXT(ptr, SIZE(*HDR(ptr))))
#define PREV_BLOCK(ptr) (NEXT(ptr, -SIZE(*NEXT_K_WORDS(ptr, -2))))

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    // 4 words for [blank, 8/1, 8/1, 0/1]
    // [blank, 0/1, 0/1] is forbidden for the data will be put at a place ruined 8-alignment.
    void* p = mem_sbrk(4 * WORD_SIZE);
    if (p == (void *)-1) {
        return -1;
    }
    *NEXT_K_WORDS(p, 1) = ALLOC_SPACE(8); // You cannot destroy the consistency
    *NEXT_K_WORDS(p, 2) = ALLOC_SPACE(8); // of 8-alignment.
    *NEXT_K_WORDS(p, 3) = ALLOC_SPACE(0);
    return 0;
}

/* 
 * mm_malloc - Implicit Link Version.
 */
void *mm_malloc(size_t size)
{
    // fprintf(stderr, "Count\n");
    int newsize = ALIGN(size + SIZE_T_SIZE);
    size_t* cur = NEXT_K_WORDS(mem_heap_lo(), 1);
    while (cur && ((SIZE(*cur) < newsize)        // First fit
                   || (STATE(*cur) == ALLOC)) && (SIZE(*cur) != 0)) {
        // printf("skipped %d long\n", ((*cur) & (~1)));
        cur = NEXT(cur, SIZE(*cur));
    }
    // fprintf(stdout, "cur: %p, *cur: %d, (-1)%d, (+1)%d\n", cur, *cur, *(size_t*)((char*)cur - 1), *(size_t*)((char*)cur + 1));
    // fflush(stdout);
    if (!cur) {
        fprintf(stderr, "mm_malloc error: null brk\n");
        return NULL;
    }
    if (SIZE(*cur) == 0 && STATE(*cur) == ALLOC) {
        // Allocate new space in heap
        // from [0/1] to [newsize/1, USER..., newsize/1, 0/1]
        void* p = mem_sbrk(newsize);
        // if (p == (void*)-1) printf("mem_sbrk fault\n");
        // fflush(stdout);
        // Header
        *HDR(p) = ALLOC_SPACE(newsize);
        // Footer
        *FTR(p) = ALLOC_SPACE(newsize);
        // Last
        *HDR(POST_BLOCK(p)) = ALLOC_SPACE(0);
        // [Header, USER..., Footer]
        return (void *)p;
    }
    if (SIZE(*cur) >= newsize && STATE(*cur) == FREE) {
        if (SIZE(*cur) > newsize) {
            size_t next_free_size = SIZE(*cur) - newsize;
            // Header for next free space
            size_t next_free_header_offset = newsize;
            *NEXT(cur, next_free_header_offset) = FREE_SPACE(next_free_size);
            // Footer for next free space
            size_t next_free_footer_offset = next_free_header_offset + next_free_size - WORD_SIZE;
            *NEXT(cur, next_free_footer_offset) = FREE_SPACE(next_free_size);
        }
        // Header
        *cur = ALLOC_SPACE(newsize);
        // Footer
        size_t footer_offset = newsize - WORD_SIZE;
        *NEXT(cur, footer_offset) = ALLOC_SPACE(newsize);
        
        return NEXT_K_WORDS(cur, 1);
    }
    fprintf(stderr, "mm_malloc: unkown mistake happens.\n");
    return 0;
}

/*
 * mm_free - Freeing a block.
 */
void mm_free(void *ptr)
{
    if (!ptr) return;
    // [Prev_Header, prev_size, Prev_Footer,
    //  Header, size(new_size - 2*SIZE_T_SIZE), Footer,
    //  Post_Footer, post_size, Post_Footer]
    size_t* block_header = HDR(ptr);
    size_t* block_footer = FTR(ptr);
    size_t* prev_header = HDR(PREV_BLOCK(ptr));
    size_t* post_footer = FTR(POST_BLOCK(ptr));
    size_t block_size = SIZE(*block_header);
    size_t prev_size = SIZE(*prev_header);
    size_t post_size = SIZE(*post_footer);
    int prev_state = STATE((int)(*prev_header));
    int post_state = STATE((int)(*post_footer));
    if (prev_state == ALLOC && post_state == ALLOC) {   // both blocks are allocated
        *block_header = FREE_SPACE(block_size);
        *block_footer = FREE_SPACE(block_size);
        return;
    }
    else if (prev_state == FREE  && post_state == ALLOC) {
        size_t newsize = prev_size + block_size;
        *prev_header   = FREE_SPACE(newsize);
        *block_footer  = FREE_SPACE(newsize);
    }
    else if (prev_state == ALLOC && post_state == FREE)  {
        size_t newsize = block_size + post_size;
        *block_header  = FREE_SPACE(newsize);
        *post_footer   = FREE_SPACE(newsize);
    }
    else if (prev_state == FREE  && post_state == FREE)  {
        size_t newsize = prev_size + block_size + post_size;
        *prev_header   = FREE_SPACE(newsize);
        *post_footer   = FREE_SPACE(newsize);
    }
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    size_t aligned_size = ALIGN(size + 2*WORD_SIZE);
    copySize = SIZE(*HDR(oldptr));
    if (aligned_size <= copySize) {
        if (aligned_size == copySize) {
            return (void *)oldptr;
        }
        size_t* header      = HDR(oldptr);
        size_t* new_footer  = NEXT(oldptr, aligned_size - DWORD_SIZE);
        size_t* free_header = HDR(NEXT(oldptr, aligned_size));
        size_t* free_footer = FTR(oldptr);

        *header      = ALLOC_SPACE(aligned_size);
        *new_footer  = ALLOC_SPACE(aligned_size);
        *free_header = FREE_SPACE(copySize - aligned_size);
        *free_footer = FREE_SPACE(copySize - aligned_size);
        return (void *)oldptr;
    }

    size_t* next_block_header = HDR(POST_BLOCK(ptr));
    size_t next_size = SIZE(*next_block_header);
    int next_block_state = STATE(*next_block_header);
    size_t total_size = next_size + copySize;

    if (next_block_state == FREE && total_size >= aligned_size) {
        // [block_header, block, block_footer, (next_header), (next_footer)]
        if (total_size > aligned_size) {
            size_t* next_header = NEXT(oldptr, aligned_size - WORD_SIZE);
            size_t* next_footer = NEXT(oldptr, total_size - DWORD_SIZE);
            *next_header = FREE_SPACE(total_size - aligned_size);
            *next_footer = FREE_SPACE(total_size - aligned_size);
        }
        size_t* block_header = NEXT_K_WORDS(oldptr, -1);
        size_t* block_footer = NEXT(oldptr, aligned_size - DWORD_SIZE);
        *block_header = ALLOC_SPACE(aligned_size);
        *block_footer = ALLOC_SPACE(aligned_size);

        return (void *)oldptr;
    }

    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}













