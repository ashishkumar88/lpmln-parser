/*
* Author		: Ashish Kumar
* Created		: 04-04-2016
* Version		: 6.0v
* Description		: The lexer program
* Modification log	:
*/

#include <ctype.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <cstring>
#include "lpmln_parser.h"
#define WHITESPACE 25

template<typename char_t>
struct input_t {
    size_t len;
    char_t *str;

    input_t(FILE *f) : len(0), str(NULL)
    {
        fseek(f, 0, SEEK_END);
        len = ftell(f) / sizeof(char_t);
        fseek(f, 0, SEEK_SET);
        str = new char_t[len + 1];
        fread(str, sizeof(char_t), len, f);
        str[len] = 0;
    }
    ~input_t()
    {
        delete[]str;
    }
};

typedef input_t<YYCTYPE>  fio;
using namespace std;

token next_token(YYCTYPE * str){
    YYCTYPE * YYCURSOR = str;
    YYCTYPE * YYMARKER = 0;

        YYCTYPE * start = YYCURSOR;
        token new_token;
        /*!re2c
                        re2c:yyfill:enable = 0;

                        (["/"]{2}[^(\r|\n)]+) { new_token.type = COMMENT; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token;}

                        ("FORALL") { new_token.type = FORALL; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token;}
                        ("EXIST") { new_token.type = EXIST; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token;}

                        ("v ") { new_token.type = DISJUNCTION; new_token.text = strndup(start, YYCURSOR - start - 1); new_token.next = YYCURSOR - 1; return new_token; }
                        ([0-9]+("\.")?[0-9]*) { new_token.type = ID_NUMBER; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token;}
                        ([a-zA-Z_][a-zA-Z_0-9_-]*) { new_token.type = ID; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token;}
                        ("=") { new_token.type = ASSIGN; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR;  return new_token;}
                        ("=>") { new_token.type = IMPLICATION; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token; }
                        ("!=") { new_token.type = NOTEQUALS; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token; }
                        ("!") { new_token.type = NOT; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token; }
                        ("^") { new_token.type = CONJUNCTION; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token; }

                        ("\.") { new_token.type = PERIOD; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token; }
                        (",") { new_token.type = COMMA; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token; }
                        ("(") { new_token.type = LPAREN; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token; }
                        (")") { new_token.type = RPAREN; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token; }
                        ("{") { new_token.type = LCURL; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token; }
                        ("}") { new_token.type = RCURL; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token; }
                        [ \t]+ { new_token.type = WHITESPACE; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token; }
                        [\r\n]+ { new_token.type = NEWLINE; new_token.text = strndup(start, YYCURSOR - start); new_token.next = YYCURSOR; return new_token; }
                        "\x00" { new_token.type = FILE_END; new_token.text = "\x00"; *new_token.next = '\0'; return new_token; return new_token;}
                        * { new_token.type = WHITESPACE; new_token.text = " "; new_token.next = YYCURSOR; return new_token;}

        */
        return new_token;
}
