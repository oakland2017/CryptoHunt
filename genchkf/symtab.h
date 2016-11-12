typedef enum {
    Non_Init,
    Expr_Init,
    Form_Init,
} Symtab_Ty;

typedef struct symtab_item SymTabItem;

SymTabItem *createSymTabItem(char *name, ASTNode *type, Symtab_Ty symtab_ty, ASTNode *init);

int printSymTab();
int createBV();
int printBVSymTab();
