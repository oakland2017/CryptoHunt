#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

struct ExprNode {
    Ty_Expr type;
};

struct FormNode {
    Ty_Form type;
};

struct AssertNode {
};

struct StringNode {
    char *name;
};

struct BVConstNode {
    char *name;
};

struct NumeralNode {
    char *name;
};

struct UpdatesNode {
};

struct LetDeclNode {
    Ty_LetDecl type;
};

struct TypeNode{
    Ty_Type type;
};

struct QueryNode{
};


struct astnode {
    Ty_ASTNode type;
    union {
        struct StringNode str_node;
        struct NumeralNode num_node;
        struct BVConstNode bvconst_node;
        struct ExprNode expr_node;
        struct FormNode form_node;
        struct AssertNode assert_node;
        struct UpdatesNode updates_node;
        struct LetDeclNode letdecl_node;
        struct TypeNode type_node;
        struct QueryNode query_node;
    } x;
    ASTNode *kids[3];
};


ASTNode *createStringNode(char *str)
{
    ASTNode *p = malloc(sizeof(ASTNode));
    
    p->type = String;
    p->x.str_node.name = str;

    return p;
}

ASTNode *createBVConstNode(char *str)
{
    ASTNode *p = malloc(sizeof(ASTNode));
    
    p->type = BVConst;
    p->x.bvconst_node.name = str;

    return p;
}

ASTNode *createNumeralNode(char *str)
{
    ASTNode *p = malloc(sizeof(ASTNode));
    
    p->type = Numeral;
    p->x.num_node.name = str;

    return p;
}

ASTNode *createFormNode(Ty_Form ty, ASTNode *nd0, ASTNode *nd1, ASTNode *nd2)
{
    ASTNode *p = malloc(sizeof(ASTNode));

    p->type = Formula;
    p->x.form_node.type = ty;
    p->kids[0] = nd0;
    p->kids[1] = nd1;
    p->kids[2] = nd2;

    return p;
}

ASTNode *createExprNode(Ty_Expr ty, ASTNode *nd0, ASTNode *nd1, ASTNode *nd2)
{
    ASTNode *p = malloc(sizeof(ASTNode));

    p->type = Expr;
    p->x.expr_node.type = ty;
    p->kids[0] = nd0;
    p->kids[1] = nd1;
    p->kids[2] = nd2;

    return p;
}

ASTNode *createAssertNode(ASTNode *nd0)
{
    ASTNode *p = malloc(sizeof(ASTNode));

    p->type = Assert;
    p->kids[0] = nd0;

    return p;
}

ASTNode *createQueryNode(ASTNode *nd0)
{
    ASTNode *p = malloc(sizeof(ASTNode));

    p->type = Query;
    p->kids[0] = nd0;

    return p;
}

ASTNode *createUpdatesNode(ASTNode *nd0, ASTNode *nd1, ASTNode *nd2)
{
    ASTNode *p = malloc(sizeof(ASTNode));

    p->type = Updates;
    p->kids[0] = nd0;
    p->kids[1] = nd1;
    p->kids[2] = nd2;

    return p;
}

ASTNode *createLetDeclNode(Ty_LetDecl ty, ASTNode *nd0, ASTNode *nd1, ASTNode *nd2)
{
    ASTNode *p = malloc(sizeof(ASTNode));

    p->type = LetDecl;

    p->x.letdecl_node.type = ty;
    p->kids[0] = nd0;
    p->kids[1] = nd1;
    p->kids[2] = nd2;

    return p;
}

ASTNode *createTypeNode(Ty_Type ty, ASTNode *nd0, ASTNode *nd1)
{
    ASTNode *p = malloc(sizeof(ASTNode));

    p->type = Type;

    p->x.type_node.type = ty;
    p->kids[0] = nd0;
    p->kids[1] = nd1;

    return p;
}

Ty_ASTNode getTy_ASTNode(ASTNode *nd)
{
    if (nd)
        return nd->type;
    else
        return Undefined;
}

Ty_Type getTy_TypeNode(ASTNode *nd)
{
    if (nd)
        return nd->x.type_node.type;
    else
        return Undefined;
}

