typedef struct astnode ASTNode;


/* Expression Types */
typedef enum {
    PEP_Expr,                        /* ( Expr ) */
    EBEB_Expr,                       /* Expr [ Expr ] */
    EPEP_Expr,                       /* Expr ( Expr ) */
    Extract_Expr,                    /* Expr [ num : num ] */

    /* Bit operations */
    Neg_Expr,
    And_Expr,
    Or_Expr,
    Xor_Expr,
    Nand_Expr,
    Nor_Expr,
    Xnor_Expr,
    Sx_Expr,
    Zx_Expr,

    Concat_Expr,
    LeftShift_Expr,
    RightShift_Expr,
    SHL_Expr,
    LSHR_Expr,
    ASHR_Expr,

    /* Arithmetic operations */
    Plus_Expr,
    Sub_Expr,
    Uminus_Expr,
    Mult_Expr,
    Div_Expr,
    Mod_Expr,
    SDiv_Expr,
    SRem_Expr,

    If_Expr,
    Array_Update_Expr,
    Let_Expr,

    Else_Expr,
    Elsif_Expr,
} Ty_Expr;


/* Formula Types */
typedef enum {
    PFP_Form,                   /* ( F ) */
    
    Eq_Form,                    /* E = E */
    Neq_Form,                   /* E != E */
    Not_Form,                   /* ~F */

    Or_Form,                  /* F or F */
    Nor_Form,                 /* F nor F */
    And_Form,                 /* F and F */
    Nand_Form,                /* F nand F */
    Imply_Form,               /* F implies F */
    Iff_Form,                 /* F iff F */
    Xor_Form,                 /* F xor F */

    LT_Form,
    LE_Form,
    GE_Form,
    SLT_Form,
    SGT_Form,
    SLE_Form,
    SGE_Form,

    If_Form,
    True_Form,
    False_Form,
    Let_Form,

    Else_Form,
    Elsif_Form,
} Ty_Form;

/* AST Node Types */
typedef enum {
    String,
    Numeral,
    BVConst,
    Expr,
    Formula,
    Assert,

    Updates,
    LetDecl,
    Type,
    Query,

    Undefined,
} Ty_ASTNode;

typedef enum {
    Let_No_Type_Expr,
    Let_Type_Expr,
    Let_No_Type_Form,
    Let_Type_Form,
} Ty_LetDecl;

typedef enum {
    Bv_Type,
    Bool_Type,
    Array_Type,
} Ty_Type;


ASTNode *createStringNode(char *str);
ASTNode *createBVConstNode(char *str);
ASTNode *createNumeralNode(char *str);

ASTNode *createFormNode(Ty_Form ty, ASTNode *nd0, ASTNode *nd1, ASTNode *nd2);
ASTNode *createExprNode(Ty_Expr ty, ASTNode *nd0, ASTNode *nd1, ASTNode *nd2);
ASTNode *createAssertNode(ASTNode *nd0);
ASTNode *createUpdatesNode(ASTNode *nd0, ASTNode *nd1, ASTNode *nd2);
ASTNode *createLetDeclNode(Ty_LetDecl ty, ASTNode *nd0, ASTNode *nd1, ASTNode *nd2);
ASTNode *createTypeNode(Ty_Type ty, ASTNode *nd0, ASTNode *nd1);
ASTNode *createQueryNode(ASTNode *nd0);


int outputAST(ASTNode *nd);
int PreOrderTraverse(ASTNode *nd, void (*f)(ASTNode *ndx));
void foo(ASTNode *nd);

ASTNode *result;                /* Store the result of Traversing */


Ty_ASTNode getTy_ASTNode(ASTNode *nd);
Ty_Type getTy_TypeNode(ASTNode *nd);
Ty_Expr getTy_ExprNode(ASTNode *nd);
ASTNode *getkid(ASTNode *nd, int i);
