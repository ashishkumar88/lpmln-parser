/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 10 "lpmln_parser.y"

    #include <cassert>
    #include <iostream>
    #include <stdio.h>
    #include "lpmln_to_mln.h"
#line 34 "lpmln_parser.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    ParseTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 44
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE  token* 
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  token* yy84;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL  datacontainer * container ;
#define ParseARG_PDECL , datacontainer * container 
#define ParseARG_FETCH  datacontainer * container  = yypParser->container 
#define ParseARG_STORE yypParser->container  = container 
#define YYNSTATE             79
#define YYNRULE              63
#define YY_MAX_SHIFT         78
#define YY_MIN_SHIFTREDUCE   120
#define YY_MAX_SHIFTREDUCE   182
#define YY_MIN_REDUCE        183
#define YY_MAX_REDUCE        245
#define YY_ERROR_ACTION      246
#define YY_ACCEPT_ACTION     247
#define YY_NO_ACTION         248
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE

**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (343)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   247,    1,  161,   77,   41,  167,  168,   68,   76,    3,
 /*    10 */     5,   62,   72,   34,   42,   57,   33,   65,  179,   54,
 /*    20 */    53,   52,  160,   77,   41,   16,   21,   68,   76,    3,
 /*    30 */     5,   62,   72,   34,   42,   57,   33,   65,  179,   54,
 /*    40 */    53,   52,   47,   19,   20,   68,  122,  167,    4,  175,
 /*    50 */    72,   35,   42,   57,   39,   65,  179,  180,   47,   14,
 /*    60 */   153,   68,  122,  174,    2,  175,   72,   37,   42,   57,
 /*    70 */    36,   65,  179,  152,   47,    9,   10,   68,  122,   26,
 /*    80 */     4,  175,   72,   38,   42,   57,   39,   65,  179,  180,
 /*    90 */    47,   13,  155,   68,  122,  163,    4,  175,   72,   32,
 /*   100 */    42,   57,   39,   65,  179,  180,  222,  167,  168,   78,
 /*   110 */   182,    6,  158,   22,  156,  167,  168,   18,   17,    6,
 /*   120 */    47,   11,  157,   68,  122,  181,   20,  175,   72,   44,
 /*   130 */    40,   21,   43,   65,  179,   47,   12,  121,   68,  122,
 /*   140 */   177,  122,  175,   72,   44,   46,   73,   43,   65,  179,
 /*   150 */   123,   78,  182,  175,   72,   55,   56,  167,  168,   18,
 /*   160 */    17,    6,  167,   11,    8,   47,    7,  181,   68,  122,
 /*   170 */   167,  168,  175,   72,   63,   64,   29,   47,  140,  121,
 /*   180 */    68,  122,  165,  122,  175,   72,   66,   67,  167,   47,
 /*   190 */   141,   27,   68,  122,   29,  167,  175,   72,  131,   15,
 /*   200 */    23,   29,  132,  167,  168,   18,   17,    6,  167,   11,
 /*   210 */    18,   17,    7,   73,   30,   73,    9,   19,   20,  174,
 /*   220 */   127,   72,  175,   72,   58,   59,  189,  142,   73,  178,
 /*   230 */    19,   20,   73,   14,  154,  175,   72,   60,   61,  175,
 /*   240 */    72,   69,   45,  167,  168,   73,  121,   24,   74,  166,
 /*   250 */   122,   73,  175,   72,   70,   71,  164,   28,  175,   72,
 /*   260 */   133,  189,  167,  168,  184,   73,   15,  121,   29,   75,
 /*   270 */   166,  122,  175,   72,  129,   73,  167,  168,   28,   73,
 /*   280 */    25,  185,  175,   72,  128,  185,  127,   72,  121,  120,
 /*   290 */    48,  166,  122,  142,  121,  162,   49,  166,  122,  121,
 /*   300 */   185,   50,  166,  122,  185,  121,   73,   51,  166,  122,
 /*   310 */    21,   18,   17,  127,   72,   30,   19,   20,    9,   10,
 /*   320 */    16,   21,  130,  185,   28,  185,  185,   31,  185,  185,
 /*   330 */   125,   28,   14,  130,   13,  189,   28,  173,  184,  185,
 /*   340 */   185,  185,  172,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    21,   22,   23,   24,   25,    9,   10,   28,   29,   30,
 /*    10 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*    20 */    41,   42,   23,   24,   25,    1,    2,   28,   29,   30,
 /*    30 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*    40 */    41,   42,   25,    1,    2,   28,   29,    9,   31,   32,
 /*    50 */    33,   34,   35,   36,   37,   38,   39,   40,   25,   17,
 /*    60 */    18,   28,   29,   31,   31,   32,   33,   34,   35,   36,
 /*    70 */    37,   38,   39,   40,   25,    1,    2,   28,   29,    6,
 /*    80 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*    90 */    25,   17,   18,   28,   29,   26,   31,   32,   33,   34,
 /*   100 */    35,   36,   37,   38,   39,   40,    0,    9,   10,    3,
 /*   110 */     4,   13,    4,   15,   18,    9,   10,   11,   12,   13,
 /*   120 */    25,   15,    4,   28,   29,   19,    2,   32,   33,   34,
 /*   130 */    35,    2,   37,   38,   39,   25,   17,   25,   28,   29,
 /*   140 */    28,   29,   32,   33,   34,   35,   25,   37,   38,   39,
 /*   150 */     4,    3,    4,   32,   33,   34,   35,    9,   10,   11,
 /*   160 */    12,   13,    9,   15,    1,   25,   13,   19,   28,   29,
 /*   170 */     9,   10,   32,   33,   34,   35,   15,   25,   38,   25,
 /*   180 */    28,   29,   28,   29,   32,   33,   34,   35,    9,   25,
 /*   190 */    38,   16,   28,   29,   15,    9,   32,   33,   34,   13,
 /*   200 */    13,   15,   38,    9,   10,   11,   12,   13,    9,   15,
 /*   210 */    11,   12,   13,   25,   15,   25,    1,    1,    2,   31,
 /*   220 */    32,   33,   32,   33,   34,   35,   13,   39,   25,   14,
 /*   230 */     1,    2,   25,   17,   18,   32,   33,   34,   35,   32,
 /*   240 */    33,   34,   35,    9,   10,   25,   25,   13,   27,   28,
 /*   250 */    29,   25,   32,   33,   34,   35,    7,    8,   32,   33,
 /*   260 */    34,   13,    9,   10,   16,   25,   13,   25,   15,   27,
 /*   270 */    28,   29,   32,   33,   34,   25,    9,   10,    8,   25,
 /*   280 */    13,   16,   32,   33,   34,   43,   32,   33,   25,    4,
 /*   290 */    27,   28,   29,   39,   25,    4,   27,   28,   29,   25,
 /*   300 */    43,   27,   28,   29,   43,   25,   25,   27,   28,   29,
 /*   310 */     2,   11,   12,   32,   33,   15,    1,    2,    1,    2,
 /*   320 */     1,    2,   14,   43,    8,   43,   43,    5,   43,   43,
 /*   330 */    14,    8,   17,   14,   17,   13,    8,   14,   16,   43,
 /*   340 */    43,   43,   14,
};
#define YY_SHIFT_USE_DFLT (-5)
#define YY_SHIFT_COUNT (78)
#define YY_SHIFT_MIN   (-4)
#define YY_SHIFT_MAX   (328)
static const short yy_shift_ofst[] = {
 /*     0 */   148,  106,  194,  194,  194,  194,   98,   98,  253,  253,
 /*    10 */   161,  199,  186,  186,  186,  186,  186,  234,  267,  179,
 /*    20 */   179,  179,  153,   -4,   -4,   -4,   -4,   -4,   -4,   38,
 /*    30 */   300,   73,   42,   74,  216,  315,  317,  315,  315,  317,
 /*    40 */   319,  322,   24,  215,  229,  308,   24,  248,  316,  323,
 /*    50 */   328,  249,  108,  118,   96,  124,  129,  119,  124,  129,
 /*    60 */   124,  129,  146,  124,  129,  163,  124,  129,  175,  124,
 /*    70 */   124,  129,  187,  213,  270,  270,  265,  285,  291,
};
#define YY_REDUCE_USE_DFLT (-22)
#define YY_REDUCE_COUNT (31)
#define YY_REDUCE_MIN   (-21)
#define YY_REDUCE_MAX   (281)
static const short yy_reduce_ofst[] = {
 /*     0 */   -21,   -1,   17,   33,   49,   65,   95,  110,  140,  152,
 /*    10 */   164,  188,  121,  190,  203,  207,  220,  221,  242,  226,
 /*    20 */   240,  250,  254,  263,  269,  274,  280,  112,  154,  281,
 /*    30 */    32,   69,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   246,  246,  246,  246,  246,  246,  246,  246,  246,  246,
 /*    10 */   246,  246,  246,  246,  246,  246,  246,  246,  246,  246,
 /*    20 */   246,  246,  246,  246,  246,  246,  246,  246,  246,  246,
 /*    30 */   246,  246,  246,  246,  246,  212,  214,  213,  246,  246,
 /*    40 */   246,  187,  239,  246,  246,  246,  246,  187,  246,  246,
 /*    50 */   246,  246,  246,  246,  246,  211,  210,  246,  207,  208,
 /*    60 */   206,  209,  238,  201,  202,  246,  197,  199,  246,  246,
 /*    70 */   200,  198,  246,  187,  234,  233,  232,  246,  246,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "CONJUNCTION",   "DISJUNCTION",   "COMMENT",     
  "NEWLINE",       "ASSIGN",        "LCURL",         "RCURL",       
  "COMMA",         "ID",            "ID_NUMBER",     "EXIST",       
  "FORALL",        "LPAREN",        "RPAREN",        "NOT",         
  "NOTEQUALS",     "IMPLICATION",   "PERIOD",        "FILE_END",    
  "error",         "start",         "statementlist",  "statement",   
  "objectdeclaration",  "identifier",    "declarationbody",  "terms",       
  "term",          "number",        "weight",        "quantifier",  
  "atom",          "predicate",     "literal",       "clause",      
  "disjunct",      "conjunct",      "comparison",    "parenthesizedconjunct",
  "implications",  "softrule",      "hardrule",    
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "statement ::= objectdeclaration NEWLINE",
 /*   1 */ "term ::= identifier",
 /*   2 */ "term ::= number",
 /*   3 */ "statement ::= atom NEWLINE",
 /*   4 */ "atom ::= identifier",
 /*   5 */ "atom ::= predicate LPAREN terms RPAREN",
 /*   6 */ "predicate ::= identifier",
 /*   7 */ "literal ::= NOT atom",
 /*   8 */ "clause ::= clause DISJUNCTION literal",
 /*   9 */ "clause ::= literal DISJUNCTION literal",
 /*  10 */ "clause ::= LPAREN clause RPAREN",
 /*  11 */ "disjunct ::= conjunct DISJUNCTION literal",
 /*  12 */ "disjunct ::= conjunct DISJUNCTION comparison",
 /*  13 */ "conjunct ::= literal CONJUNCTION literal",
 /*  14 */ "conjunct ::= conjunct CONJUNCTION literal",
 /*  15 */ "conjunct ::= clause CONJUNCTION clause",
 /*  16 */ "conjunct ::= conjunct CONJUNCTION clause",
 /*  17 */ "conjunct ::= clause CONJUNCTION literal",
 /*  18 */ "conjunct ::= comparison CONJUNCTION literal",
 /*  19 */ "conjunct ::= comparison CONJUNCTION clause",
 /*  20 */ "conjunct ::= comparison CONJUNCTION comparison",
 /*  21 */ "conjunct ::= conjunct CONJUNCTION comparison",
 /*  22 */ "conjunct ::= NOT parenthesizedconjunct",
 /*  23 */ "implications ::= literal IMPLICATION literal",
 /*  24 */ "implications ::= conjunct IMPLICATION literal",
 /*  25 */ "implications ::= conjunct IMPLICATION clause",
 /*  26 */ "implications ::= literal IMPLICATION clause",
 /*  27 */ "implications ::= disjunct IMPLICATION clause",
 /*  28 */ "implications ::= disjunct IMPLICATION literal",
 /*  29 */ "softrule ::= weight quantifier literal",
 /*  30 */ "softrule ::= weight literal",
 /*  31 */ "softrule ::= weight conjunct",
 /*  32 */ "softrule ::= weight implications",
 /*  33 */ "hardrule ::= quantifier literal PERIOD",
 /*  34 */ "hardrule ::= literal PERIOD",
 /*  35 */ "hardrule ::= conjunct PERIOD",
 /*  36 */ "hardrule ::= implications PERIOD",
 /*  37 */ "statement ::= softrule NEWLINE",
 /*  38 */ "statement ::= hardrule NEWLINE",
 /*  39 */ "start ::= statementlist",
 /*  40 */ "statementlist ::= statementlist statement",
 /*  41 */ "statementlist ::= statement",
 /*  42 */ "statement ::= COMMENT NEWLINE",
 /*  43 */ "objectdeclaration ::= identifier ASSIGN declarationbody",
 /*  44 */ "declarationbody ::= LCURL terms RCURL",
 /*  45 */ "terms ::= terms COMMA term",
 /*  46 */ "terms ::= term",
 /*  47 */ "identifier ::= ID",
 /*  48 */ "number ::= ID_NUMBER",
 /*  49 */ "weight ::= number",
 /*  50 */ "quantifier ::= EXIST terms",
 /*  51 */ "quantifier ::= FORALL terms",
 /*  52 */ "quantifier ::= EXIST LPAREN terms RPAREN",
 /*  53 */ "quantifier ::= FORALL LPAREN terms RPAREN",
 /*  54 */ "quantifier ::= NOT quantifier",
 /*  55 */ "literal ::= atom",
 /*  56 */ "disjunct ::= clause",
 /*  57 */ "comparison ::= term NOTEQUALS term",
 /*  58 */ "parenthesizedconjunct ::= LPAREN conjunct RPAREN",
 /*  59 */ "conjunct ::= parenthesizedconjunct",
 /*  60 */ "implications ::= quantifier implications",
 /*  61 */ "statement ::= FILE_END",
 /*  62 */ "statement ::= NEWLINE",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to ParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yyidx>=0 );
  yytos = &pParser->yystack[pParser->yyidx--];
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
#ifndef YYPARSEFREENEVERNULL
  if( pParser==0 ) return;
