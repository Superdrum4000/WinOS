# WinOS — Development Log

## Version 0.5

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

## Version 0.4

- Buffer de commande, quelques caractères spéciaux.
- Scrolling VGA.

## Prochaine étape

1. IDT (table des interruptions) + gestion des exceptions CPU.
2. Remappage du PIC.
3. Clavier par interruption (IRQ1) au lieu du polling.
4. PIT/timer (IRQ0) : compteur de temps, commande `uptime`.
5. Faire clignoter le curseur avec le timer.
6. Flèches gauche/droite et historique des commandes.
