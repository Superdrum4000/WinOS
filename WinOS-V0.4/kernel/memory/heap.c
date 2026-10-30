#include "heap.h"

struct block_header
{
    unsigned int size;              // taille des données (sans le post-it)
    unsigned int is_free;           // 1 = libre, 0 = occupé
    struct block_header *next;      // adresse du post-it suivant (0 = c'est le dernier)
    unsigned int magic;             // code secret : 0xC0FFEE
};

#define HEAP_MAGIC 0xC0FFEE

static struct block_header *first_block = 0;


void heap_init(void)
{
    // le premier post-it est posé au début du tas (1 Mo)
    first_block = (struct block_header *)HEAP_START;

    // tout le tas, moins la place du post-it lui-même
    first_block->size = (HEAP_END - HEAP_START) - sizeof(struct block_header);
    first_block->is_free = 1;           // libre
    first_block->next = 0;              // pas de post-it après
    first_block->magic = HEAP_MAGIC;    // le code secret
}

void *kmalloc(unsigned int size)
{
    // 1. une demande de 0 octet n'a pas de sens
    if (size == 0)
    {
        return 0;
    }

    // 2. arrondir au multiple de 8 (100 devient 104)
    size = (size + 7) & ~7u;

    // 3. chercher le premier post-it libre et assez grand
    struct block_header *block = first_block;

    while (block != 0)
    {
        if (block->is_free == 1 && block->size >= size)
        {
            // découper seulement si le reste peut devenir un vrai bloc
        if (block->size - size >= sizeof(struct block_header) + 8)
        {
            // 1. où poser le nouveau post-it : juste après les données gardées
            struct block_header *new_block =
                (struct block_header *)((unsigned char *)(block + 1) + size);

            // 2. remplir le nouveau post-it
            new_block->size    = block->size - size - sizeof(struct block_header);
            new_block->is_free = 1;
            new_block->next    = block->next;
            new_block->magic   = HEAP_MAGIC;

            // 3. raccourcir l'ancien post-it et l'accrocher au nouveau
            block->size = size;
            block->next = new_block;
        }
            block->is_free = 0;                 // on le donne : il devient occupé
            return (void *)(block + 1);         // l'adresse juste après le post-it
        }

        block = block->next;                    // sinon, post-it suivant
    }

    // 4. arrivé au bout sans rien trouver : plus de place
    return 0;
}

void heap_stats(unsigned int *used, unsigned int *free, unsigned int *blocks)
{
    // 1. vider les trois boîtes
    *used = 0;
    *free = 0;
    *blocks = 0;

    // 2. parcourir tous les post-its
    struct block_header *block = first_block;

    while (block != 0)
    {
        if (block->is_free == 1)        // ce post-it est libre ?
        {
            *free += block->size;
        }
        else
        {
            *used += block->size;
        }

        *blocks += 1;                   // un post-it de plus

        block = block->next;            // le doigt passe au suivant
    }
}


// recoller chaque bloc libre avec le suivant s'il est libre aussi
static void merge_free_blocks(void)
{
    struct block_header *block = first_block;

    // tant qu'il y a un post-it ET un post-it après lui
    while (block != 0 && block->next != 0)
    {
        // le post-it ET son suivant sont-ils libres tous les deux ?
        if (block->is_free == 1 && block->next->is_free == 1)
        {
            // 1. A grandit : sa taille + le post-it de B + la taille de B
            block->size = block->size + sizeof(struct block_header) + block->next->size;

            // 2. A s'accroche au wagon d'après B (B disparaît)
            block->next = block->next->next;

            // on ne bouge pas le doigt : le nouveau voisin est peut-être libre aussi
        }
        else
        {
            block = block->next;        // le doigt passe au suivant
        }
    }
}


int kfree(void *ptr)
{
    // 1. kfree(0) : rien à rendre, ce n'est pas une erreur
    if (ptr == 0)
    {
        return 0;
    }

    // 2. une adresse en dehors du tas ne peut pas venir de kmalloc
    if ((unsigned int)ptr < HEAP_START + sizeof(struct block_header)
        || (unsigned int)ptr >= HEAP_END)
    {
        return 1;                           // 1 = mauvaise adresse
    }

    // 3. reculer d'un post-it pour le retrouver
    struct block_header *block = (struct block_header *)ptr - 1;

    // 4. le code secret est-il faux ?
    if (block->magic != HEAP_MAGIC)
    {
        return 1;                           // 1 = mauvaise adresse
    }

    // 5. le bloc est-il déjà libre ?
    if (block->is_free == 1)
    {
        return 2;                           // 2 = double free
    }

    // 6. on le rend : il redevient libre
    block->is_free = 1;

    // 7. on recolle les bloques libres
    merge_free_blocks();

    return 0;                               // 0 = tout va bien
}
