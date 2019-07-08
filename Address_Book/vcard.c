#include <stdlib.h>
#include <stdio.h>
#include "adrbook.h"

const char *_vcard_todo_format = "TODO [vcard]: %s\nhalting\n";

/* vcard_new : allocate a new vcard, copy all strings, return new object
 * note: this is a "deep copy" as opposed to a "shallow copy"; the string 
 *   arguments are not to be shared by the newly allocated vcard
 */
vcard *vcard_new(char *cnet, char *email, char *fname, char *lname, char *tel){
    vcard *new_card = malloc(sizeof(vcard));
    new_card->cnet = strdup(cnet);
    new_card->email = strdup(email);
    new_card->fname = strdup(fname);
    new_card->lname = strdup(lname);
    new_card->tel = strdup(tel);
    
    return new_card;
}

/* vcard_free : free vcard and the strings it points to
 */
void vcard_free(vcard *c){
    free(c->cnet);
    free(c->email);
    free(c->fname);
    free(c->lname);
    free(c->tel);
    free(c);
    
}
    


void vcard_show(FILE *f, vcard *c){
    fprintf(f, "CNET ID: %s \nEMAIL: %s \nFIRST NAME: %s \nLAST NAME: %s \nPHONE: %s \n",
            c->cnet, c->email, c->fname, c->lname, c->tel);
    
}
    
    

    
    
  
