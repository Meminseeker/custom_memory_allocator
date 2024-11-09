#include <stdio.h>
#include <stdbool.h>

#define POOL_SIZE 1024

typedef struct Block
{
    size_t size;
    bool is_free;
    struct Block *next;
} Block;

#define BLOCK_SIZE sizeof(Block)

char memory_pool[POOL_SIZE];

Block *free_list = (Block *)memory_pool;

void init_pool()
{
    free_list->size = POOL_SIZE - BLOCK_SIZE;
    free_list->is_free = true;
    free_list->next = NULL;
};

void split_block(Block *block, size_t size)
{
    Block *new_block = (Block *)((char *)block + BLOCK_SIZE + size);
    new_block->size = block->size - size - BLOCK_SIZE;
    new_block->is_free = true;
    new_block->next = block->next;

    block->size = size;
    block->is_free = false;
    block->next = new_block;
}

void *my_malloc(size_t size)
{
    Block *current = free_list;

    while (current != NULL)
    {
        if (current->is_free && current->size >= size)
        {
            if (current->size > size + BLOCK_SIZE)
            {
                split_block(current, size);
            }
            else
            {
                current->is_free = false;
            }
            return (char *)current + BLOCK_SIZE;
        }
        current = current->next;
    }

    return NULL;
}

void merge_blocks()
{
    Block *current = free_list;
    while (current != NULL && current->next != NULL)
    {
        if (current->is_free && current->next->is_free)
        {
            current->size += current->next->size + BLOCK_SIZE;
            current->next = current->next->next;
        }
        else
        {
            current = current->next;
        }
    }
}

void my_free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }

    Block *block_to_free = (Block *)((char *)ptr - BLOCK_SIZE);
    block_to_free->is_free = true;

    merge_blocks();
}

void print_memory_blocks()
{
    Block *current = free_list;
    printf("Memory blocks:\n");
    while (current != NULL)
    {
        printf("Block at %p, size: %zu, is_free: %d\n", (void *)current, current->size, current->is_free);
        current = current->next;
    }
}

int main()
{
    init_pool();

    int *p1 = my_malloc(100);
    char *p2 = my_malloc(200);
    void *p3 = my_malloc(400);
    print_memory_blocks();

    my_free(p1);
    my_free(p2);
    print_memory_blocks();

    return 0;
}
