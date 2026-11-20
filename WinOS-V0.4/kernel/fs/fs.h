#ifndef FS_H
#define FS_H

#define FS_NAME_MAX 32          // longueur max d'un nom (avec le '\0')

struct file
{
    char name[FS_NAME_MAX];     // nom du fichier
    char *data;                 // contenu (dans le tas, via kmalloc)
    unsigned int size;          // taille du contenu en octets
    struct file *next;          // fichier suivant (0 = c'est le dernier)
};

struct file *fs_find(const char *name);
int fs_create(const char *name);
struct file *fs_first(void);
int fs_write(const char *name, const char *text);
int fs_delete(const char *name);

#endif