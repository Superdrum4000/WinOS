# WinOS

Petit système d'exploitation 32 bits pour x86, écrit en C et en assembleur,
pour apprendre comment fonctionne un ordinateur au plus bas niveau.

Version actuelle : **0.5**

## Ce que WinOS sait faire

- Démarrer depuis une image disque (bootloader BIOS maison).
- Passer en mode protégé 32 bits et lancer un kernel en C.
- Afficher du texte en couleur (mode texte VGA 80×25, fond bleu).
- Lire le clavier PS/2 en AZERTY belge (Shift, Caps Lock, AltGr, accents).
- Fournir un petit terminal avec les commandes `help`, `clear`, `echo`,
  `version` et `reboot`.

## Compiler et lancer

Outils nécessaires : `nasm`, le cross-compiler `i386-elf-gcc`
(avec `i386-elf-ld` et `i386-elf-objcopy`) et `qemu-system-i386`.

```bash
make          # compile et crée build/winos.img
make run      # compile puis lance WinOS dans QEMU
make clean    # supprime build/
```

Sans cross-compiler, sur Linux 64 bits, on peut aussi utiliser le gcc du système :

```bash
make CC="gcc -m32 -fno-pie -fno-pic" LD="ld -m elf_i386" OBJCOPY=objcopy
```

## Organisation

Voir [architecture.md](architecture.md), [ROADMAP.md](ROADMAP.md) et [LOG.md](LOG.md).
