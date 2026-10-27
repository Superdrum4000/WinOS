# WinOS — Architecture

## Démarrage

```text
BIOS ──> boot.asm (0x7C00, 16 bits)
          │  lit le kernel sur le disque (LBA, int 0x13 AH=0x42) vers 0x8000
          │  active A20, charge la GDT, passe en mode protégé
          ▼
         entry.asm (_start, 0x8000, 32 bits)
          │  met la section .bss à zéro
          ▼
         kernel_main()  (kernel/core/kernel.c)
          │  initialise le VGA
          ▼
         shell_run()    (kernel/shell/shell.c) — boucle infinie
```

## Carte mémoire

| Adresse          | Contenu                                   |
|------------------|-------------------------------------------|
| `0x7C00`         | Bootloader (512 octets)                   |
| `0x8000`         | Kernel (`.text`, `.rodata`, `.data`, `.bss`) |
| `0x90000`        | Haut de la pile du kernel (descend)       |
| `0xB8000`        | Mémoire vidéo du mode texte VGA           |

Le kernel peut grandir jusqu'à environ 500 Ko avant de toucher la pile.

## Image disque

```text
secteur 0        : boot.bin (se termine par 0xAA55)
secteurs 1 → N   : kernel.bin complété avec des zéros
```

Le Makefile calcule N automatiquement et le passe à NASM
(`-dKERNEL_SECTORS=N`).

## Arborescence

```text
boot/boot.asm                 bootloader
linker/linker.ld              place _start en premier, à 0x8000
kernel/
├── entry.asm                 point d'entrée 32 bits
├── core/kernel.c|h           kernel_main, nom et version
├── io/io.h                   inb() / outb()
├── lib/string.c|h            strlen, strcmp, memset, memcpy...
├── shell/shell.c|h           terminal et commandes
└── Drivers/
    ├── vga/vga.c|h           affichage texte, couleurs, scroll, curseur
    └── keyboard/keyboard.c|h clavier PS/2, AZERTY belge
```

## Ajouter une commande au shell

Dans `kernel/shell/shell.c` :

1. écrire une fonction `static void cmd_xxx(const char *args)` ;
2. ajouter une ligne `{ "xxx", "description", cmd_xxx }` dans le tableau `commands`.
