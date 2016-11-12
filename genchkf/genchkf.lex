%{

#include "ast.h"
#include "hashstring.h"
#include "genchkf.tab.h"
void yyerror(char *);

%}


%option yylineno
%x    COMMENT
%x    STRING_LITERAL
LETTER    ([a-zA-Z])
HEX       ([0-9a-fA-F])
BITS      ([0-1])
DIGIT     ([0-9])
OPCHAR    (['?\_$])
ANYTHING  ({LETTER}|{DIGIT}|{OPCHAR})

%%

[()[\]{},.;:'!#?_=]  { return *yytext;}

[\n]             { /*Skip new line */ }
[ \t\r\f]        { /* skip whitespace */ }
0b{BITS}+        { yylval.str = string(yytext); return BVCONST_TOK;}
0bin{BITS}+      { yylval.str = string(yytext); return BVCONST_TOK;}
0x{HEX}+         { yylval.str = string(yytext); return BVCONST_TOK;}
0h{HEX}+         { yylval.str = string(yytext); return BVCONST_TOK;}
0hex{HEX}+       { yylval.str = string(yytext); return BVCONST_TOK;}
{DIGIT}+         { yylval.str = string(yytext); return NUMERAL_TOK;}
\'b[0-1]+        { return BIN_BASED_NUMBER;}
\'d[0-9]+        { return DEC_BASED_NUMBER;}
\'h[0-9a-fA-F]+  { return HEX_BASED_NUMBER;}

"%"              { BEGIN COMMENT;}
<COMMENT>"\n"    { BEGIN INITIAL; /* return to normal mode */}
<COMMENT>.       { /* stay in comment mode */}

"ARRAY"          { return ARRAY_TOK; }
"OF"             { return OF_TOK; }
"WITH"           { return WITH_TOK; }
"AND"            { return AND_TOK;}
"NAND"           { return NAND_TOK;}
"NOR"            { return NOR_TOK;}
"NOT"            { return NOT_TOK; }
"EXCEPT"         { return EXCEPT_TOK; }
"OR"             { return OR_TOK; }
"/="             { return NEQ_TOK; }
":="             { return ASSIGN_TOK;}
"=>"             { return IMPLIES_TOK; }
"<=>"            { return IFF_TOK; }
"XOR"            { return XOR_TOK; }
"IF"             { return IF_TOK; }
"THEN"           { return THEN_TOK; }
"ELSE"           { return ELSE_TOK; }
"ELSIF"          { return ELSIF_TOK; }
"END"            { return END_TOK; }
"ENDIF"          { return ENDIF_TOK; }
"BV"             { return BV_TOK;}
"BITVECTOR"      { return BV_TOK;}
"BOOLEAN"        { return BOOLEAN_TOK;}
"<<"             { return BVLEFTSHIFT_TOK;}
">>"             { return BVRIGHTSHIFT_TOK;}
"BVSHL"          { return BVSHL_TOK;}
"BVLSHR"         { return BVLSHR_TOK;}
"BVASHR"         { return BVASHR_TOK;}
"BVPLUS"         { return BVPLUS_TOK;}
"BVSUB"          { return BVSUB_TOK;}
"BVUMINUS"       { return BVUMINUS_TOK;}
"BVMULT"         { return BVMULT_TOK;}
"BVDIV"          { return BVDIV_TOK;}
"BVMOD"          { return BVMOD_TOK;}
"SBVDIV"         { return SBVDIV_TOK;}
"SBVMOD"         { return SBVREM_TOK;}
"SBVREM"         { return SBVREM_TOK;}
"~"              { return BVNEG_TOK;}
"&"              { return BVAND_TOK;}
"|"              { return BVOR_TOK;}
"BVXOR"          { return BVXOR_TOK;}
"BVNAND"         { return BVNAND_TOK;}
"BVNOR"          { return BVNOR_TOK;}
"BVXNOR"         { return BVXNOR_TOK;}
"@"              { return BVCONCAT_TOK;}
"BVLT"           { return BVLT_TOK;}
"BVGT"           { return BVGT_TOK;}
"BVLE"           { return BVLE_TOK;}
"BVGE"           { return BVGE_TOK;}
"BVSLT"          { return BVSLT_TOK;}
"BVSGT"          { return BVSGT_TOK;}
"BVSLE"          { return BVSLE_TOK;}
"BVSGE"          { return BVSGE_TOK;}
"BVSX"           { return BVSX_TOK;}
"BVZX"           { return BVZX_TOK;}
"SBVLT"          { return BVSLT_TOK;}
"SBVGT"          { return BVSGT_TOK;}
"SBVLE"          { return BVSLE_TOK;}
"SBVGE"          { return BVSGE_TOK;}
"SX"             { return BVSX_TOK;}
"ZX"             { return BVZX_TOK;}
"BOOLEXTRACT"    { return BOOLEXTRACT_TOK;}
"BOOLBV"         { return BOOL_TO_BV_TOK;}
"ASSERT"         { return ASSERT_TOK; }
"QUERY"          { return QUERY_TOK; }
"FALSE"          { return FALSELIT_TOK;}
"TRUE"           { return TRUELIT_TOK;}
"IN"             { return IN_TOK;}
"LET"            { return LET_TOK;}
"COUNTEREXAMPLE" { return COUNTEREXAMPLE_TOK;}
"COUNTERMODEL"   { return COUNTEREXAMPLE_TOK;}
"PUSH"           { return PUSH_TOK;}
"POP"            { return POP_TOK;}
.                { yyerror("Illegal input character.");}

(({LETTER})|(_)({ANYTHING}))({ANYTHING})*    { yylval.str = string(yytext); return STRING_TOK; }

%%

int yywrap(void) {
  return 1;
}