Ty_Expr getTy_ExprNode(ASTNode *nd)
{
    if (nd)
        return nd->x.expr_node.type;
    else
        return Undefined;
}

ASTNode *getkid(ASTNode *nd, int i)
{
    return nd->kids[i];
}


int print_string(char* str)
{
    printf("%s", str);
    return 0;
}

int print_space()
{
    printf(" ");
    return 0;
}

int outputAST(ASTNode *nd)
{
    switch (nd->type) {
    case String:
        print_string(nd->x.str_node.name);
        break;
    case Numeral:
        print_string(nd->x.num_node.name);
        break;
    case BVConst:
        print_string(nd->x.bvconst_node.name);
        break;
    case Expr:
        switch (nd->x.expr_node.type) {
        case PEP_Expr:
            printf("(");
            outputAST(nd->kids[0]);
            printf(")");
            break;
        case EBEB_Expr:
            outputAST(nd->kids[0]);
            printf("[");
            outputAST(nd->kids[1]);
            printf("]");
            break;
        case EPEP_Expr:
            outputAST(nd->kids[0]);
            printf("(");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case Extract_Expr:
            outputAST(nd->kids[0]);
            printf("[");
            outputAST(nd->kids[1]);
            printf(":");
            outputAST(nd->kids[2]);
            printf("]");
            break;
        case Neg_Expr:
            printf("~");
            outputAST(nd->kids[0]);
            break;
        case And_Expr:
            outputAST(nd->kids[0]);
            printf("&");
            outputAST(nd->kids[1]);
            break;
        case Or_Expr:
            outputAST(nd->kids[0]);
            printf("|");
            outputAST(nd->kids[1]);
            break;
        case Xor_Expr:
            printf("BVXOR(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case Nand_Expr:
            printf("BVNAND(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case Nor_Expr:
            printf("BVNOR(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case Xnor_Expr:
            printf("BVXNOR(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case Sx_Expr:
            printf("BVSX(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case Zx_Expr:
            printf("BVZX(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case Concat_Expr:
            outputAST(nd->kids[0]);
            printf("@");
            outputAST(nd->kids[1]);
            break;
        case LeftShift_Expr:
            outputAST(nd->kids[0]);
            printf("<<");
            outputAST(nd->kids[1]);
            break;
        case RightShift_Expr:
            outputAST(nd->kids[0]);
            printf(">>");
            outputAST(nd->kids[1]);
            break;
        case SHL_Expr:
            printf("BVSHL(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case LSHR_Expr:
            printf("BVLSHR(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case ASHR_Expr:
            printf("BVASHR(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case Plus_Expr:
            printf("BVPLUS(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(", ");
            outputAST(nd->kids[2]);
            printf(")");
            break;
        case Sub_Expr:
            printf("BVSUB(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(", ");
            outputAST(nd->kids[2]);
            printf(")");
            break;
        case Uminus_Expr:
            printf("BVUMINUS(");
            outputAST(nd->kids[0]);
            printf(")");
            break;
        case Mult_Expr:
            printf("BVMULT(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(", ");
            outputAST(nd->kids[2]);
            printf(")");
            break;
        case Div_Expr:
            printf("BVDIV(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(", ");
            outputAST(nd->kids[2]);
            printf(")");
            break;
        case Mod_Expr:
            printf("BVMOD(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(", ");
            outputAST(nd->kids[2]);
            printf(")");
            break;
        case SDiv_Expr:
            printf("SBVDIV(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(", ");
            outputAST(nd->kids[2]);
            printf(")");
            break;
        case SRem_Expr:
            printf("SBVREM(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(", ");
            outputAST(nd->kids[2]);
            printf(")");
            break;
        case If_Expr:
            printf(" IF ");
            outputAST(nd->kids[0]);
            printf(" THEN ");
            outputAST(nd->kids[1]);
            outputAST(nd->kids[2]);
            break;
        case Else_Expr:
            printf(" ELSE ");
            outputAST(nd->kids[0]);
            printf(" ENDIF ");
            break;
        case Elsif_Expr:
            printf(" ELSIF ");
            outputAST(nd->kids[0]);
            printf(" THEN ");
            outputAST(nd->kids[1]);
            outputAST(nd->kids[2]);
            break;
        case Array_Update_Expr:
            outputAST(nd->kids[0]);
            printf(" WITH ");
            outputAST(nd->kids[1]);
            break;
        case Let_Expr:
            printf("LET ");
            outputAST(nd->kids[0]);
            printf(" IN ");
            outputAST(nd->kids[1]);
            break;
        }
        break;
    case Formula:
        switch (nd->x.form_node.type) {
        case PFP_Form:
            printf("(");
            outputAST(nd->kids[0]);
            printf(")");
            break;
        case Eq_Form:
            outputAST(nd->kids[0]);
            printf(" = ");
            outputAST(nd->kids[1]);
            break;
        case Neq_Form:
            outputAST(nd->kids[0]);
            printf(" /= ");
            outputAST(nd->kids[1]);
            break;
        case Not_Form:
            printf(" NOT ");
            outputAST(nd->kids[0]);
            break;
        case Or_Form:
            outputAST(nd->kids[0]);
            printf(" OR ");
            outputAST(nd->kids[1]);
            break;
        case Nor_Form:
            outputAST(nd->kids[0]);
            printf(" NOR ");
            outputAST(nd->kids[1]);
            break;
        case And_Form:
            outputAST(nd->kids[0]);
            printf(" AND ");
            outputAST(nd->kids[1]);
            break;
        case Nand_Form:
            outputAST(nd->kids[0]);
            printf(" NAND ");
            outputAST(nd->kids[1]);
            break;
        case Imply_Form:
            outputAST(nd->kids[0]);
            printf(" => ");
            outputAST(nd->kids[1]);
            break;
        case Iff_Form:
            outputAST(nd->kids[0]);
            printf(" <=> ");
            outputAST(nd->kids[1]);
            break;
        case Xor_Form:
            outputAST(nd->kids[0]);
            printf(" XOR ");
            outputAST(nd->kids[1]);
            break;
        case LT_Form:
            printf("BVLT(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case LE_Form:
            printf("BVLE(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case GE_Form:
            printf("BVGE(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case SLT_Form:
            printf("BVSLT(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case SGT_Form:
            printf("BVSGT(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case SLE_Form:
            printf("BVSLE(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case SGE_Form:
            printf("BVSGE(");
            outputAST(nd->kids[0]);
            printf(", ");
            outputAST(nd->kids[1]);
            printf(")");
            break;
        case If_Form:
            printf("IF ");
            outputAST(nd->kids[0]);
            printf(" THEN ");
            outputAST(nd->kids[1]);
            outputAST(nd->kids[2]);
            break;
        case Else_Form:
            printf(" ELSE ");
            outputAST(nd->kids[0]);
            printf(" ENDIF ");
            break;
        case Elsif_Form:
            printf(" ELSIF_TOK ");
            outputAST(nd->kids[0]);
            printf(" THEN ");
            outputAST(nd->kids[1]);
            outputAST(nd->kids[2]);
            break;
        }
        break;
    case Assert:
        printf("ASSERT");
        outputAST(nd->kids[0]);
        printf(";");
        break;

    case Updates:
        printf("[");
        outputAST(nd->kids[0]);
        printf("]");
        printf(" := ");
        outputAST(nd->kids[1]);
        break;
    case LetDecl:
        switch (nd->x.letdecl_node.type) {
        case Let_No_Type_Expr:
        case Let_No_Type_Form:
            outputAST(nd->kids[0]);
            printf(" = ");
            outputAST(nd->kids[1]);
            break;
        case Let_Type_Expr:
        case Let_Type_Form:
            outputAST(nd->kids[0]);
            printf(" : ");
            outputAST(nd->kids[1]);
            printf(" = ");
            outputAST(nd->kids[2]);
            break;
        }
        break;
    case Type:
        switch (nd->x.type_node.type) {
        case Bv_Type:
            printf("BITVECTOR(");
            outputAST(nd->kids[0]);
            printf(")");
            break;
        case Bool_Type:
            printf("BOOLEAN");
            break;
        case Array_Type:
            printf(" ARRAY ");
            outputAST(nd->kids[0]);
            printf(" OF ");
            outputAST(nd->kids[1]);
            break;
        }
        break;
    case Query:
        printf(" QUERY ");
        outputAST(nd->kids[0]);
        break;
    }

    return 0;
}

int PreOrderTraverse(ASTNode *nd, void (*f)(ASTNode *ndx))
{
    switch (nd->type) {
    case String:
    case Numeral:
    case BVConst:
        (*f)(nd);
        break;
    case Expr:
        switch (nd->x.expr_node.type) {
        case PEP_Expr:
        case Neg_Expr:
        case Uminus_Expr:
        case Else_Expr:
            (*f)(nd);
            PreOrderTraverse(nd->kids[0], f);
            break;
        case EBEB_Expr:
        case EPEP_Expr:
        case And_Expr:
        case Or_Expr:
        case Xor_Expr:
        case Nand_Expr:
        case Nor_Expr:
        case Xnor_Expr:
        case Sx_Expr:
        case Zx_Expr:
        case Concat_Expr:
        case LeftShift_Expr:
        case RightShift_Expr:
        case SHL_Expr:
        case LSHR_Expr:
        case ASHR_Expr:
        case Array_Update_Expr:
        case Let_Expr:
            (*f)(nd);
            PreOrderTraverse(nd->kids[0], f);
            PreOrderTraverse(nd->kids[1], f);
            break;
        case Extract_Expr:
        case Plus_Expr:
        case Sub_Expr:
        case Mult_Expr:
        case Div_Expr:
        case Mod_Expr:
        case SDiv_Expr:
        case SRem_Expr:
        case If_Expr:
        case Elsif_Expr:
            (*f)(nd);
            PreOrderTraverse(nd->kids[0], f);
            PreOrderTraverse(nd->kids[1], f);
            PreOrderTraverse(nd->kids[2], f);
            break;
        }
        break;
    case Formula:
        switch (nd->x.form_node.type) {
        case PFP_Form:
        case Not_Form:
        case Else_Form:
            (*f)(nd);
            PreOrderTraverse(nd->kids[0], f);
            break;
        case Eq_Form:
        case Neq_Form:
        case Or_Form:
        case Nor_Form:
        case And_Form:
        case Nand_Form:
        case Imply_Form:
        case Iff_Form:
        case Xor_Form:
        case LT_Form:
        case LE_Form:
        case GE_Form:
        case SLT_Form:
        case SGT_Form:
        case SLE_Form:
        case SGE_Form:
            (*f)(nd);
            PreOrderTraverse(nd->kids[0], f);
            PreOrderTraverse(nd->kids[1], f);
            break;
        case If_Form:
        case Elsif_Form:
            (*f)(nd);
            PreOrderTraverse(nd->kids[0], f);
            PreOrderTraverse(nd->kids[1], f);
            PreOrderTraverse(nd->kids[2], f);
            break;
        }
        break;
    case Assert:
        (*f)(nd);
        PreOrderTraverse(nd->kids[0], f);
        break;
    case Updates:
        (*f)(nd);
        PreOrderTraverse(nd->kids[0], f);
        PreOrderTraverse(nd->kids[1], f);
        break;
    case LetDecl:
        switch (nd->x.letdecl_node.type) {
        case Let_No_Type_Expr:
        case Let_No_Type_Form:
            (*f)(nd);
            PreOrderTraverse(nd->kids[0], f);
            PreOrderTraverse(nd->kids[1], f);
            break;
        case Let_Type_Expr:
        case Let_Type_Form:
            (*f)(nd);
            PreOrderTraverse(nd->kids[0], f);
            PreOrderTraverse(nd->kids[1], f);
            PreOrderTraverse(nd->kids[2], f);
            break;
        }
        break;
    case Type:
        switch (nd->x.type_node.type) {
        case Bv_Type:
            (*f)(nd);
            PreOrderTraverse(nd->kids[0], f);
            break;
        case Bool_Type:
            (*f)(nd);
            break;
        case Array_Type:
            (*f)(nd);
            PreOrderTraverse(nd->kids[0], f);
            PreOrderTraverse(nd->kids[1], f);
            break;
        }
        break;
    case Query:
        (*f)(nd);
        PreOrderTraverse(nd->kids[0], f);
        break;
    }

    return 0;
}

