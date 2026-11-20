#include "fs.h"
#include "string.h"
#include "memory/heap.h"

static struct file *first_file = 0;     // début de la liste (0 = aucun fichier)

struct file *fs_first(void)
{
    return first_file;
}

// chercher un fichier par son nom (0 = pas trouvé)
struct file *fs_find(const char *name)
{
    struct file *f = first_file;
    while (f != 0)
    {
        if (strcmp(f->name, name) == 0)
            return f;
        f = f->next;
    }
    return 0;
}

// 0 = ok, 1 = nom invalide, 2 = existe déjà, 3 = plus de mémoire
int fs_create(const char *name)
{
    unsigned int length = strlen(name);
    if (length == 0 || length >= FS_NAME_MAX) return 1;
    if (fs_find(name) != 0)                   return 2;

    struct file *f = kmalloc(sizeof(struct file));  // la fiche va dans le tas
    if (f == 0) return 3;

    memcpy(f->name, name, length + 1);  // +1 pour copier le '\0'
    f->data = 0;                        // fichier vide
    f->size = 0;

    f->next = first_file;               // on l'accroche au début de la liste
    first_file = f;
    return 0;
}

// remplacer le contenu d'un fichier par "text"
// 0 = ok, 1 = fichier introuvable, 3 = plus de mémoire
int fs_write(const char *name, const char *text)
{
    struct file *f = fs_find(name);
    if (f == 0) return 1;

    unsigned int length = strlen(text);
    char *new_data = 0;

    // 1. réserver la place pour le nouveau contenu (rien à réserver si vide)
    if (length > 0)
    {
        new_data = kmalloc(length);
        if (new_data == 0) return 3;    // l'ancien contenu est toujours là, intact
        memcpy(new_data, text, length);
    }

    // 2. rendre l'ancien contenu au tas (kfree(0) ne fait rien)
    kfree(f->data);

    // 3. la fiche pointe vers le nouveau contenu
    f->data = new_data;
    f->size = length;
    return 0;
}

// supprimer un fichier : 0 = ok, 1 = fichier introuvable
int fs_delete(const char *name)
{
    struct file *previous = 0;          // la fiche juste avant
    struct file *f = first_file;

    // 1. chercher la fiche en retenant celle d'avant
    while (f != 0 && strcmp(f->name, name) != 0)
    {
        previous = f;
        f = f->next;
    }
    if (f == 0) return 1;

    // 2. la décrocher de la liste : celle d'avant saute par-dessus
    if (previous == 0) first_file = f->next;    // c'était la première
    else               previous->next = f->next;

    // 3. rendre au tas le contenu, puis la fiche elle-même
    kfree(f->data);
    kfree(f);
    return 0;
}