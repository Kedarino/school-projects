/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {

    // Inicializace všech položek hash tabulky na NULL
    for(int i = 0; i < HT_SIZE; i++) {
        (*table)[i] = NULL;
    }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {

    // Hledání prvku s daným klíčem v hash tabulce
    int index = get_hash(key); // Získání indexu pro klíč
    ht_item_t *current = (*table)[index]; // Nastavení aktuálního prvku

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current; // Nalezený prvek je vrácen
        }
        current = current->next; // Posun na další prvek v seznamu
    }

    return NULL; // Prvek nenalezen
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    
    // Vkládání nebo aktualizace prvku v hash tabulce
    int hash_index = get_hash(key); // Výpočet indexu hash
    ht_item_t *existing_item = ht_search(table, key); // Hledání existujícího prvku

    if (existing_item == NULL) {
        // Vytvoření nového prvku, pokud klíč neexistuje
        ht_item_t *new_element = (ht_item_t *)malloc(sizeof(ht_item_t));
        if (!new_element) {
            exit(1); // Přerušení při selhání alokace paměti
        }

        new_element->key = strdup(key); // Duplikace klíče
        new_element->value = value; // Nastavení hodnoty
        new_element->next = (*table)[hash_index]; // Vložení prvku na začátek seznamu
        (*table)[hash_index] = new_element;
    } else {
        // Aktualizace existujícího prvku
        existing_item->value = value;
    }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    // Získání hodnoty pro daný klíč v hash tabulce
    ht_item_t *found_item = ht_search(table, key); // Vyhledání prvku

    if (found_item != NULL) {
        return &(found_item->value); // Vracení ukazatele na hodnotu, pokud byl prvek nalezen
    }

    return NULL; // Vracení NULL, pokud prvek nebyl nalezen
}
/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    int hash_index = get_hash(key);  // Výpočet indexu pro daný klíč
    ht_item_t *previous = NULL;      // Ukazatel na předchozí prvek
    ht_item_t *current = (*table)[hash_index];  // Aktuálně prohledávaný prvek

    while (current != NULL) { 
        if (strcmp(current->key, key) == 0) {
            // Pokud je nalezen shodný klíč, odstraňte prvek z tabulky
            if (previous == NULL) {
                // Prvek je první v seznamu
                (*table)[hash_index] = current->next;
            } else {
                // Prvek je uprostřed nebo na konci seznamu
                previous->next = current->next;
            }
            free(current->key);  // Uvolnění paměti pro klíč
            free(current);       // Uvolnění paměti pro prvek
            return;
        }
        // Posun na další prvek v seznamu
        previous = current;
        current = current->next;
    }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
    // Projděte všechny sloty v hash tabulce
    for (int index = 0; index < HT_SIZE; index++) {
        ht_item_t *current_item = (*table)[index]; // Aktuální prvek v seznamu

        // Odstraňování všech prvků v aktuálním slotu
        while (current_item != NULL) {
            ht_item_t *next = current_item->next; // Uchování odkazu na další prvek
            free(current_item->key); // Uvolnění paměti pro klíč
            free(current_item);      // Uvolnění paměti pro prvek
            current_item = next;     // Posun na další prvek v seznamu
        }

        (*table)[index] = NULL; // Nastavení slotu na NULL po odstranění všech prvků
    }
}
