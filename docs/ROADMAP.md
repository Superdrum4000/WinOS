# WinOS — Roadmap

## 🥾 Boot & démarrage

* [x] Boot sector BIOS
* [x] Chargement du kernel depuis le disque
* [x] Détection et conservation du disque de boot (`DL`)
* [x] Chargement automatique du nombre de secteurs du kernel
* [x] Passage du mode réel au mode protégé 32 bits
* [x] Mise en place de la GDT
* [x] Mise en place des segments code et données
* [x] Mise en place de la stack
* [x] Chargement du kernel à l'adresse `0x8000`
* [x] Saut du bootloader vers le kernel

## 🧠 Kernel

* [x] Premier kernel en C freestanding
* [x] Compilation avec le cross-compiler `i386-elf-gcc`
* [x] Édition de liens avec `i386-elf-ld`
* [x] Conversion du kernel ELF en binaire brut
* [x] Organisation du kernel en modules/drivers

## 🖥️ VGA

* [x] Utilisation du mode texte VGA `80×25`
* [x] Accès direct à la mémoire vidéo `0xB8000`
* [x] Fond bleu
* [x] Texte blanc
* [x] `put_char()`
* [x] `put_char_at()`
* [x] `print()`
* [x] Affichage hexadécimal
* [x] Gestion de la position du curseur logiciel
* [x] Retour automatique à la ligne
* [x] Gestion de `Enter`
* [x] Gestion de `Backspace`
* [x] Curseur logiciel `_`
* [x] Curseur blanc sur fond bleu
* [x] Déplacement du curseur avec la saisie

## ⌨️ Clavier

* [x] Communication avec le contrôleur clavier PS/2
* [x] Lecture du port `0x60`
* [x] Lecture du statut via `0x64`
* [x] Lecture des scancodes
* [x] Gestion des make codes
* [x] Ignorance des break codes
* [x] Traduction des scancodes en caractères
* [x] Mapping AZERTY
* [x] Lettres
* [x] Espace
* [x] `Enter`
* [x] `Backspace`

## 🔌 Entrées/sorties bas niveau

* [x] Fonction `outb()`
* [x] Fonction `inb()`
* [x] Communication avec les ports I/O x86

## 🛠️ Build system

* [x] Makefile
* [x] Compilation automatique du kernel
* [x] Assemblage automatique du bootloader
* [x] Création automatique de `winos.img`
* [x] Lancement de WinOS avec QEMU

## 📁 Organisation actuelle

```text
kernel/
├── core/
│   └── kernel.c
├── io/
│   └── io.h
└── Drivers/
    ├── vga/
    │   ├── vga.c
    │   └── vga.h
    └── keyboard/
        ├── keyboard.c
        └── keyboard.h
```

## 🏁 État actuel

WinOS peut démarrer depuis son image disque, passer en mode protégé
32 bits, charger son kernel C, initialiser l'affichage VGA et accepter
une saisie clavier AZERTY avec `Enter` et `Backspace`.

Le système possède maintenant les bases nécessaires pour évoluer vers
un véritable terminal.
