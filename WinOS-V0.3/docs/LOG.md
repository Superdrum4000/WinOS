# WinOS — Development Log

## État actuel

WinOS démarre en mode protégé 32 bits et charge un kernel C à
l'adresse 0x8000.

### Bootloader
- BIOS charge le bootloader à 0x7C00.
- Le bootloader sauvegarde le numéro du disque dans DL.
- Le kernel est chargé depuis le secteur 2.
- Le nombre de secteurs du kernel est calculé automatiquement.
- Passage en protected mode via la GDT.
- Kernel chargé à 0x8000.

### VGA
- Mode texte VGA 80x25.
- Fond bleu.
- Texte blanc.
- `put_char()`
- `print()`
- retour automatique à la ligne.
- `new_line()`
- `backspace()`
- scrolling : à faire.
- curseur logiciel `_` blanc sur bleu.
- curseur clignotant : à faire avec un timer.
- ancien curseur matériel : désactivé.

### Clavier
- Lecture du contrôleur PS/2 par polling.
- Mapping AZERTY.
- Make/break codes gérés.
- Lettres et espace fonctionnels.
- Enter fonctionnel.
- Backspace fonctionnel.

### Terminal
- Affichage des caractères : OK.
- Enter : OK.
- Backspace : OK.
- Buffer de commande : à faire.
- Validation d'une commande : à faire.
- Shell : à faire.

## Prochaine étape

1. Ajouter le scrolling VGA.
2. Ajouter le buffer de commande.
3. Faire fonctionner Enter comme validation.
4. Créer une première commande.
5. Ajouter le PIT/timer.
6. Faire clignoter le curseur.