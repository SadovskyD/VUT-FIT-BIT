/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  (*tree)= NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t *node_iterator = tree;
  
  if (tree) return false;
  
  while (node_iterator)
  {
		if (key < node_iterator->key) node_iterator = node_iterator->left;
		else 
		{	
			if (key > node_iterator->key) node_iterator = node_iterator->right;
			else
				if (key == node_iterator->key)
				{
					*value = node_iterator->value;
					return true;
				}
		}
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *tmp = *tree;
	bst_node_t *previous;

	if((*tree) == NULL)
	{
		(*tree) = malloc(sizeof(struct bst_node));
		(*tree)->key = key;
		(*tree)->value = value;
		(*tree)->left = (*tree)->right = NULL;
		return;
	}

	while(tmp != NULL)
	{
		previous = tmp;
		if(tmp->key > key)
		{
			tmp = tmp->left;
		}
		else if(tmp->key < key)
		{
			tmp = tmp->right;
		}
		else if(tmp->key == key)
		{
			tmp->value=value;
			return;
		}
	}

	bst_node_t *item = malloc(sizeof(struct bst_node));
	item->value = value;
	item->key = key;
	item->left = NULL;
	item->right = NULL;

	if(previous->key > key)
	{
		previous->left = item;	
	}
	else if(previous->key < key)
	{
		previous->right = item;
	}
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *tmp, *curr_r;
	
	if (!(*tree)) return;
	
	curr_r=target->left;
	tmp=NULL;
	while (curr_r->right!=NULL)
	{
		tmp = curr_r;
		curr_r=curr_r->right;
	}
	target->key = curr_r->key;
	target->value = curr_r->value;
	if(curr_r->left != NULL) tmp->right=curr_r->left;
	else tmp->right=NULL;
	free(curr_r);
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  	bst_node_t* curr = *tree;
    bst_node_t* prev = NULL;

	if (tree == NULL) {
        return;
    }
	
    while (curr != NULL && curr->key != key)
	{
        prev = curr;
        if (key < curr->key) curr = curr->left;
        else curr = curr->right;
    }
 
    if (curr == NULL) return;
 
    if (curr->right == NULL || curr->left == NULL)
	{
        bst_node_t* newCurr;
 
		if (curr->left == NULL) newCurr = curr->right;
        else newCurr = curr->left;
 
        if (prev == NULL) return;
 
        if (curr == prev->left) prev->left = newCurr;
        else prev->right = newCurr;
        
		free(curr);
    }
    else
	{
        bst_replace_by_rightmost(curr, tree);
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  	bst_node_t *current;
	
	if(*tree == NULL) return;
	
	stack_bst_t stack;
    stack_bst_init(&stack);
	stack_bst_push(&stack, *tree);

	while(!stack_bst_empty(&stack))
	{
		current = stack_bst_pop(&stack);
		if(current->left != NULL) stack_bst_push(&stack, current->left);
		if(current->right != NULL) stack_bst_push(&stack, current->right);
        free(current);
	}
	
	(*tree)= NULL;
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  	if(tree != NULL)
	{
		while(tree != NULL)
		{
			bst_print_node(tree);
			stack_bst_push(to_visit, tree);
			tree = tree->left;
		}	
	}
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  	if(tree == NULL) return;

	stack_bst_t stack;
	stack_bst_init(&stack);	
        
    bst_leftmost_preorder(tree, &stack);
        
	while(!stack_bst_empty(&stack))
	{
		bst_node_t *current = stack_bst_pop(&stack);
		
		if(current->right != NULL)
		{
			bst_leftmost_preorder(current->right, &stack);
		}
	}
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  	if(tree != NULL)
	{
		while(tree != NULL)
		{
			stack_bst_push(to_visit, tree);
			tree = tree->left;
		}	
	}
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  	if(tree == NULL) return;
	
	stack_bst_t stack;
	stack_bst_init(&stack);
	
	bst_leftmost_inorder(tree, &stack);
	
	while(!stack_bst_empty(&stack))
	{
		bst_node_t *current = stack_bst_pop(&stack);
		
		bst_print_node(current);
		
		if(current->right != NULL) bst_leftmost_inorder(current->right, &stack);
	}
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  	if(tree != NULL)
	{
		while(tree != NULL)
		{
			stack_bst_push(to_visit, tree);
			stack_bool_push(first_visit, true);
			tree = tree->left;
		}	
	}
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  	if(tree == NULL) return;

	bool left;
	stack_bst_t stack;
	stack_bool_t booleans;

	stack_bst_init(&stack);
	stack_bool_init(&booleans);
	bst_leftmost_postorder(tree,&stack, &booleans);
	while(!stack_bst_empty(&stack))
	{
		tree = stack_bst_top(&stack);
		left = stack_bool_pop(&booleans);
		if(left == true){
			stack_bool_push(&booleans, false);
			bst_leftmost_postorder(tree->right, &stack, &booleans);
		}
		else{
			stack_bst_pop(&stack);
			bst_print_node(tree);
		}
	}
}
