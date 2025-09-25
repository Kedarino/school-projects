/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
  if (tree) {
        *tree = NULL;
    }
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
   // Průshod stromu, dokud nenarazí na NULL ukazatel
    while (tree != NULL) {
        if (key < tree->key) {
            // Pokud je hledaný klíč menší než klíč v aktuálním uzlu, jde doleva
            tree = tree->left;
        } else if (key > tree->key) {
            // Pokud je hledaný klíč větší než klíč v aktuálním uzlu, jde doprava
            tree = tree->right;
        } else {
            // Pokud nalezne shodu, aktualizuje hodnotu a vratí true
            *value = tree->value;
            return true;
        }
    }
    // Pokud dojde k NULL ukazateli, klíč ve stromu není
    return false;
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  /// Vytvoří nový uzel
    bst_node_t *new_node = malloc(sizeof(bst_node_t));
    if (new_node == NULL) {
        // Zpracuje selhání alokace paměti, pokud je to nutné
        return;
    }
    new_node->key = key;
    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;

    // Pokud je strom prázdný, vloží nový uzel jako kořen
    if (*tree == NULL) {
        *tree = new_node;
        return;
    }

    // Jinak najde správné místo pro vložení nového uzlu
    bst_node_t *current = *tree;
    bst_node_t *parent = NULL;
    while (current != NULL) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            // Pokud klíč již existuje, aktualizuje hodnotu a uvolní nový uzel
            current->value = value;
            free(new_node);
            return;
        }
    }

    // Připojí nový uzel k příslušnému rodičovskému uzlu
    if (key < parent->key) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if (tree == NULL || *tree == NULL || target == NULL) {
        return; // Řešení situací, kdy jsou vstupní parametry NULL
    }

    bst_node_t *parent_node = NULL;
    bst_node_t *rightmost_node = *tree;

    // Najde nejpravější uzel
    while (rightmost_node->right != NULL) {
        parent_node = rightmost_node;
        rightmost_node = rightmost_node->right;
    }

    // Nahradí údaje cílového uzlu údaji nejpravějšího uzlu
    target->key = rightmost_node->key;
    target->value = rightmost_node->value;

    // Odstranění nejpravějšího uzlu
    if (parent_node != NULL) {
        // Pokud má nejpravější uzel levého potomka, připojí jej k rodiči
        parent_node->right = rightmost_node->left;
    } else {
        // Pokud rodič neexistuje, nejpravější uzel je kořenem podstromu
        *tree = rightmost_node->left;
    }

    // Uvolnění paměťi nejpravějšího uzlu
    free(rightmost_node);
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (tree == NULL || *tree == NULL) {
        return; // Strom je prázdný nebo klíč neexistuje
    }

    bst_node_t *node_to_delete = *tree;
    bst_node_t *parent_node = NULL;

    // Najít uzel k odstranění a jeho rodiče
    while (node_to_delete != NULL) {
        if (key < node_to_delete->key) {
            parent_node = node_to_delete;
            node_to_delete = node_to_delete->left;
        } else if (key > node_to_delete->key) {
            parent_node = node_to_delete;
            node_to_delete = node_to_delete->right;
        } else {
            // Uzel nalezen, ukončit loop
            break;
        }
    }

    // Pokud uzel nebyl nalezen, node_to_delete bude NULL
    if (node_to_delete == NULL) {
        return; // Klíč nebyl nalezen, není co odstranit
    }

    // Případ 1: Uzel ke smazání je list (bez dětí)
    if (node_to_delete->left == NULL && node_to_delete->right == NULL) {
        if (parent_node == NULL) {
            *tree = NULL; // Uzel ke smazání je kořen
        } else if (parent_node->left == node_to_delete) {
            parent_node->left = NULL;
        } else {
            parent_node->right = NULL;
        }
        free(node_to_delete);
    }
    // Případ 2: Uzel ke smazání má pouze jedno dítě
    else if (node_to_delete->left == NULL || node_to_delete->right == NULL) {
        bst_node_t *child_node = (node_to_delete->left != NULL) ? node_to_delete->left : node_to_delete->right;
        if (parent_node == NULL) {
            *tree = child_node; // Uzel ke smazání je kořen
        } else if (parent_node->left == node_to_delete) {
            parent_node->left = child_node;
        } else {
            parent_node->right = child_node;
        }
        free(node_to_delete);
    }
    // Případ 3: Uzel ke smazání má dvě děti
    else {
        bst_replace_by_rightmost(node_to_delete, &(node_to_delete->left));

    }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    // Kontrola, zda už strom není prázdný
    if (tree == NULL || *tree == NULL) {
        return;
    }

    // Inicializace zásobníku pro uzly
    stack_bst_t stack;
    stack_bst_init(&stack);

    // Začněte s kořenovým uzlem
    bst_node_t *current_node = *tree;
    
    while (current_node != NULL || !stack_bst_empty(&stack)) {
        // Dosažení nejlevějšího uzlu aktuálního uzlu
        while (current_node != NULL) {
            // Pokud má tento uzel pravého potomka, vložte ho na zásobník
            if (current_node->right != NULL) {
                stack_bst_push(&stack, current_node->right);
            }
            stack_bst_push(&stack, current_node);
            current_node = current_node->left;
        }

        // V tomto bodě musí být current_node NULL
        current_node = stack_bst_top(&stack);
        stack_bst_pop(&stack);

        // Pokud je pravý potomek na vrcholu dalšího zásobníku, zpracujte tohoto rodiče později
        if (current_node->right != NULL && !stack_bst_empty(&stack) && current_node->right == stack_bst_top(&stack)) {
            stack_bst_pop(&stack); // Odstraňte pravého potomka
            stack_bst_push(&stack, current_node); // Vraťte rodičovský uzel zpět
            current_node = current_node->right; // Zpracujte pravého potomka
        } else {
            // Pokud je pravý potomek NULL nebo už byl zpracován, můžeme zpracovat rodiče
            free(current_node);
            current_node = NULL; // Nastavte current_node na NULL pro pokračování s uzly ze zásobníku
        }
    }

    *tree = NULL; // Nastavte strom na NULL po odstranění všech uzlů
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
    // Pokračujte v průchodu, dokud máme platný uzel
    while (tree != NULL) {
        // Zpracujte aktuální uzel
        bst_add_node_to_items(tree, items);

        // Pokud existuje pravý potomek, vložte ho na zásobník pro pozdější zpracování
        if (tree->right != NULL) {
            stack_bst_push(to_visit, tree->right);
        }

        // Přesuňte se k levému potomkovi, který bude zpracován jako další
        tree = tree->left;
    }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    // Inicializujte zásobník pro sledování uzlů k návštěvě
    stack_bst_t nodes_to_visit;
    stack_bst_init(&nodes_to_visit);

    // Zahajte průchod nejlevějšími uzly
    bst_leftmost_preorder(tree, &nodes_to_visit, items);

    // Pokračujte se zbytkem uzlů ze zásobníku
    while (!stack_bst_empty(&nodes_to_visit)) {
        // Vyjměte další uzel ze zásobníku
        tree = stack_bst_top(&nodes_to_visit);
        stack_bst_pop(&nodes_to_visit);

        // Navštivte nejlevější uzly od aktuálního uzlu
        bst_leftmost_preorder(tree, &nodes_to_visit, items);
    }
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    // Projde na nejlevější uzel a cestou na zásobník vloží všechny uzly
    while (tree != NULL) {
        stack_bst_push(to_visit, tree); // Vložte aktuální uzel na zásobník
        tree = tree->left; // Přesuňte se k levému potomkovi
    }
    // Po ukončení smyčky jsou na zásobníku všechny levé uzly v obráceném pořadí
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    // Inicializujte zásobník pro sledování uzlů k návštěvě
    stack_bst_t to_visit;
    stack_bst_init(&to_visit);

    // Přejděte na nejlevější uzel
    bst_leftmost_inorder(tree, &to_visit);

    // Procházejte stromem
    while (!stack_bst_empty(&to_visit)) {
        // Navštivte uzel na vrcholu zásobníku
        tree = stack_bst_top(&to_visit);
        stack_bst_pop(&to_visit);

        // Přidejte hodnotu uzlu do items
        bst_add_node_to_items(tree, items);

        // Pokud existuje pravý potomek, následujte jeho nejlevější cestu
        if (tree->right != NULL) {
            bst_leftmost_inorder(tree->right, &to_visit);
        }
    }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
    while (tree != NULL) {
        // Uzel se vloží na zásobník uzlů k návštěvě
        stack_bst_push(to_visit, tree);
        // Na zásobník bool hodnot se vloží 'true', což indikuje první návštěvu
        stack_bool_push(first_visit, true);
        // Přesune se k levému potomkovi
        tree = tree->left;
    }
    // Po ukončení smyčky jsou na zásobníku všechny nejlevější uzly až k listu
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    // Inicializují se dva zásobníky, jeden pro uzly a druhý pro příznaky návštěv
    stack_bst_t to_visit;
    stack_bool_t first_visit;

    stack_bst_init(&to_visit);
    stack_bool_init(&first_visit);

    bst_node_t *posledni_navstiveny_uzel = NULL;

    while (tree != NULL || !stack_bst_empty(&to_visit)) {
        if (tree != NULL) {
            bst_leftmost_postorder(tree, &to_visit, &first_visit);
        }

        tree = stack_bst_top(&to_visit);
        bool navstiveno_poprve = stack_bool_top(&first_visit);

        // Pokud je uzel navštíven podruhé, nebo nemá pravý podstrom, je čas uzel zpracovat
        if (!navstiveno_poprve || tree->right == NULL || tree->right == posledni_navstiveny_uzel) {
            stack_bst_pop(&to_visit);
            stack_bool_pop(&first_visit);
            bst_add_node_to_items(tree, items); // Zpracování uzlu
            posledni_navstiveny_uzel = tree;
            tree = NULL; // Nastavení tree na NULL pro pokračování s uzly ze zásobníku
        } else {
            // Uzel je navštíven poprvé a má pravý podstrom.
            // Aktualizuje se příznak na false a pokračuje se k pravému podstromu.
            stack_bool_pop(&first_visit); // Odstraní se aktuální hodnota true
            stack_bool_push(&first_visit, false); // Vloží se false, což indikuje, že uzel byl navštíven
            tree = tree->right; // Přesun k pravému podstromu
        }
    }
}
