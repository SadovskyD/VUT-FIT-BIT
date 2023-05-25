/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
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
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
	
	if (table == NULL) return;
	
	for (int i = 0; i < HT_SIZE; i++)
	{
		(*table)[i] = NULL;
	}
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
	
	ht_item_t *item = (*table)[get_hash(key)];
	
	while(item != NULL)
	{
		if (strcmp(key, item->key) == 0)
		{
			return item;
		}
		item = item->next;
	}
	return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
	
	ht_item_t **new, *pred = NULL;

    if (table == NULL || key == NULL) return;
	
	pred = (ht_search(table, key));
    
	if (pred != NULL) pred->value = value;
    else 
	{
		new = &(*table)[get_hash(key)];
        if (*new != NULL) pred = *new;
        (*new) = malloc(sizeof(ht_item_t));
        (*new)->key = key;
        (*new)->value = value;
        (*new)->next = pred;
    }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
	
	if (table == NULL || key == NULL) return NULL;
	
	ht_item_t *search = ht_search(table, key);
	
	if(search != NULL) return &ht_search(table, key)->value;
	
	return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
	
	int hash = get_hash(key);
	ht_item_t *prev_item = NULL, *next_item = NULL;
	ht_item_t *item = (*table)[hash];
	
	if(item == NULL) return;
	

	while(item != NULL)
	{
		next_item = item->next;

		if (strcmp(key, item->key) == 0)
		{
			free(item);

			if (!prev_item)
			{
				(*table)[hash] = next_item;
				return;
			}

			prev_item->next = next_item;
			return;
		}
		prev_item = item;
		item = item->next;
	}
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
	ht_item_t *item, *item_to_delete;

	for (int i = 0; i < HT_SIZE; i++)
	{
		item = (*table)[i];
		while (item)
		{
			item_to_delete = item;
			item = item->next;
			free(item_to_delete);
		}
		(*table)[i] = NULL;
	}
}
