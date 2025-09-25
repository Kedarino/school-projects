/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    // Základní případ: Aktuální uzel je NULL, klíč nenalezen.
    if (tree == NULL) {
        return false;
    }

    if (key < tree->key) {
        // Klíč je menší než klíč aktuálního uzlu; hledání probíhá v levém podstromu.
        return bst_search(tree->left, key, value);
    } else if (key > tree->key) {
        // Klíč je větší než klíč aktuálního uzlu; hledání probíhá v pravém podstromu.
        return bst_search(tree->right, key, value);
    } else {
        // Klíč je roven klíči aktuálního uzlu; klíč byl nalezen.
        *value = tree->value;
        return true;
    }
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
   if (*tree == NULL) {
        // Alokuje prostor pro nový uzel
        *tree = malloc(sizeof(bst_node_t));
        if (*tree != NULL) { // Vždy zkontrolujte, zda byla alokace úspěšná
            (*tree)->key = key;
            (*tree)->value = value;
            (*tree)->left = NULL;
            (*tree)->right = NULL;
        }
    } else if (key < (*tree)->key) {
        // Klíč je menší než klíč aktuálního uzlu; pokračuje se v levém podstromu
        bst_insert(&((*tree)->left), key, value);
    } else if (key > (*tree)->key) {
        // Klíč je větší než klíč aktuálního uzlu; pokračuje se v pravém podstromu
        bst_insert(&((*tree)->right), key, value);
    } else {
        // Klíč je roven klíči aktuálního uzlu; aktualizuje se hodnota uzlu
        (*tree)->value = value;
    }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  // Jelikož strom není NULL, kontroluje se, zda existuje pravý potomek
    if ((*tree)->right != NULL) {
        // Rekurzivní volání pro přesun k nejpravějšímu potomkovi
        bst_replace_by_rightmost(target, &((*tree)->right));
    } else {
        // Byl nalezen nejpravější uzel, nyní se nahradí klíč a hodnota cílového uzlu
        target->key = (*tree)->key;
        target->value = (*tree)->value;
        
        // Uloží se levý potomek nejpravějšího uzlu (pokud existuje)
        bst_node_t *levyPotomek = (*tree)->left;

        // Nyní se uvolní nejpravější uzel
        free(*tree);

        // Rodič nejpravějšího uzlu by nyní měl ukazovat na levého potomka
        *tree = levyPotomek;
    }
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    // Kontrola, zda strom existuje.
    if (*tree == NULL) {
        return;
    }

    // Hledání klíče v levém podstromu.
    if (key < (*tree)->key) {
        bst_delete(&((*tree)->left), key);
    }
    // Hledání klíče v pravém podstromu.
    else if (key > (*tree)->key) {
        bst_delete(&((*tree)->right), key);
    }
    // Klíč byl nalezen.
    else {
        // Uzel má nejvýše jednoho potomka.
        if ((*tree)->left == NULL || (*tree)->right == NULL) {
            bst_node_t *newChild = ((*tree)->left != NULL) ? (*tree)->left : (*tree)->right;
            free(*tree);
            *tree = newChild;
        } 
        // Uzel má oba potomky.
        else {
            // Náhrada uzlu pravým nejvzdálenějším uzlem levého podstromu.
            bst_replace_by_rightmost(*tree, &((*tree)->left));
        }
    }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    // Pokud je uzel NULL, není co uvolňovat.
    if (*tree == NULL) {
        return;
    }

    // Uvolnění levého podstromu pomocí rekurze.
    bst_node_t *leftSubtree = (*tree)->left;
    if (leftSubtree != NULL) {
        bst_dispose(&leftSubtree);
    }

    // Uvolnění pravého podstromu.
    bst_node_t *rightSubtree = (*tree)->right;
    if (rightSubtree != NULL) {
        bst_dispose(&rightSubtree);
    }

    // Uvolnění aktuálního uzlu a nastavení na NULL.
    free(*tree);
    *tree = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    // Pokud je uzel prázdný, nedělá nic.
    if (tree == NULL) {
        return;
    }

    // Přidání uzlu do seznamu prvků.
    bst_add_node_to_items(tree, items);

    // Rekurzivní průchod levým podstromem.
    bst_preorder(tree->left, items);
    

    // Rekurzivní průchod pravým podstromem.
    bst_preorder(tree->right, items);
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    // Pokud je uzel prázdný, nedělá nic.
    if (tree == NULL) {
        return;
    }

    // Rekurzivní průchod levým podstromem.
    bst_inorder(tree->left, items);

    // Přidání aktuálního uzlu do seznamu položek.
    bst_add_node_to_items(tree, items);

    // Rekurzivní průchod pravým podstromem.
    bst_inorder(tree->right, items);
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    // Pokud je uzel prázdný, nedělá nic.
    if (tree == NULL) {
        return;
    }

    // Rekurzivní průchod levým podstromem.
    bst_postorder(tree->left, items);

    // Rekurzivní průchod pravým podstromem.
    bst_postorder(tree->right, items);

    // Přidání aktuálního uzlu do seznamu položek.
    bst_add_node_to_items(tree, items);
}