#endif
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static unsigned int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    if( i==YY_SHIFT_USE_DFLT ) return yy_default[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
      if( iLookAhead>0 ){
#ifdef YYFALLBACK
        YYCODETYPE iFallback;            /* Fallback token */
        if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
               && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
          }
#endif
          assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
          iLookAhead = iFallback;
          continue;
        }
#endif
#ifdef YYWILDCARD
        {
          int j = i - iLookAhead + YYWILDCARD;
          if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
            j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
            j<YY_ACTTAB_COUNT &&
#endif
            yy_lookahead[j]==YYWILDCARD
          ){
#ifndef NDEBUG
            if( yyTraceFILE ){
              fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
                 yyTracePrompt, yyTokenName[iLookAhead],
                 yyTokenName[YYWILDCARD]);
            }
#endif /* NDEBUG */
            return yy_action[j];
          }
        }
#endif /* YYWILDCARD */
      }
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   ParseARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yystack[yypParser->yyidx].major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yystack[yypParser->yyidx].major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  ParseTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 23, 2 },
  { 28, 1 },
  { 28, 1 },
  { 23, 2 },
  { 32, 1 },
  { 32, 4 },
  { 33, 1 },
  { 34, 2 },
  { 35, 3 },
  { 35, 3 },
  { 35, 3 },
  { 36, 3 },
  { 36, 3 },
  { 37, 3 },
  { 37, 3 },
  { 37, 3 },
  { 37, 3 },
  { 37, 3 },
  { 37, 3 },
  { 37, 3 },
  { 37, 3 },
  { 37, 3 },
  { 37, 2 },
  { 40, 3 },
  { 40, 3 },
  { 40, 3 },
  { 40, 3 },
  { 40, 3 },
  { 40, 3 },
  { 41, 3 },
  { 41, 2 },
  { 41, 2 },
  { 41, 2 },
  { 42, 3 },
  { 42, 2 },
  { 42, 2 },
  { 42, 2 },
  { 23, 2 },
  { 23, 2 },
  { 21, 1 },
  { 22, 2 },
  { 22, 1 },
  { 23, 2 },
  { 24, 3 },
  { 26, 3 },
  { 27, 3 },
  { 27, 1 },
  { 25, 1 },
  { 29, 1 },
  { 30, 1 },
  { 31, 2 },
  { 31, 2 },
  { 31, 4 },
  { 31, 4 },
  { 31, 2 },
  { 34, 1 },
  { 36, 1 },
  { 38, 3 },
  { 39, 3 },
  { 37, 1 },
  { 40, 2 },
  { 23, 1 },
  { 23, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno        /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( yypParser->yyidx>yypParser->yyidxMax ){
      yypParser->yyidxMax = yypParser->yyidx;
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yyidx>=YYSTACKDEPTH-1 ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yyidx>=yypParser->yystksz-1 ){
      yyGrowStack(yypParser);
      if( yypParser->yyidx>=yypParser->yystksz-1 ){
        yyStackOverflow(yypParser);
        return;
      }
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
      case 0: /* statement ::= objectdeclaration NEWLINE */
#line 45 "lpmln_parser.y"
{
    while(!container->tokens.empty() && container->tokens.top().type == CONSTRUCT_TERM) {
        container->objects.push_back(container->tokens.top());
        container->tokens.pop();
    }
}
#line 938 "lpmln_parser.c"
        break;
      case 1: /* term ::= identifier */
      case 2: /* term ::= number */ yytestcase(yyruleno==2);
#line 60 "lpmln_parser.y"
{
    parsetoken new_parsetoken;
    new_parsetoken.text = yymsp[0].minor.yy84->last;
    new_parsetoken.type = CONSTRUCT_TERM;
    container->tokens.push(new_parsetoken);
}
#line 949 "lpmln_parser.c"
        break;
      case 3: /* statement ::= atom NEWLINE */
#line 85 "lpmln_parser.y"
{
    while(!container->predicates.empty()) {
        container->predicatelist.push_back(container->predicates.top());
        container->predicates.pop();
        container->treehead.pop();
    }
}
#line 960 "lpmln_parser.c"
        break;
      case 4: /* atom ::= identifier */
#line 94 "lpmln_parser.y"
{
    predicate new_predicate;
    new_predicate.setname(yymsp[0].minor.yy84->last);
    container->predicates.push(new_predicate);
    
    expressiontree node;
    node.pr = new_predicate;
    node.type = CONSTRUCT_PREDICATE;
    container->treehead.push(node);
}
#line 974 "lpmln_parser.c"
        break;
      case 5: /* atom ::= predicate LPAREN terms RPAREN */
#line 105 "lpmln_parser.y"
{
    predicate new_predicate;
    
    while(!container->tokens.empty() && container->tokens.top().type == CONSTRUCT_TERM) {
        new_predicate.insertterm(term(container->tokens.top().text));
        container->tokens.pop();
    }

    if(!container->tokens.empty() && container->tokens.top().type == CONSTRUCT_PREDICATE) {
        new_predicate.setname(container->tokens.top().text);
        container->tokens.pop();
    }
    container->predicates.push(new_predicate);

    expressiontree node;
    node.pr = new_predicate;
    node.type = CONSTRUCT_PREDICATE;
    container->treehead.push(node);
}
#line 997 "lpmln_parser.c"
        break;
      case 6: /* predicate ::= identifier */
#line 125 "lpmln_parser.y"
{
    parsetoken new_parsetoken;
    new_parsetoken.text = yymsp[0].minor.yy84->last;
    new_parsetoken.type = CONSTRUCT_PREDICATE;

    container->tokens.push(new_parsetoken);
}
#line 1008 "lpmln_parser.c"
        break;
      case 7: /* literal ::= NOT atom */
      case 22: /* conjunct ::= NOT parenthesizedconjunct */ yytestcase(yyruleno==22);
#line 136 "lpmln_parser.y"
{
    HANDLER_NEGATION()
}
#line 1016 "lpmln_parser.c"
        break;
      case 8: /* clause ::= clause DISJUNCTION literal */
      case 9: /* clause ::= literal DISJUNCTION literal */ yytestcase(yyruleno==9);
      case 10: /* clause ::= LPAREN clause RPAREN */ yytestcase(yyruleno==10);
      case 11: /* disjunct ::= conjunct DISJUNCTION literal */ yytestcase(yyruleno==11);
      case 12: /* disjunct ::= conjunct DISJUNCTION comparison */ yytestcase(yyruleno==12);
#line 141 "lpmln_parser.y"
{
    HANDLER_DISJUNCTION()
}
#line 1027 "lpmln_parser.c"
        break;
      case 13: /* conjunct ::= literal CONJUNCTION literal */
      case 14: /* conjunct ::= conjunct CONJUNCTION literal */ yytestcase(yyruleno==14);
      case 15: /* conjunct ::= clause CONJUNCTION clause */ yytestcase(yyruleno==15);
      case 16: /* conjunct ::= conjunct CONJUNCTION clause */ yytestcase(yyruleno==16);
      case 17: /* conjunct ::= clause CONJUNCTION literal */ yytestcase(yyruleno==17);
      case 18: /* conjunct ::= comparison CONJUNCTION literal */ yytestcase(yyruleno==18);
      case 19: /* conjunct ::= comparison CONJUNCTION clause */ yytestcase(yyruleno==19);
      case 20: /* conjunct ::= comparison CONJUNCTION comparison */ yytestcase(yyruleno==20);
      case 21: /* conjunct ::= conjunct CONJUNCTION comparison */ yytestcase(yyruleno==21);
#line 167 "lpmln_parser.y"
{
    HANDLER_CONJUNCTION()
}
#line 1042 "lpmln_parser.c"
        break;
      case 23: /* implications ::= literal IMPLICATION literal */
      case 24: /* implications ::= conjunct IMPLICATION literal */ yytestcase(yyruleno==24);
      case 25: /* implications ::= conjunct IMPLICATION clause */ yytestcase(yyruleno==25);
      case 26: /* implications ::= literal IMPLICATION clause */ yytestcase(yyruleno==26);
      case 27: /* implications ::= disjunct IMPLICATION clause */ yytestcase(yyruleno==27);
      case 28: /* implications ::= disjunct IMPLICATION literal */ yytestcase(yyruleno==28);
#line 212 "lpmln_parser.y"
{
    HANDLER_IMPLICATION()
}
#line 1054 "lpmln_parser.c"
        break;
      case 29: /* softrule ::= weight quantifier literal */
      case 30: /* softrule ::= weight literal */ yytestcase(yyruleno==30);
      case 31: /* softrule ::= weight conjunct */ yytestcase(yyruleno==31);
      case 32: /* softrule ::= weight implications */ yytestcase(yyruleno==32);
      case 33: /* hardrule ::= quantifier literal PERIOD */ yytestcase(yyruleno==33);
      case 34: /* hardrule ::= literal PERIOD */ yytestcase(yyruleno==34);
      case 35: /* hardrule ::= conjunct PERIOD */ yytestcase(yyruleno==35);
      case 36: /* hardrule ::= implications PERIOD */ yytestcase(yyruleno==36);
#line 239 "lpmln_parser.y"
{
    HANDLER_ENDOFFORMULA()
}
#line 1068 "lpmln_parser.c"
        break;
      case 37: /* statement ::= softrule NEWLINE */
      case 38: /* statement ::= hardrule NEWLINE */ yytestcase(yyruleno==38);
#line 272 "lpmln_parser.y"
{  }
#line 1074 "lpmln_parser.c"
        break;
      default:
      /* (39) start ::= statementlist */ yytestcase(yyruleno==39);
      /* (40) statementlist ::= statementlist statement */ yytestcase(yyruleno==40);
      /* (41) statementlist ::= statement */ yytestcase(yyruleno==41);
      /* (42) statement ::= COMMENT NEWLINE */ yytestcase(yyruleno==42);
      /* (43) objectdeclaration ::= identifier ASSIGN declarationbody */ yytestcase(yyruleno==43);
      /* (44) declarationbody ::= LCURL terms RCURL */ yytestcase(yyruleno==44);
      /* (45) terms ::= terms COMMA term */ yytestcase(yyruleno==45);
      /* (46) terms ::= term */ yytestcase(yyruleno==46);
      /* (47) identifier ::= ID */ yytestcase(yyruleno==47);
      /* (48) number ::= ID_NUMBER */ yytestcase(yyruleno==48);
      /* (49) weight ::= number */ yytestcase(yyruleno==49);
      /* (50) quantifier ::= EXIST terms */ yytestcase(yyruleno==50);
      /* (51) quantifier ::= FORALL terms */ yytestcase(yyruleno==51);
      /* (52) quantifier ::= EXIST LPAREN terms RPAREN */ yytestcase(yyruleno==52);
      /* (53) quantifier ::= FORALL LPAREN terms RPAREN */ yytestcase(yyruleno==53);
      /* (54) quantifier ::= NOT quantifier */ yytestcase(yyruleno==54);
      /* (55) literal ::= atom */ yytestcase(yyruleno==55);
      /* (56) disjunct ::= clause */ yytestcase(yyruleno==56);
      /* (57) comparison ::= term NOTEQUALS term */ yytestcase(yyruleno==57);
      /* (58) parenthesizedconjunct ::= LPAREN conjunct RPAREN */ yytestcase(yyruleno==58);
      /* (59) conjunct ::= parenthesizedconjunct */ yytestcase(yyruleno==59);
      /* (60) implications ::= quantifier implications */ yytestcase(yyruleno==60);
      /* (61) statement ::= FILE_END */ yytestcase(yyruleno==61);
      /* (62) statement ::= NEWLINE */ yytestcase(yyruleno==62);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    yypParser->yyidx -= yysize - 1;
    yymsp -= yysize-1;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
    yypParser->yyidx -= yysize;
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  ParseTOKENTYPE yyminor         /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 24 "lpmln_parser.y"

    std::cout << "Syntax error!" << std::endl;
#line 1158 "lpmln_parser.c"
/************ End %syntax_error code ******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
#line 28 "lpmln_parser.y"

    std::cout << "parsing complete!" << std::endl;
#line 1182 "lpmln_parser.c"
/*********** End %parse_accept code *******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  unsigned int yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      yyStackOverflow(yypParser);
      return;
    }
#endif
    yypParser->yyidx = 0;
#ifndef YYNOERRORRECOVERY
    yypParser->yyerrcnt = -1;
#endif
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sInitialize. Empty stack. State 0\n",
              yyTracePrompt);
    }
#endif
  }
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      if( yyact > YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
#ifndef NDEBUG
  if( yyTraceFILE ){
    int i;
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE,"%c%s", i==1 ? '[' : ' ', 
              yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
