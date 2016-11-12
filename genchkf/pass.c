#include <stdio.h>
#include "ast.h"
#include "symtab.h"
#include "pass.h"

void foo1(ASTNode *nd)
{
    if (getTy_ASTNode(nd) == Expr && getTy_ExprNode(nd) == Let_Expr) result = nd;
}

void foo2(ASTNode *nd)
{
    if (getTy_ASTNode(nd) == String) result = nd;
}

void ex(ASTNode *assert, ASTNode *query)
{
    PreOrderTraverse(assert, foo1);
    /* outputAST(result); */
    ASTNode *inexpr = getkid(result, 1);
    ASTNode *tmp = getkid(inexpr, 0);
    PreOrderTraverse(tmp, foo2);
    outputAST(result);
    
    printf("\n");
    createBV();
    /* printBVSymTab(); */
}

