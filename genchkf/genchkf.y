%{
#include <stdio.h>
#include "hashstring.h"
#include "ast.h"
#include "symtab.h"
#include "pass.h"
  
int yylex(void);
void yyerror(char *s);
extern int yylineno;

%}


%union {
  /* unsigned int uintval; */
  ASTNode *node;
  /* struct String_Node stringnode; */
  /* struct BVConst_Node bvconstnode; */
  /* struct Numeral_Node numeralnode; */
  char *str;
};

%start cmd

%token  AND_TOK                 "AND"
%token  OR_TOK                  "OR"
%token  NOT_TOK                 "NOT"
%token  EXCEPT_TOK              "EXCEPT"
%token  XOR_TOK                 "XOR"
%token  NAND_TOK                "NAND"
%token  NOR_TOK                 "NOR"
%token  IMPLIES_TOK             "=>"
%token  IFF_TOK                 "<=>"

%token  IF_TOK                  "IF"
%token  THEN_TOK                "THEN"
%token  ELSE_TOK                "ELSE"
%token  ELSIF_TOK               "ELSIF"
%token  END_TOK                 "END"
%token  ENDIF_TOK               "ENDIF"
%token  NEQ_TOK                 "/="
%token  ASSIGN_TOK              ":="

%token  BV_TOK                  "BV"
%token  BVLEFTSHIFT_TOK         "<<"
%token  BVRIGHTSHIFT_TOK        ">>"

%token  BVSHL_TOK               "BVSHL"
%token  BVLSHR_TOK              "BVLSHR"
%token  BVASHR_TOK              "BVASHR"

%token  BVPLUS_TOK              "BVPLUS"
%token  BVSUB_TOK               "BVSUB"
%token  BVUMINUS_TOK            "BVUMINUS"
%token  BVMULT_TOK              "BVMULT"

%token  BVDIV_TOK               "BVDIV"
%token  BVMOD_TOK               "BVMOD"
%token  SBVDIV_TOK              "SBVDIV"
%token  SBVREM_TOK              "SBVREM"

%token  BVNEG_TOK               "~"
%token  BVAND_TOK               "&"
%token  BVOR_TOK                "|"
%token  BVXOR_TOK               "BVXOR"
%token  BVNAND_TOK              "BVNAND"
%token  BVNOR_TOK               "BVNOR"
%token  BVXNOR_TOK              "BVXNOR"
%token  BVCONCAT_TOK            "@"

%token  BVLT_TOK                "BVLT"
%token  BVGT_TOK                "BVGT"
%token  BVLE_TOK                "BVLE"
%token  BVGE_TOK                "BVGE"

%token  BVSLT_TOK               "BVSLT"
%token  BVSGT_TOK               "BVSGT"
%token  BVSLE_TOK               "BVSLE"
%token  BVSGE_TOK               "BVSGE"
%token  BOOL_TO_BV_TOK          "BOOLBV"
%token  BVSX_TOK                "BVSX"
%token  BVZX_TOK                "BVZX"
%token  BOOLEXTRACT_TOK         "BOOLEXTRACT"
%token  ASSERT_TOK              "ASSERT"
%token  QUERY_TOK               "QUERY"

%token  BOOLEAN_TOK             "BOOLEAN"
%token  ARRAY_TOK               "ARRAY"
%token  OF_TOK                  "OF"
%token  WITH_TOK                "WITH"

%token  TRUELIT_TOK             "TRUE"
%token  FALSELIT_TOK            "FALSE"

%token  IN_TOK                  "IN"
%token  LET_TOK                 "LET"

%token  PUSH_TOK                "PUSH"
%token  POP_TOK                 "POP"

%left IN_TOK
%left XOR_TOK
%left IFF_TOK
%right IMPLIES_TOK
%left OR_TOK
%left AND_TOK
%left NAND_TOK
%left NOR_TOK
%left NOT_TOK
%left BVCONCAT_TOK
%left BVOR_TOK
%left BVAND_TOK
%left BVXOR_TOK
%left BVNAND_TOK
%left BVNOR_TOK
%left BVXNOR_TOK
%left BVNEG_TOK
%left BVLEFTSHIFT_TOK BVRIGHTSHIFT_TOK
%left WITH_TOK

