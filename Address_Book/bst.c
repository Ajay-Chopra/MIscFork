#include <stdlib.h>
#include <stdio.h>
#include "adrbook.h"

const char *_bst_todo_format = "TODO [bst]: %s\nhalting\n";

// Begin helper function prototypes
bst bst_singleton(vcard *c);
int bst_insert(bst *t, vcard *c);
unsigned int bst_num_entries(bst *t);
unsigned int bst_height(bst *t);
vcard *bst_search(bst *t, char *cnet, int *n_comparisons);
unsigned int bst_c(FILE *f, bst *t, char c);
unsigned int bst_substring(FILE *f, bst *t, char *sub);
void bst_free(bst *t);
// End helper function prototypes




/*
bst_singleton: Create heap allocated bst root with two empty
subtrees
*/
bst *bst_singleton(vcard *c){
    bst *bst_single = malloc(sizeof(bst));
    bst_single->vc = c;
    bst_single->lsub = NULL;
    bst_single->rsub = NULL;
    return bst_single;
}

/*
bst_insert: insert a value into the tree and return the number of items
successfully inserted
*/

int bst_insert(bst *t, vcard *c){
    if (t == NULL){
        fprintf(stderr, "bst_insert: cannot insert vc into empty tree \n");
        exit(1);
    }
    bst *curr = t;
    while (curr != NULL) {
        if (strcmp(c->cnet, curr->vc->cnet) == 0){
            fprintf(stderr, "cnet is already present in bst \n");
            return 0;
        } else if (strcmp(c->cnet, curr->vc->cnet) < 0){
            if (curr->lsub == NULL){
                curr->lsub = bst_singleton(c);
                return 1;
            } else {
                curr = curr->lsub;

            }

        } else {
            if (curr->rsub == NULL){
                curr->rsub = bst_singleton(c);
                return 1;

            } else {
                curr = curr->rsub;
            }
        }
    }
    return 0;
}

/*
bst_num_entries: Determine the number of entries in the tree
*/
unsigned int bst_num_entries(bst *t){
  if (t == NULL){
 	return 0;
  } else {
  	return 1 + bst_num_entries(t->lsub) + bst_num_entries(t->rsub);
  }
}

/*
bst_height: calculate the height of the tree
*/
unsigned int bst_height(bst *t){
	if (t == NULL){
		return 0;
	} else {
		unsigned int lheight = bst_height(t->lsub);
		unsigned int rheight = bst_height(t->rsub);
		if (lheight > rheight){
			return lheight + 1;

		} else {
			return rheight + 1;

		}

	}

}

/*
bst_search: search for a value in the tree and return it if it is found
*/
vcard *bst_search(bst *t, char *cnet, int *n_comparisons){
	int comps = 0;
	bst *curr = t;
	while (curr != NULL){
		if (strcmp(curr->vc->cnet, cnet) == 0){
			++comps;
			*n_comparisons = comps;
			return curr->vc;

		} else if (strcmp(cnet, curr->vc->cnet) < 0){
			++comps;
			curr = curr->lsub;
		} else {
			++comps;
			curr = curr->rsub;
		}
	}
	return NULL;
}


/*
bst_c: returns the number of leafs of the tree containing the given char
*/
unsigned int bst_c(FILE *f, bst *t, char c){
	if (t == NULL){
		return 0;
	} else {
		if (c == t->vc->cnet[0]){
			unsigned int l = bst_c(f, t->lsub, c);
			fprintf(f, "%s \n", t->vc->cnet);
			unsigned int r = bst_c(f, t->rsub, c);
			return 1 + l + r;
		} else if (strcmp(&c, &t->vc->cnet[0]) > 0){
			unsigned int r = bst_c(f, t->rsub, c);
			return r;
		} else {
			unsigned int l = bst_c(f, t->lsub, c);
			return l;
		}
	}
}

/*
bst_substring: Return the number of entries in the address book that contain
the given substring
*/
unsigned int bst_substring(FILE *f, bst *t, char *sub){
	if (t == NULL){
		return 0;
	} else if (strstr(t->vc->cnet, sub) != NULL){
		unsigned int l = bst_substring(f, t->lsub, sub);
		fprintf(f, "%s \n", t->vc->cnet);
		unsigned int r = bst_substring(f, t->rsub, sub);
		return 1 + l + r;
	} else {
		unsigned int l = bst_substring(f, t->lsub, sub);
		unsigned int r = bst_substring(f, t->rsub, sub);
		return l + r;

	}

}


/*
bst_free: free the tree and all branches
*/
void bst_free(bst *t)
{
  if(t == NULL){
    return;
  } else if (t->lsub != NULL) {
    bst_free(t->lsub);
  } else if (t->rsub != NULL) {
    bst_free(t->rsub);
  }
  free(t->vc);
  free(t);
}
