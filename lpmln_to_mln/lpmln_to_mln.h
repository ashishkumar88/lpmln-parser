/*
* Author		: Ashish Kumar
* Created		: 04-04-2016
* Version		: 2.0v
* Description		: Header file for the main program
* Modification log	:
*/

#define YYCTYPE char
#define CONSTRUCT_PREDICATE            101
#define CONSTRUCT_CONJUNCTION          102
#define CONSTRUCT_DISJUNCTION          103
#define CONSTRUCT_IMPLICATION          104
#define CONSTRUCT_NEGATION             105
#define CONSTRUCT_CONJUNCT             107
#define CONSTRUCT_DISJUNCT             108
#define END_OF_FORMULA                 109
#define CONSTRUCT_TERM                 110
#define CONSTRUCT_OBJECT               111
#define CONSTRUCT_VARIABLE             112


#include <stack>
#include <list>
#include <string>

// define symbols
#define SYMBOL_CONJUNCTION             "^"
#define SYMBOL_DISJUNCTION             "v"
#define SYMBOL_NEGATION                "!"
#define SYMBOL_PERIOD                  "."
#define SYMBOL_IMPLICATION             "=>"
#define SYMBOL_SPACE                   " "
#define SYMBOL_LPAREN                  "("
#define SYMBOL_RPAREN                  ")"
#define SYMBOL_CONJUNCTION_SPACED      "^ "
#define SYMBOL_DISJUNCTION_SPACED      "v "

// define handler
#define HANDLER_IMPLICATION() parsetoken temp_token = container->tokens.top();\
    container->tokens.pop();\
    typeimplication implication;\
    implication.head.text = temp_token.text;\
    temp_token = container->tokens.top();\
    container->tokens.pop();\
    implication.body.text = temp_token.text;\
    container->rules.push(implication);

#define HANDLER_NEGATION() parsetoken temp_token = container-> tokens.top();\
    container->tokens.pop();\
    temp_token.text = SYMBOL_NEGATION + temp_token.text;\
    container->tokens.push(temp_token);

#define HANDLER_CONJUNCTION() string body = " ";\
    parsetoken temp_token = container->tokens.top();\
    container->tokens.pop();\
    body = SYMBOL_CONJUNCTION_SPACED + temp_token.text + SYMBOL_RPAREN;\
    temp_token = container->tokens.top();\
    container->tokens.pop();\
    body = SYMBOL_LPAREN + temp_token.text + " " + body;\
    parsetoken new_parsetoken;\
    new_parsetoken.text = body;\
    new_parsetoken.type = CONSTRUCT_CONJUNCT;\
    container->tokens.push(new_parsetoken);

#define HANDLER_DISJUNCTION() string body = " ";\
    parsetoken temp_token = container->tokens.top();\
    container->tokens.pop();\
    body = SYMBOL_DISJUNCTION_SPACED  + temp_token.text + SYMBOL_RPAREN;\
    temp_token = container->tokens.top();\
    container->tokens.pop();\
    body = SYMBOL_LPAREN + temp_token.text + " " + body;\
    parsetoken new_parsetoken;\
    new_parsetoken.text = body;\
    new_parsetoken.type = CONSTRUCT_DISJUNCT;\
    container->tokens.push(new_parsetoken);

#define HANDLER_ENDOFFORMULA()     string new_token = "";\
    parsetoken new_parsetoken;\
    new_parsetoken.text = new_token;\
    new_parsetoken.type = END_OF_FORMULA;\
    container->tokens.push(new_parsetoken);

using namespace std;
struct parsetoken { int type; string text; list<parsetoken> tokens; };

class typehead {
    public:
        list<parsetoken> head;
        string text;
        string clauses() {
             return text;
        }
};

class typebody {
    public:
        list<parsetoken> body;
        string text;
        string conjuncts()  {
             return text;
        }
};

class typeimplication {
    public:
        typehead head;
        typebody body;
};

class datacontainer {
    public:
        std::list<parsetoken> predicates;
        std::stack<parsetoken> tokens;
        std::stack<typeimplication> rules;
};

struct token { int type; string text; char * next; string last; int construct_type; };
