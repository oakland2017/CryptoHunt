#include <stdio.h>
#include <stdlib.h>
#include "hashstring.h"
#include "ast.h"
#include "symtab.h"

struct symtab_item {
    char *name;
    ASTNode *type;
    Symtab_Ty symtab_ty;
    ASTNode *init;
    struct symtab_item *next;

    /* bit symbol list */
    char *bitname[8];
};

SymTabItem *head = NULL;

SymTabItem *createSymTabItem(char *name, ASTNode *type, Symtab_Ty symtab_ty, ASTNode *init)
{
    SymTabItem *p = malloc(sizeof(SymTabItem));
    int i;

    p->name = name;
    p->type = type;
    p->symtab_ty = symtab_ty;
    p->init = init;

    for (i = 0; i < 8; ++i) p->bitname[i] = NULL;
    
    p->next = head;
    head = p;

    return p;
}

int printSymTab()
{
    SymTabItem *p;

    for (p = head; p != NULL; p = p->next) {
        printf("%s", p->name);
        printf(" : ");
        outputAST(p->type);
        printf("\n");
    }
}

int createBV()
{
    SymTabItem *p;
    int i;

    for (p = head; p; p = p->next) {
        if (getTy_ASTNode(p->type) == Type && getTy_TypeNode(p->type) == Bv_Type) {
            for (i = 0; i < 8; ++i) p->bitname[i] = concatstr(p->name, stringd(i));
        }
    }

    return 0;
}

int printBVSymTab()
{
    SymTabItem *p;
    int i;

    for (p = head; p != NULL; p = p->next) {
        if (p->bitname[0] != NULL) {
            for (i = 0; i < 8; ++i) {
                printf("%s", p->bitname[i]);
                printf(" : ");
                printf("BITVECTOR(8);\n");
            }
        }
    }

}
