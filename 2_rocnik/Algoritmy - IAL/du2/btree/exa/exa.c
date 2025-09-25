/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    // Inicializace stromu
    bst_init(tree);

    // Pole pro ukládání frekvence znaků
    int char_frequencies[128] = {0}; // Předpokládá se použití ASCII znakové sady

    // Procházení každého znaku ve vstupním řetězci
    for (char *current = input; *current; current++) {
        char processed_char = *current;

        // Převod velkých písmen na malá
        if (processed_char >= 'A' && processed_char <= 'Z') {
            processed_char = processed_char - 'A' + 'a';
        }

        // Seskupení všech nealfanumerických a ne-mezerových znaků pod klíč '_'
        if ((processed_char < 'a' || processed_char > 'z') && processed_char != ' ') {
            processed_char = '_';
        }

        // Zvýšení frekvence pro aktuální znak
        char_frequencies[(int)processed_char]++;
    }

    // Vložení frekvencí do stromu
    for (int i = 0; i < 128; i++) {
        if (char_frequencies[i] > 0) {
            bst_insert(tree, (char)i, char_frequencies[i]);
        }
    }
}
// Funkce pro zjištění celkového počtu uzlů v binárním vyhledávacím stromu.
int bst_node_count(bst_node_t *root) {
    // Pokud je kořenový uzel NULL, vrátí 0, protože neexistují žádné uzly.
    if (root == NULL) {
        return 0;
    } else {
        // V opačném případě rekurzivně spočítá uzly včetně aktuálního uzlu a jeho podstromů.
        return 1 + bst_node_count(root->left) + bst_node_count(root->right);
    }
}

// Funkce pro převod binárního vyhledávacího stromu do pole. Tato funkce ukládá uzly do pole v inorder pořadí.
void bst_to_array(bst_node_t *root, bst_node_t **array, int *index) {
    // Pokud je kořenový uzel NULL, funkce se ukončí.
    if (root == NULL) {
        return;
    } else {
        // Prochází levý podstrom a přidává uzly do pole.
        bst_to_array(root->left, array, index);

        // Přidává aktuální uzel do pole.
        array[(*index)++] = root;

        // Prochází pravý podstrom a přidává uzly do pole.
        bst_to_array(root->right, array, index);
    }
}

// Funkce pro vytvoření binárního vyhledávacího stromu (BST) z pole uzlů.
bst_node_t *array_to_bst(bst_node_t **array, int start, int end) {
    // Kontrola, zda je počáteční index větší než koncový, což naznačuje prázdný podstrom.
    if (start > end) {
        return NULL;
    } else {
        // Výpočet středního indexu a vytvoření kořenového uzlu stromu z tohoto indexu.
        int mid = (start + end) / 2;
        bst_node_t *root = array[mid];

        // Rekurzivní vytvoření levého a pravého podstromu stromu.
        root->left = array_to_bst(array, start, mid - 1);
        root->right = array_to_bst(array, mid + 1, end);

        // Vrácení nově vytvořeného kořenového uzlu.
        return root;
    }
}


/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
    // Zjištění počtu uzlů ve stromu
    int total_nodes = bst_node_count(*tree);
    if (total_nodes == 0) {
        // Pokud je strom prázdný, není potřeba vyvažování
        return;
    }

    // Alokace paměti pro pole ukazatelů na uzly
    bst_node_t **node_array = malloc(sizeof(bst_node_t*) * total_nodes);
    if (node_array == NULL) {
        // Ošetření selhání alokace paměti
        return;
    }
    int current_index = 0;

    // Převod BST na setříděné pole
    bst_to_array(*tree, node_array, &current_index);

    // Vytvoření vyváženého BST ze setříděného pole
    *tree = array_to_bst(node_array, 0, total_nodes - 1);

    // Uvolnění pole, individuální uzly se neuvolňují, protože jsou znovu použity
    free(node_array); // Uvolnění pole použitého pro vytvoření vyváženého BST
}
