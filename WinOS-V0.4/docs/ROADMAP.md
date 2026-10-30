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
* [x] Lecture LBA étendue, secteur par secteur (V0.4)
* [x] Activation de la ligne A20 (V0.4)
* [x] Point d'entrée `entry.asm` + mise à zéro de `.bss` (V0.4)

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
* [x] Scrolling (24 lignes)
* [x] Couleurs (`set_color`), `clear_screen`, `print_at`, `print_dec` (V0.4)
* [ ] Curseur clignotant (nécessite le timer)

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
* [x] AZERTY belge complet : chiffres, ponctuation, pavé numérique (V0.4)
* [x] Shift, Caps Lock, AltGr (V0.4)
* [x] Accents via la code page 437 (V0.4)
* [ ] Clavier par interruption (IRQ1)
* [ ] Flèches et historique

## 🔌 Entrées/sorties bas niveau

* [x] Fonction `outb()`
* [x] Fonction `inb()`
* [x] Communication avec les ports I/O x86

## 💻 Shell

* [x] Buffer de commande
* [x] Découpage commande / arguments
* [x] `help`, `clear`, `echo`, `version`, `reboot` (V0.4)
* [x] `mem` et `memtest` (V0.5)
* [ ] `uptime` (nécessite le timer)

## 🧮 Mémoire (V0.5)

* [x] Tas de 3 Mo, de `0x100000` à `0x400000`
* [x] Étiquette de bloc (`struct block_header`) avec nombre magique
* [x] Liste chaînée de blocs
* [x] `heap_init()`
* [x] `kmalloc()` : arrondi à 8, *first fit*, découpage des blocs
* [x] `kfree()` : vérification de l'adresse et détection du double free
* [x] Recollage des blocs libres voisins
* [x] `heap_stats()` + commandes `mem` et `memtest`
* [ ] Carte de la mémoire fournie par le BIOS (E820)

## 📁 Système de fichiers (prochaine étape)

* [ ] Règles et types de fichiers
* [ ] Fichiers en RAM, avec `kmalloc`
* [ ] Commandes : lister, créer, lire, écrire, supprimer
* [ ] Sauvegarde sur le disque
* [ ] Dossiers

## ⚡ Interruptions

* [ ] IDT
* [ ] Exceptions CPU
* [ ] Remappage du PIC
* [ ] PIT / timer

## 🛠️ Build system

* [x] Makefile
* [x] Compilation automatique du kernel
* [x] Assemblage automatique du bootloader
* [x] Création automatique de `winos.img`
* [x] Lancement de WinOS avec QEMU
* [x] Détection automatique des fichiers `.c` (V0.4)
* [x] `make run` / `make clean` + `.gitignore` (V0.4)

## 📁 Organisation actuelle

Voir [architecture.md](architecture.md).

## 🏁 État actuel

WinOS démarre, passe en mode protégé 32 bits, et propose un petit terminal en
couleur avec un clavier AZERTY belge complet. Depuis la V0.5, il possède aussi
son propre allocateur de mémoire (`kmalloc` / `kfree`), qui servira de base au
futur système de fichiers.