%nonassoc '=' NEQ_TOK ASSIGN_TOK
%nonassoc BVLT_TOK BVLE_TOK BVGT_TOK BVGE_TOK
%nonassoc BVUMINUS_TOK BVPLUS_TOK BVSUB_TOK BVSX_TOK BVZX_TOK
%nonassoc '[' 
%nonassoc '{' '.' '('
%nonassoc BV_TOK

/* %type <vec>  Exprs */
/* %type <vec>  Asserts */
/* %type <stringVec>  FORM_IDs reverseFORM_IDs */
/* %type <node> Expr Formula ForDecl IfExpr ElseRestExpr IfForm ElseRestForm Assert Query ArrayUpdateExpr */
/* %type <Index_To_UpdateValue> Updates */

/* %type <indexvaluewidth>  BvType BoolType ArrayType Type */

/* %type <node> Expr Formula ForDecl IfExpr ElseRestExpr IfForm ElseRestForm Assert Query ArrayUpdateExpr */

%type <node> Expr IfExpr ElseRestExpr Formula Updates ArrayUpdateExpr LetDecl LetDecls Type IfForm ElseRestForm BvType BoolType ArrayType Assert Query

%token <str> BVCONST_TOK
/* %token <node> TERMID_TOK FORMID_TOK COUNTEREXAMPLE_TOK */
%token <node> COUNTEREXAMPLE_TOK
%token <str> NUMERAL_TOK
%token <str> BIN_BASED_NUMBER
%token <str> DEC_BASED_NUMBER
%token <str> HEX_BASED_NUMBER
%token <str> STRING_TOK

%%

cmd:
   VarDecls Assert Query counterexample     { ex($2, $3); }
;

/* cmd: */
/*    other_cmd                             { printf("Counterexample is missing! \n"); } */
/* |  other_cmd counterexample              { $$ = $1; } */
/* ; */

counterexample:
   COUNTEREXAMPLE_TOK ';'
;

/* other_cmd: */
/*    Query                                 { printf("Asserts and VarDecls are missing \n"); } */
/* |  VarDecls Query                        { printf("Asserts is missing! \n"); } */
/* |  other_cmd1 Query */
/* ; */

/* other_cmd1: */
/*    VarDecls Asserts                      { $$ = $2; } */
/* |  Asserts                               { printf("VarDecls is missing! \n"); } */
/* |  other_cmd1 VarDecls Asserts           { printf("Only one VarDecls Asserts is supported! \n"); } */
/* ; */

/* Asserts: */
/*    Assert                                { $$ = $1; } */
/* |  Asserts Assert                        { printf("Multiple asserts is unsupported! \n"); } */
/* ;                                         */
                                         
Assert:                                  
   ASSERT_TOK Formula ';'                { $$ = createAssertNode($2); }
;                                        
                                         
Query:                                   
   QUERY_TOK Formula ';'                 { $$ = createQueryNode($2); }
;

VarDecls:
   VarDecl ';'
|  VarDecls  VarDecl ';'
;

VarDecl:
   STRING_TOK ':' Type                   { createSymTabItem(string($1), $3, Non_Init, NULL); }
|  STRING_TOK ':' Type '=' Expr          { createSymTabItem(string($1), $3, Expr_Init, $5); }
|  STRING_TOK ':' Type '=' Formula       { createSymTabItem(string($1), $3, Form_Init, $5); }
;

/* VarDecl: */
/*    FORM_IDs ':' Type */
/* |  FORM_IDs ':' Type '=' Expr */
/* |  FORM_IDs ':' Type '=' Formula */
/* ; */

/* reverseFORM_IDs: */
/*    STRING_TOK */
/* |  STRING_TOK ',' reverseFORM_IDs */
/* ; */

/* FORM_IDs: */
/*    reverseFORM_IDs */
/* ; */

/* ForDecl: */
/*    FORMID_TOK ':' Type */
/* ; */

/* Grammar for Types */
Type:
   BvType                                              { $$ = $1; }
