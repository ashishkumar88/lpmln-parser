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
#define YYNOCODE 42
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE  token* 
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  token* yy48;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL  datacontainer * container ;
#define ParseARG_PDECL , datacontainer * container 
#define ParseARG_FETCH  datacontainer * container  = yypParser->container 
#define ParseARG_STORE yypParser->container  = container 
#define YYNSTATE             77
#define YYNRULE              59
#define YY_MAX_SHIFT         76
#define YY_MIN_SHIFTREDUCE   116
#define YY_MAX_SHIFTREDUCE   174
#define YY_MIN_REDUCE        175
#define YY_MAX_REDUCE        233
#define YY_ERROR_ACTION      234
#define YY_ACCEPT_ACTION     235
#define YY_NO_ACTION         236
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
#define YY_ACTTAB_COUNT (293)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   235,    1,  145,   75,   41,   16,   19,   64,    5,   58,
 /*    10 */    69,   34,   42,   53,   33,   61,  165,   50,   49,   48,
 /*    20 */   144,   75,   41,   17,   18,   64,    5,   58,   69,   34,
 /*    30 */    42,   53,   33,   61,  165,   50,   49,   48,   47,   17,
 /*    40 */    18,   64,    4,  161,   69,   35,   42,   53,   39,   61,
 /*    50 */   165,  166,   47,  153,   14,   64,    2,  161,   69,   37,
 /*    60 */    42,   53,   36,   61,  165,  170,  153,   47,  158,    7,
 /*    70 */    64,    4,  161,   69,   38,   42,   53,   39,   61,  165,
 /*    80 */   166,   47,   17,   18,   64,    4,  161,   69,   32,   42,
 /*    90 */    53,   39,   61,  165,  166,  202,   70,   14,   76,  171,
 /*   100 */   158,  118,   69,   28,  153,   22,   21,    6,  133,   11,
 /*   110 */    47,  142,    3,   64,  174,  161,   69,   44,   40,  148,
 /*   120 */    43,   61,  165,  153,   47,  141,    6,   64,   20,  161,
 /*   130 */    69,   44,   46,   76,   43,   61,  165,    9,   10,  153,
 /*   140 */    22,   21,    6,  152,   11,  155,   73,    3,   47,  174,
 /*   150 */    18,   64,   13,  161,   69,   59,   60,   47,  153,  131,
 /*   160 */    64,   25,  161,   69,   62,   63,   47,   19,  132,   64,
 /*   170 */    70,  161,   69,  122,    9,  118,   69,  123,  153,   22,
 /*   180 */    21,    6,  133,   11,   12,  140,  164,  153,   22,   21,
 /*   190 */     7,   70,   30,  116,    8,   70,  161,   69,   51,   52,
 /*   200 */   161,   69,   54,   55,   70,  152,   29,  154,   73,  161,
 /*   210 */    69,   56,   57,   70,   24,   16,   19,   70,  161,   69,
 /*   220 */    65,   45,  161,   69,   66,   67,   70,  121,  219,   70,
 /*   230 */    19,  161,   69,  124,  161,   69,  120,   70,    9,   10,
 /*   240 */   159,  121,  161,   69,  119,   17,   18,  153,  157,  149,
 /*   250 */    15,   70,   23,   13,  156,  173,  118,   69,  177,   31,
 /*   260 */    14,  152,  172,   68,   73,  152,  219,   71,   73,  211,
 /*   270 */   152,  153,   72,   73,   26,  153,  152,   27,  150,   73,
 /*   280 */    23,  152,  152,   74,   73,  163,  219,   22,   21,  211,
 /*   290 */   147,   30,  146,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    21,   22,   23,   24,   25,    1,    2,   28,   29,   30,
 /*    10 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*    20 */    23,   24,   25,    1,    2,   28,   29,   30,   31,   32,
 /*    30 */    33,   34,   35,   36,   37,   38,   39,   40,   25,    1,
 /*    40 */     2,   28,   29,   30,   31,   32,   33,   34,   35,   36,
 /*    50 */    37,   38,   25,    9,   16,   28,   29,   30,   31,   32,
 /*    60 */    33,   34,   35,   36,   37,   38,    9,   25,   29,   12,
 /*    70 */    28,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*    80 */    38,   25,    1,    2,   28,   29,   30,   31,   32,   33,
 /*    90 */    34,   35,   36,   37,   38,    0,   25,   16,    3,   18,
 /*   100 */    29,   30,   31,    6,    9,   10,   11,   12,   37,   14,
 /*   110 */    25,    4,   17,   28,   19,   30,   31,   32,   33,   26,
 /*   120 */    35,   36,   37,    9,   25,    4,   12,   28,   14,   30,
 /*   130 */    31,   32,   33,    3,   35,   36,   37,    1,    2,    9,
 /*   140 */    10,   11,   12,   25,   14,   27,   28,   17,   25,   19,
 /*   150 */     2,   28,   16,   30,   31,   32,   33,   25,    9,   36,
 /*   160 */    28,   12,   30,   31,   32,   33,   25,    2,   36,   28,
 /*   170 */    25,   30,   31,   32,    1,   30,   31,   36,    9,   10,
 /*   180 */    11,   12,   37,   14,   16,   18,   13,    9,   10,   11,
 /*   190 */    12,   25,   14,    4,    1,   25,   30,   31,   32,   33,
 /*   200 */    30,   31,   32,   33,   25,   25,   15,   27,   28,   30,
 /*   210 */    31,   32,   33,   25,   12,    1,    2,   25,   30,   31,
 /*   220 */    32,   33,   30,   31,   32,   33,   25,   13,   12,   25,
 /*   230 */     2,   30,   31,   32,   30,   31,   32,   25,    1,    2,
 /*   240 */    13,   13,   30,   31,   32,    1,    2,    9,   13,    7,
 /*   250 */    12,   25,   14,   16,   13,   18,   30,   31,   41,    5,
 /*   260 */    16,   25,   18,   27,   28,   25,   12,   27,   28,   15,
 /*   270 */    25,    9,   27,   28,   12,    9,   25,    8,   27,   28,
 /*   280 */    14,   25,   25,   27,   28,   28,   12,   10,   11,   15,
 /*   290 */     4,   14,    4,
};
#define YY_SHIFT_USE_DFLT (-1)
#define YY_SHIFT_COUNT (76)
#define YY_SHIFT_MIN   (0)
#define YY_SHIFT_MAX   (288)
static const short yy_shift_ofst[] = {
 /*     0 */   130,   95,  169,  169,  169,  169,  114,  114,  238,  238,
 /*    10 */   266,  178,  238,  238,  238,  238,  238,  266,  266,  266,
 /*    20 */    57,  149,  262,   44,   44,   44,   44,   44,   44,   44,
 /*    30 */   277,   97,   81,  237,  244,   38,  136,   38,   38,  136,
 /*    40 */   214,  254,    4,  173,   22,  228,    4,  274,  107,  121,
 /*    50 */   167,  148,  165,  168,  148,  165,  148,  165,  189,  148,
 /*    60 */   165,  193,  148,  165,  191,  148,  148,  165,  227,  202,
 /*    70 */   216,  235,  241,  269,  242,  286,  288,
};
#define YY_REDUCE_USE_DFLT (-22)
#define YY_REDUCE_COUNT (31)
#define YY_REDUCE_MIN   (-21)
#define YY_REDUCE_MAX   (257)
static const short yy_reduce_ofst[] = {
 /*     0 */   -21,   -3,   13,   27,   42,   56,   85,   99,  123,  132,
 /*    10 */   141,   71,  166,  170,  179,  188,  192,  201,  204,  212,
 /*    20 */   145,  118,  180,  226,  236,  240,  245,  251,  256,  257,
 /*    30 */    39,   93,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
 /*    10 */   234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
 /*    20 */   234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
 /*    30 */   234,  234,  234,  234,  234,  226,  228,  227,  234,  234,
 /*    40 */   234,  176,  221,  234,  234,  234,  234,  176,  234,  234,
 /*    50 */   234,  198,  197,  234,  194,  195,  193,  196,  220,  188,
 /*    60 */   189,  234,  184,  186,  234,  234,  187,  185,  234,  234,
 /*    70 */   176,  234,  234,  210,  234,  234,  234,
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
  "COMMA",         "ID",            "EXIST",         "FORALL",      
  "LPAREN",        "RPAREN",        "NOT",           "NOTEQUALS",   
  "IMPLICATION",   "WEIGHT",        "PERIOD",        "FILE_END",    
  "error",         "start",         "statementlist",  "statement",   
  "objectdeclaration",  "identifier",    "declarationbody",  "terms",       
  "term",          "quantifier",    "atom",          "predicate",   
  "literal",       "clause",        "disjunct",      "conjunct",    
  "comparison",    "parenthesizedconjunct",  "implications",  "softrule",    
  "hardrule",    
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "statement ::= atom NEWLINE",
 /*   1 */ "atom ::= identifier",
 /*   2 */ "literal ::= NOT atom",
 /*   3 */ "clause ::= clause DISJUNCTION literal",
 /*   4 */ "clause ::= literal DISJUNCTION literal",
 /*   5 */ "clause ::= LPAREN clause RPAREN",
 /*   6 */ "disjunct ::= conjunct DISJUNCTION literal",
 /*   7 */ "disjunct ::= conjunct DISJUNCTION comparison",
 /*   8 */ "conjunct ::= literal CONJUNCTION literal",
 /*   9 */ "conjunct ::= conjunct CONJUNCTION literal",
 /*  10 */ "conjunct ::= clause CONJUNCTION clause",
 /*  11 */ "conjunct ::= conjunct CONJUNCTION clause",
 /*  12 */ "conjunct ::= clause CONJUNCTION literal",
 /*  13 */ "conjunct ::= comparison CONJUNCTION literal",
 /*  14 */ "conjunct ::= comparison CONJUNCTION clause",
 /*  15 */ "conjunct ::= comparison CONJUNCTION comparison",
 /*  16 */ "conjunct ::= conjunct CONJUNCTION comparison",
 /*  17 */ "conjunct ::= NOT parenthesizedconjunct",
 /*  18 */ "implications ::= literal IMPLICATION literal",
 /*  19 */ "implications ::= conjunct IMPLICATION literal",
 /*  20 */ "implications ::= conjunct IMPLICATION clause",
 /*  21 */ "implications ::= literal IMPLICATION clause",
 /*  22 */ "implications ::= disjunct IMPLICATION clause",
 /*  23 */ "implications ::= disjunct IMPLICATION literal",
 /*  24 */ "hardrule ::= implications PERIOD",
 /*  25 */ "statement ::= softrule NEWLINE",
 /*  26 */ "statement ::= hardrule NEWLINE",
 /*  27 */ "start ::= statementlist",
 /*  28 */ "statementlist ::= statementlist statement",
 /*  29 */ "statementlist ::= statement",
 /*  30 */ "statement ::= COMMENT NEWLINE",
 /*  31 */ "statement ::= objectdeclaration NEWLINE",
 /*  32 */ "objectdeclaration ::= identifier ASSIGN declarationbody",
 /*  33 */ "declarationbody ::= LCURL terms RCURL",
 /*  34 */ "terms ::= term COMMA terms",
 /*  35 */ "terms ::= term",
 /*  36 */ "term ::= identifier",
 /*  37 */ "identifier ::= ID",
 /*  38 */ "quantifier ::= EXIST terms",
 /*  39 */ "quantifier ::= FORALL terms",
 /*  40 */ "quantifier ::= EXIST LPAREN terms RPAREN",
 /*  41 */ "quantifier ::= FORALL LPAREN terms RPAREN",
 /*  42 */ "quantifier ::= NOT quantifier",
 /*  43 */ "atom ::= predicate LPAREN terms RPAREN",
 /*  44 */ "predicate ::= identifier",
 /*  45 */ "literal ::= atom",
 /*  46 */ "disjunct ::= clause",
 /*  47 */ "comparison ::= term NOTEQUALS term",
 /*  48 */ "parenthesizedconjunct ::= LPAREN conjunct RPAREN",
 /*  49 */ "conjunct ::= parenthesizedconjunct",
 /*  50 */ "implications ::= quantifier implications",
 /*  51 */ "softrule ::= WEIGHT quantifier literal",
 /*  52 */ "softrule ::= WEIGHT literal",
 /*  53 */ "softrule ::= WEIGHT conjunct",
 /*  54 */ "softrule ::= WEIGHT implications",
 /*  55 */ "hardrule ::= quantifier literal PERIOD",
 /*  56 */ "hardrule ::= literal PERIOD",
 /*  57 */ "hardrule ::= conjunct PERIOD",
 /*  58 */ "statement ::= FILE_END",
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
  { 30, 1 },
  { 32, 2 },
  { 33, 3 },
  { 33, 3 },
  { 33, 3 },
  { 34, 3 },
  { 34, 3 },
  { 35, 3 },
  { 35, 3 },
  { 35, 3 },
  { 35, 3 },
  { 35, 3 },
  { 35, 3 },
  { 35, 3 },
  { 35, 3 },
  { 35, 3 },
  { 35, 2 },
  { 38, 3 },
  { 38, 3 },
  { 38, 3 },
  { 38, 3 },
  { 38, 3 },
  { 38, 3 },
  { 40, 2 },
  { 23, 2 },
  { 23, 2 },
  { 21, 1 },
  { 22, 2 },
  { 22, 1 },
  { 23, 2 },
  { 23, 2 },
  { 24, 3 },
  { 26, 3 },
  { 27, 3 },
  { 27, 1 },
  { 28, 1 },
  { 25, 1 },
  { 29, 2 },
  { 29, 2 },
  { 29, 4 },
  { 29, 4 },
  { 29, 2 },
  { 30, 4 },
  { 31, 1 },
  { 32, 1 },
  { 34, 1 },
  { 36, 3 },
  { 37, 3 },
  { 35, 1 },
  { 38, 2 },
  { 39, 3 },
  { 39, 2 },
  { 39, 2 },
  { 39, 2 },
  { 40, 3 },
  { 40, 2 },
  { 40, 2 },
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
      case 0: /* statement ::= atom NEWLINE */
#line 63 "lpmln_parser.y"
{
    while(!container->tokens.empty() && container->tokens.top().type == CONSTRUCT_PREDICATE) {
        container->predicates.push_back(container->tokens.top());
        container->tokens.pop();
    }
}
#line 920 "lpmln_parser.c"
        break;
      case 1: /* atom ::= identifier */
#line 71 "lpmln_parser.y"
{
    parsetoken new_parsetoken;
    new_parsetoken.text = yymsp[0].minor.yy48->last;
    new_parsetoken.type = CONSTRUCT_PREDICATE;

    container->tokens.push(new_parsetoken);
}
#line 931 "lpmln_parser.c"
        break;
      case 2: /* literal ::= NOT atom */
      case 17: /* conjunct ::= NOT parenthesizedconjunct */ yytestcase(yyruleno==17);
#line 85 "lpmln_parser.y"
{
    HANDLER_NEGATION()
}
#line 939 "lpmln_parser.c"
        break;
      case 3: /* clause ::= clause DISJUNCTION literal */
      case 4: /* clause ::= literal DISJUNCTION literal */ yytestcase(yyruleno==4);
      case 5: /* clause ::= LPAREN clause RPAREN */ yytestcase(yyruleno==5);
      case 6: /* disjunct ::= conjunct DISJUNCTION literal */ yytestcase(yyruleno==6);
      case 7: /* disjunct ::= conjunct DISJUNCTION comparison */ yytestcase(yyruleno==7);
#line 90 "lpmln_parser.y"
{
    HANDLER_DISJUNCTION()
}
#line 950 "lpmln_parser.c"
        break;
      case 8: /* conjunct ::= literal CONJUNCTION literal */
      case 9: /* conjunct ::= conjunct CONJUNCTION literal */ yytestcase(yyruleno==9);
      case 10: /* conjunct ::= clause CONJUNCTION clause */ yytestcase(yyruleno==10);
      case 11: /* conjunct ::= conjunct CONJUNCTION clause */ yytestcase(yyruleno==11);
      case 12: /* conjunct ::= clause CONJUNCTION literal */ yytestcase(yyruleno==12);
      case 13: /* conjunct ::= comparison CONJUNCTION literal */ yytestcase(yyruleno==13);
      case 14: /* conjunct ::= comparison CONJUNCTION clause */ yytestcase(yyruleno==14);
      case 15: /* conjunct ::= comparison CONJUNCTION comparison */ yytestcase(yyruleno==15);
      case 16: /* conjunct ::= conjunct CONJUNCTION comparison */ yytestcase(yyruleno==16);
#line 116 "lpmln_parser.y"
{
    HANDLER_CONJUNCTION()
}
#line 965 "lpmln_parser.c"
        break;
      case 18: /* implications ::= literal IMPLICATION literal */
      case 19: /* implications ::= conjunct IMPLICATION literal */ yytestcase(yyruleno==19);
      case 20: /* implications ::= conjunct IMPLICATION clause */ yytestcase(yyruleno==20);
      case 21: /* implications ::= literal IMPLICATION clause */ yytestcase(yyruleno==21);
      case 22: /* implications ::= disjunct IMPLICATION clause */ yytestcase(yyruleno==22);
      case 23: /* implications ::= disjunct IMPLICATION literal */ yytestcase(yyruleno==23);
#line 161 "lpmln_parser.y"
{
    HANDLER_IMPLICATION()
}
#line 977 "lpmln_parser.c"
        break;
      case 24: /* hardrule ::= implications PERIOD */
#line 196 "lpmln_parser.y"
{
    HANDLER_ENDOFFORMULA()
}
#line 984 "lpmln_parser.c"
        break;
      case 25: /* statement ::= softrule NEWLINE */
      case 26: /* statement ::= hardrule NEWLINE */ yytestcase(yyruleno==26);
#line 200 "lpmln_parser.y"
{  }
#line 990 "lpmln_parser.c"
        break;
      default:
      /* (27) start ::= statementlist */ yytestcase(yyruleno==27);
      /* (28) statementlist ::= statementlist statement */ yytestcase(yyruleno==28);
      /* (29) statementlist ::= statement */ yytestcase(yyruleno==29);
      /* (30) statement ::= COMMENT NEWLINE */ yytestcase(yyruleno==30);
      /* (31) statement ::= objectdeclaration NEWLINE */ yytestcase(yyruleno==31);
      /* (32) objectdeclaration ::= identifier ASSIGN declarationbody */ yytestcase(yyruleno==32);
      /* (33) declarationbody ::= LCURL terms RCURL */ yytestcase(yyruleno==33);
      /* (34) terms ::= term COMMA terms */ yytestcase(yyruleno==34);
      /* (35) terms ::= term */ yytestcase(yyruleno==35);
      /* (36) term ::= identifier */ yytestcase(yyruleno==36);
      /* (37) identifier ::= ID */ yytestcase(yyruleno==37);
      /* (38) quantifier ::= EXIST terms */ yytestcase(yyruleno==38);
      /* (39) quantifier ::= FORALL terms */ yytestcase(yyruleno==39);
      /* (40) quantifier ::= EXIST LPAREN terms RPAREN */ yytestcase(yyruleno==40);
      /* (41) quantifier ::= FORALL LPAREN terms RPAREN */ yytestcase(yyruleno==41);
      /* (42) quantifier ::= NOT quantifier */ yytestcase(yyruleno==42);
      /* (43) atom ::= predicate LPAREN terms RPAREN */ yytestcase(yyruleno==43);
      /* (44) predicate ::= identifier */ yytestcase(yyruleno==44);
      /* (45) literal ::= atom */ yytestcase(yyruleno==45);
      /* (46) disjunct ::= clause */ yytestcase(yyruleno==46);
      /* (47) comparison ::= term NOTEQUALS term */ yytestcase(yyruleno==47);
      /* (48) parenthesizedconjunct ::= LPAREN conjunct RPAREN */ yytestcase(yyruleno==48);
      /* (49) conjunct ::= parenthesizedconjunct */ yytestcase(yyruleno==49);
      /* (50) implications ::= quantifier implications */ yytestcase(yyruleno==50);
      /* (51) softrule ::= WEIGHT quantifier literal */ yytestcase(yyruleno==51);
      /* (52) softrule ::= WEIGHT literal */ yytestcase(yyruleno==52);
      /* (53) softrule ::= WEIGHT conjunct */ yytestcase(yyruleno==53);
      /* (54) softrule ::= WEIGHT implications */ yytestcase(yyruleno==54);
      /* (55) hardrule ::= quantifier literal PERIOD */ yytestcase(yyruleno==55);
      /* (56) hardrule ::= literal PERIOD */ yytestcase(yyruleno==56);
      /* (57) hardrule ::= conjunct PERIOD */ yytestcase(yyruleno==57);
      /* (58) statement ::= FILE_END */ yytestcase(yyruleno==58);
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
#line 1082 "lpmln_parser.c"
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
#line 1106 "lpmln_parser.c"
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
