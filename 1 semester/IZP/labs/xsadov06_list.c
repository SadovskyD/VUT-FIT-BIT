/**
 * Kostra pro cviceni operaci nad jednosmerne vazanym seznamem.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

/**
 * struktura Object
 */
typedef struct {
    int id;
    char *name;
} Object;

/**
 * Polozka seznamu
 */
typedef struct item Item;
struct item {
    Object data;
    Item *next;
    // Item *prev;
};

/**
 * Typ seznam
 */
typedef struct {
    Item *first;
    // Item *last;
    // unsigned length;
} List;

/**
 * Inicializace seznamu. Vytvori prazdny seznam.
 */
List list_ctor() {
    List lst;
    lst.first = NULL;
    return lst;
}

/**
 * Inicializace polozky seznamu. Z objektu vytvori polozku bez naslednika.
 * @note copy constructor
 */
Item *item_ctor(Object data) {
    Item *new_item=malloc(sizeof(Item));
    assert(new_item);
    new_item->data=data;
    new_item->next=NULL;
    return new_item;
}


/**
 * Vlozi polozku na zacatek seznamu.
 */
void list_insert_first(List *list, Item *i) {
    i->next=list->first;
    list->first=i;
}

/**
 * Vrati true, pokud je seznam prazdny.
 */
bool list_empty(List *list) {
    if (list->first==NULL){
        return true;
    }
    return false;
}

/**
 * Odstrani prvni prvek seznamu, pokud je.
 */
void list_delete_first(List *list) {
    Item *old_first=list->first;
    if(list->first!=NULL){
        list->first=list->first->next;
        free(old_first);
    }
}

/**
 * Vrati pocet polozek seznamu.
 */
unsigned list_count(List *list) {
    Item *point=list->first;
    unsigned i=0;
    while (point!=NULL){
        i++;
        point=point->next;
    }
    return i;
}


/**
 * Najde polozku seznamu s nejmensim identifikatorem. Vraci NULL, pokud je
 * seznam prazdny.
 */
Item *list_find_minid(List *list) {
    Item *minid;
    Item *point=list->first;
    minid=list->first;
    while (point!=NULL){
        if (minid->data.id > point->data.id){
            minid=point;
        }
        point=point->next;
    }
    return minid;

}

/**
 * Najde polozku seznamu s odpovidajicim jmenem objektu. Vraci NULL, pokud
 * takova polozka v seznamu neni.
 */
Item *list_find_name(List *list, char *name) {
    Item *f_name_pos=NULL;
    Item *point=list->first;
    while (point!=NULL){
        if (point->data.name == name){
            f_name_pos=point;
            break;
        }
        point=point->next;
    }
    return f_name_pos;
}

/**
 * Uvolneni seznamu.
 */
void list_dtor(List *list) {
    Item* point1=list->first;
    Item* point2;
    while (point1!=NULL){
        point2=point1->next;
        free(point1);
        point1=point2;
    }
    list->first=NULL;
}

/**
 * Remove list item at a given position. No effect if the item is not in the list.
 */
void list_delete_item(List *list, unsigned index) {
    if (index == 1){
        list_delete_first(list);
    }
    else{
        Item *point1=list->first;
        Item *point2;
        unsigned i=1;
        while(i < index-1 && point1 != NULL){
            point1=point1->next;
            i++;
        }
        if(point1!=NULL){
            point2=point1->next;
            point1->next=point2->next;
            free(point2);
        }
    }
}

/**
 * Print list items.
 * sample output: [0, "Kralem"], [2, "Malem"], [42, "Honza"]\n
 */
void print_list(List* list) {
    Item *point1=list->first;
    if(point1!=NULL){
        while(point1!=NULL){
            printf("[%d, \"%s\"], ",point1->data.id, point1->data.name);
            point1=point1->next;
        }
    }
    else{
        printf("list is empty");
    }
    printf("\n");
}

/*
checklist:

1 (0 pts): list_ctor, list_empty
2 (1 pts): item_ctor, list_insert_first, list_delete_first, list_dtor
3 (1 pts): print_list, list_count
4 (.5 pts): list_find_name
5 (.5 pts): list_find_minid
6 (1 pts): list_delete_item

7 (? pts): bonus (see below main)
*/
int main() {

    // DO NOT MODIFY THE FOLLOWING

    Item *item;
    Object o1 = {42, "Honza"};
    Object o2 = {2, "Malem"};
    Object o3 = {0, "Kralem"};

    // 1
    printf("> list_ctor...\n");
    List list = list_ctor();

    printf("> list_empty...\n");
    printf("Seznam prazdny: %s\n", list_empty(&list) ? "ano" : "ne");
    printf("\n");

    // 2
    printf("> item_ctor...\n");
    item = item_ctor(o1);

    printf("> list_insert_first...\n");
    list_insert_first(&list, item);
    printf("Seznam prazdny: %s\n", list_empty(&list) ? "ano" : "ne");

    printf("> list_delete_first...\n");
    list_delete_first(&list);
    printf("Seznam prazdny: %s\n", list_empty(&list) ? "ano" : "ne");
    printf("\n");

    // 3
    printf("> list_count...\n");
    printf("Pocet prvku v seznamu: %d\n", list_count(&list));

    printf("> list_insert_first x3...\n");
    item = item_ctor(o1);
    list_insert_first(&list, item);

    item = item_ctor(o2);
    list_insert_first(&list, item);

    item = item_ctor(o3);
    list_insert_first(&list, item);

    printf("> print_list...\n");
    print_list(&list);

    printf("Pocet prvku v seznamu: %d\n", list_count(&list));

    printf("> list_delete_first...\n");
    list_delete_first(&list);
    printf("Pocet prvku v seznamu: %d\n", list_count(&list));

    print_list(&list);

    // opetovne vlozeni objektu o1
    item = item_ctor(o1);
    printf("> list_insert_first...\n");
    list_insert_first(&list, item);

    print_list(&list);
    printf("\n");

    // 4
    printf("> list_find_name...\n");
    char* name = "Honza";
    item = list_find_name(&list, name);
    printf("Polozka s daty %s %s\n", name, item != NULL ? "nalezena" : "nenalezena");
    name = "Albert";
    item = list_find_name(&list, name);
    printf("Polozka s daty %s %s\n", name, item != NULL ? "nalezena" : "nenalezena");
    printf("\n");

    // 5
    printf("> list_find_minid...\n");
    item = list_find_minid(&list);
    if (item != NULL) {
        printf("Polozka s nejmensim identifikatorem: [%d, \"%s\"]\n",
            item->data.id, item->data.name);
    }
    else {
        printf("Polozka s nejmensim identifikatorem nenalezena\n");
    }
    printf("\n");

   // 6
    printf("> list_delete_item...\n");
    list_delete_item(&list,1);
    print_list(&list);
    list_delete_item(&list,150);
    print_list(&list);
    printf("\n");
    // 2
    printf("> list_dtor...\n");
    list_dtor(&list);
    printf("Seznam prazdny: %s\n", list_empty(&list) ? "ano" : "ne");
    printf("\n");

    // 7
   // your tests for bonus parts
    return 0;
}

// ********************************************************************************

// bonus (choose any):

// implement a double-linked list (next & prev)

/** Swap two items on given indices (if exist). */
// void list_swap(List *list, unsigned x, unsigned y);

/** Reverse a list. */
// void list_reverse(List *list);

/** Sort a list by IDs (hint: you might want to implement auxiliary functions). */
// void list_sort(List *list);