|  BoolType                                            { $$ = $1; }
|  ArrayType                                           { $$ = $1; }
;               

BvType:
   BV_TOK '(' NUMERAL_TOK ')'                          { $$ = createTypeNode(Bv_Type, createNumeralNode($3), NULL); }
;

BoolType:
   BOOLEAN_TOK                                         { $$ = createTypeNode(Bool_Type, NULL, NULL); }
;

ArrayType:
   ARRAY_TOK BvType OF_TOK BvType                      { $$ = createTypeNode(Array_Type, $2, $4); }
;

/* Grammar for ITEs which are a type of Term */
IfExpr:
   IF_TOK Formula THEN_TOK Expr ElseRestExpr           { $$ = createExprNode(If_Expr, $2, $4, $5); }
;

ElseRestExpr:
   ELSE_TOK Expr ENDIF_TOK                             { $$ = createExprNode(Else_Expr, $2, NULL, NULL); }
|  ELSIF_TOK Expr THEN_TOK Expr ElseRestExpr           { $$ = createExprNode(Elsif_Expr, $2, $4, $5); }
;

/* Grammar for formulas */
Formula:
   '(' Formula ')'                                     { $$ = createFormNode(PFP_Form, $2, NULL, NULL); }
/* |  FORMID_TOK                                          { printf("FORMID_TOK \n"); } */
/* |  FORMID_TOK '(' Expr ')'                             { printf("FORMID_TOK ( Expr ) \n"); } */
|  BOOLEXTRACT_TOK '(' Expr ',' NUMERAL_TOK ')'        { printf("BOOLEXTRACT_TOK ( Expr , NUMERAL_TOK \n"); }
|  Expr '=' Expr                                       { $$ = createFormNode(Eq_Form, $1, $3, NULL); }
|  Expr NEQ_TOK Expr                                   { $$ = createFormNode(Neq_Form, $1, $3, NULL); }
|  NOT_TOK Formula                                     { $$ = createFormNode(Not_Form, $2, NULL, NULL); }
|  Formula OR_TOK Formula %prec OR_TOK                 { $$ = createFormNode(Or_Form, $1, $3, NULL); }
|  Formula NOR_TOK Formula                             { $$ = createFormNode(Nor_Form, $1, $3, NULL); }
|  Formula AND_TOK Formula %prec AND_TOK               { $$ = createFormNode(And_Form, $1, $3, NULL); }
|  Formula NAND_TOK Formula                            { $$ = createFormNode(Nand_Form, $1, $3, NULL); }
|  Formula IMPLIES_TOK Formula                         { $$ = createFormNode(Imply_Form, $1, $3, NULL); }
|  Formula IFF_TOK Formula                             { $$ = createFormNode(Iff_Form, $1, $3, NULL); }
|  Formula XOR_TOK Formula                             { $$ = createFormNode(Xor_Form, $1, $3, NULL); }
|  BVLT_TOK '(' Expr ',' Expr ')'                      { $$ = createFormNode(LT_Form, $3, $5, NULL); }
|  BVLE_TOK '(' Expr ',' Expr ')'                      { $$ = createFormNode(LE_Form, $3, $5, NULL); }
|  BVGE_TOK '(' Expr ',' Expr ')'                      { $$ = createFormNode(GE_Form, $3, $5, NULL); }
|  BVSLT_TOK '(' Expr ',' Expr ')'                     { $$ = createFormNode(SLT_Form, $3, $5, NULL); }
|  BVSGT_TOK '(' Expr ',' Expr ')'                     { $$ = createFormNode(SGT_Form, $3, $5, NULL); }
|  BVSLE_TOK '(' Expr ',' Expr ')'                     { $$ = createFormNode(SLE_Form, $3, $5, NULL); }
|  BVSGE_TOK '(' Expr ',' Expr ')'                     { $$ = createFormNode(SGE_Form, $3, $5, NULL); }
|  IfForm                                              { $$ = $1; }
|  TRUELIT_TOK                                         { $$ = createFormNode(True_Form, NULL, NULL, NULL); }
|  FALSELIT_TOK                                        { $$ = createFormNode(False_Form, NULL, NULL, NULL); }
|  LET_TOK LetDecls IN_TOK Formula                     { $$ = createFormNode(Let_Form, $2, $4, NULL); }
;


