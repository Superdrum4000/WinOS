# WinOS — Development Log

## Version 0.5 (en cours) : allocateur de mémoire

### Mémoire (heap)
- Nouveau module `kernel/memory/heap.c` et `heap.h`.
- Tas (heap) de 3 Mo, de `0x100000` (1 Mo) à `0x400000` (4 Mo).
- Chaque bloc commence par une étiquette de 16 octets (`struct block_header`) :
  taille, libre/occupé, adresse du bloc suivant, nombre magique `0xC0FFEE`.
- Les étiquettes forment une liste chaînée qu'on parcourt du début à la fin.
- `heap_init()` : crée un seul grand bloc libre au démarrage.
- `kmalloc(taille)` : arrondit au multiple de 8, prend le premier bloc libre
  assez grand (*first fit*) et le découpe s'il est trop grand.
- `kfree(adresse)` : vérifie l'adresse (hors du tas, mauvais nombre magique),
  détecte les doubles `kfree`, marque le bloc libre puis recolle les blocs
  libres voisins (`merge_free_blocks()`).
- `heap_stats()` : compte la mémoire utilisée, libre et le nombre de blocs.

### Shell
- Commande `mem` : affiche l'état du tas.
- Commande `memtest` : enchaîne des `kmalloc` / `kfree` et affiche l'état du tas
  après chaque étape (découpage, recollage, double free, mauvaise adresse).
- Tous les tests donnent le résultat attendu dans QEMU.

## Version 0.4

### Boot
- Lecture du kernel en LBA étendu (int 0x13, AH=0x42), secteur par secteur :
  plus de limite à ~62 secteurs.
- Registres de segment et pile initialisés proprement au démarrage.
- Activation de la ligne A20.
- Nouveau point d'entrée `kernel/entry.asm` (`_start`) : toujours à 0x8000,
  quel que soit l'ordre des fichiers au linkage.
- La section `.bss` est mise à zéro avant `kernel_main`.
- `linker.ld` : sections `.rodata` et `.bss` déclarées, symboles `__bss_start` / `__bss_end`.

### VGA
- Constantes pour la taille de l'écran et les couleurs.
- `vga_init()`, `clear_screen()`, `set_color()`.
- `print_at()` implémentée, `print_dec()` ajoutée.
- `put_char()` gère `\n`.
- `backspace()` remonte à la ligne précédente si besoin.
- Une seule couleur de fond partout.
- 24 lignes utilisées (choix volontaire, plus lisible).

### Clavier
- AZERTY belge complet : chiffres (avec Shift), ponctuation, pavé numérique.
- Shift, Caps Lock et AltGr (`@`, `#`, `|`, `{` `}`, `[` `]`, `\`, `~`, `^`, accent grave).
- Accents (é è à ç ù µ § ² °) affichés grâce à la table de caractères VGA (code page 437).
- Les touches inconnues n'affichent plus `?`.
- `keyboard_getchar()` : attend une touche qui produit un caractère.

### Shell
- Nouveau module `kernel/shell/`.
- Découpage commande / arguments.
- Commandes : `help`, `clear`, `echo`, `version`, `reboot`.
- Message en rouge pour une commande inconnue, prompt en jaune.

### Divers
- Petite bibliothèque `kernel/lib/string` (strlen, strcmp, strncmp, memset, memcpy).
- Makefile automatique (`make`, `make run`, `make clean`).
- Fichiers compilés retirés du dépôt + `.gitignore`.
- Documentation : README, architecture.

## Avant la 0.4

- Buffer de commande, quelques caractères spéciaux.
- Scrolling VGA.

## Prochaine étape

1. Système de fichiers en RAM, qui utilise `kmalloc` / `kfree`.
2. Commandes pour lister, créer, lire, écrire et supprimer des fichiers.
3. Plus tard : sauvegarde des fichiers sur le disque, puis les dossiers.
4. Plus tard : interruptions (IDT, PIC, timer), curseur clignotant, `uptime`.