/* Grammar for ITEs which are Formulas */
IfForm:
   IF_TOK Formula THEN_TOK Formula ElseRestForm        { $$ = createFormNode(If_Form, $2, $4, $5); }
;

ElseRestForm:
   ELSE_TOK Formula ENDIF_TOK                          { $$ = createFormNode(Else_Form, $2, NULL, NULL); }
|  ELSIF_TOK Formula THEN_TOK Formula ElseRestForm     { $$ = createFormNode(Elsif_Form, $2, $4, $5); }
|  STRING_TOK                                          { $$ = createStringNode($1); }
;


/* Grammar for a list of expressions */
/* Exprs: */
/*    Expr */
/* |  Exprs ',' Expr            { printf("Exprs: Exprs , Expr \n"); } */
/* ; */


/* Grammar for Expr */
Expr:
   /* TERMID_TOK */
   '(' Expr ')'                             { $$ = createExprNode(PEP_Expr, $2, NULL, NULL); }
|  BVCONST_TOK                              { $$ = createBVConstNode($1); }
|  BOOL_TO_BV_TOK '(' Formula ')'           { printf("Expr: BOOL_TO_BV_TOK \n"); }
|  NUMERAL_TOK BIN_BASED_NUMBER             { printf("Expr: NUMERAL_TOK BIN_BASED_NUMBER \n"); }
|  NUMERAL_TOK DEC_BASED_NUMBER             { printf("Expr: NUMERAL_TOK DEC_BASED_NUMBER \n"); }
|  NUMERAL_TOK HEX_BASED_NUMBER             { printf("Expr: NUMERAL_TOK HEX_BASED_NUMBER \n"); }
|  Expr '[' Expr ']'                        { $$ = createExprNode(EBEB_Expr, $1, $3, NULL); }
|  Expr '(' Expr ')'                        { $$ = createExprNode(EPEP_Expr, $1, $3, NULL); }
|  Expr '[' NUMERAL_TOK ':' NUMERAL_TOK ']' { $$ = createExprNode(Extract_Expr, $1, createNumeralNode($3), createNumeralNode($5)); }
|  BVNEG_TOK Expr                           { $$ = createExprNode(Neg_Expr, $2, NULL, NULL); }
|  Expr BVAND_TOK Expr                      { $$ = createExprNode(And_Expr, $1, $3, NULL); }
|  Expr BVOR_TOK Expr                       { $$ = createExprNode(Or_Expr, $1, $3, NULL); }
|  BVXOR_TOK '(' Expr ',' Expr ')'          { $$ = createExprNode(Xor_Expr, $3, $5, NULL); }
|  BVNAND_TOK '(' Expr ',' Expr ')'         { $$ = createExprNode(Nand_Expr, $3, $5, NULL); }
|  BVNOR_TOK '(' Expr ',' Expr ')'          { $$ = createExprNode(Nor_Expr, $3, $5, NULL); }
|  BVXNOR_TOK '(' Expr ',' Expr ')'         { $$ = createExprNode(Xnor_Expr, $3, $5, NULL); }
|  BVSX_TOK '(' Expr ',' NUMERAL_TOK ')'    { $$ = createExprNode(Sx_Expr, $3, createNumeralNode($5), NULL); }
|  BVZX_TOK '(' Expr ',' NUMERAL_TOK ')'    { $$ = createExprNode(Zx_Expr, $3, createNumeralNode($5), NULL); }
|  Expr BVCONCAT_TOK Expr                   { $$ = createExprNode(Concat_Expr, $1, $3, NULL); }
|  Expr BVLEFTSHIFT_TOK NUMERAL_TOK         { $$ = createExprNode(LeftShift_Expr, $1, createNumeralNode($3), NULL); }
|  Expr BVLEFTSHIFT_TOK Expr                { $$ = createExprNode(LeftShift_Expr, $1, $3, NULL); }
|  Expr BVRIGHTSHIFT_TOK NUMERAL_TOK        { $$ = createExprNode(RightShift_Expr, $1, createNumeralNode($3), NULL); }
|  Expr BVRIGHTSHIFT_TOK Expr               { $$ = createExprNode(RightShift_Expr, $1, $3, NULL); }
|  BVSHL_TOK '(' Expr ',' Expr ')'          { $$ = createExprNode(SHL_Expr, $3, $5, NULL); }
|  BVLSHR_TOK '(' Expr ',' Expr ')'         { $$ = createExprNode(LSHR_Expr, $3, $5, NULL); }
|  BVASHR_TOK '(' Expr ',' Expr ')'         { $$ = createExprNode(ASHR_Expr, $3, $5, NULL); }
/* |  BVPLUS_TOK '(' NUMERAL_TOK ',' Exprs ')'               */
|  BVPLUS_TOK '(' NUMERAL_TOK ',' Expr ',' Expr ')'       { $$ = createExprNode(Plus_Expr, createNumeralNode($3), $5, $7); }
|  BVSUB_TOK '(' NUMERAL_TOK ',' Expr ',' Expr ')'        { $$ = createExprNode(Sub_Expr, createNumeralNode($3), $5, $7); }
|  BVUMINUS_TOK '(' Expr ')'                              { $$ = createExprNode(Uminus_Expr, $3, NULL, NULL); }
|  BVMULT_TOK '(' NUMERAL_TOK ',' Expr ',' Expr ')'       { $$ = createExprNode(Mult_Expr, createNumeralNode($3), $5, $7); }
|  BVDIV_TOK '(' NUMERAL_TOK ',' Expr ',' Expr ')'        { $$ = createExprNode(Div_Expr, createNumeralNode($3), $5, $7); }
|  BVMOD_TOK '(' NUMERAL_TOK ',' Expr ',' Expr ')'        { $$ = createExprNode(Mod_Expr, createNumeralNode($3), $5, $7); }
|  SBVDIV_TOK '(' NUMERAL_TOK ',' Expr ',' Expr ')'       { $$ = createExprNode(SDiv_Expr, createNumeralNode($3), $5, $7); }
|  SBVREM_TOK '(' NUMERAL_TOK ',' Expr ',' Expr ')'       { $$ = createExprNode(SRem_Expr, createNumeralNode($3), $5, $7); }
|  IfExpr                                                 { $$ = $1;}
|  ArrayUpdateExpr                                        { $$ = $1;}
|  LET_TOK LetDecls IN_TOK Expr                           { $$ = createExprNode(Let_Expr, $2, $4, NULL); }
|  STRING_TOK                                             { $$ = createStringNode($1); }
;


/*Grammar for Array Update Expr*/
ArrayUpdateExpr:
   Expr WITH_TOK Updates                                  { $$ = createExprNode(Array_Update_Expr, $1, $3, NULL); }
;


Updates:
   '[' Expr ']' ASSIGN_TOK Expr                           { $$ = createUpdatesNode($2, $5, NULL); }
/* |  Updates WITH_TOK '[' Expr ']' ASSIGN_TOK Expr          { $$ = createUpdatesNode($1, $4, $7); } */
;

/*Grammar for LET Expr*/
LetDecls:
   LetDecl                                  { $$ = $1; }
|  LetDecls ',' LetDecl                     { printf("LetDecls: LetDecls , LetDecl \n"); }
;

LetDecl:
   STRING_TOK '=' Expr                      { $$ = createLetDeclNode(Let_No_Type_Expr, createStringNode($1), $3, NULL); }
|  STRING_TOK ':' Type '=' Expr             { $$ = createLetDeclNode(Let_Type_Expr, createStringNode($1), $3, $5); }
|  STRING_TOK '=' Formula                   { $$ = createLetDeclNode(Let_No_Type_Form, createStringNode($1), $3, NULL); }
|  STRING_TOK ':' Type '=' Formula          { $$ = createLetDeclNode(Let_Type_Form, createStringNode($1), $3, $5); }
;

%%

void yyerror(char *s) {
  printf("line %d: %s\n", yylineno, s);
}

int main(void) {
  yyparse();
  return 0;
}
