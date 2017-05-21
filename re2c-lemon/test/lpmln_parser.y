// Author		: Ashish Kumar
// Created		: 04-04-16
// Version		: 6.0v
// Description		: Parser program for the LPMLN language
// Modification Log	:
//
//

%include
{
    #include <cassert>
    #include <iostream>
    #include <stdio.h>
    #include "lpmln_to_mln.h"
}

%left CONJUNCTION.
%left DISJUNCTION.

%token_type { token* }

%default_type { token* }

%syntax_error {
    std::cout << "Syntax error!" << std::endl;
}

%parse_accept {
    std::cout << "parsing complete!" << std::endl;
}

%extra_argument { datacontainer * container }

%start_symbol start

// start
start ::= statementlist.
statementlist ::= statementlist statement .
statementlist ::= statement.

// comments
statement ::= COMMENT NEWLINE.

// object declaration
statement ::= objectdeclaration NEWLINE. {
    while(!container->tokens.empty() && container->tokens.top().type == CONSTRUCT_TERM) {
        container->objects.push_back(container->tokens.top());
        container->tokens.pop();
    }
}

objectdeclaration ::= identifier ASSIGN declarationbody.
declarationbody ::= LCURL terms RCURL.

//terms
terms ::= terms COMMA term. 

terms ::= term. 

term ::= identifier(A). {
    parsetoken new_parsetoken;
    new_parsetoken.text = A->last;
    new_parsetoken.type = CONSTRUCT_TERM;
    container->tokens.push(new_parsetoken);
}

term ::= number(A). {
    parsetoken new_parsetoken;
    new_parsetoken.text = A->last;
    new_parsetoken.type = CONSTRUCT_TERM;
    container->tokens.push(new_parsetoken);
}

identifier ::= ID .
number ::= ID_NUMBER.
weight ::= number.

// quantifier
quantifier ::= EXIST terms.
quantifier ::= FORALL terms.
quantifier ::= EXIST LPAREN terms RPAREN.
quantifier ::= FORALL LPAREN terms RPAREN.
quantifier ::= NOT quantifier.

statement ::= atom NEWLINE. {
    while(!container->predicates.empty()) {
        container->predicatelist.push_back(container->predicates.top());
        container->predicates.pop();
        container->treehead.pop();
    }
}

// atom and literal
atom ::= identifier(A). {
    predicate new_predicate;
    new_predicate.setname(A->last);
    container->predicates.push(new_predicate);
    
    expressiontree node;
    node.pr = new_predicate;
    node.type = CONSTRUCT_PREDICATE;
    container->treehead.push(node);
}

atom ::= predicate LPAREN terms RPAREN. {
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

predicate ::= identifier(A). {
    parsetoken new_parsetoken;
    new_parsetoken.text = A->last;
    new_parsetoken.type = CONSTRUCT_PREDICATE;

    container->tokens.push(new_parsetoken);
}

// literal
literal ::= atom.

literal ::= NOT atom. {
    HANDLER_NEGATION()
}

// clause
clause ::= clause DISJUNCTION literal.{
    HANDLER_DISJUNCTION()
}

clause ::= literal DISJUNCTION literal. {
    HANDLER_DISJUNCTION()
}

clause ::= LPAREN clause RPAREN. {
    HANDLER_DISJUNCTION()
}

//disjunct
disjunct ::= clause.
disjunct ::= conjunct DISJUNCTION literal. {
    HANDLER_DISJUNCTION()
}

disjunct ::= conjunct DISJUNCTION comparison. {
    HANDLER_DISJUNCTION()
}

// comparison
comparison ::= term NOTEQUALS term.

// conjunct
conjunct ::= literal CONJUNCTION literal. {
    HANDLER_CONJUNCTION()
}

conjunct ::= conjunct CONJUNCTION literal. {
    HANDLER_CONJUNCTION()
}

conjunct ::= clause CONJUNCTION clause. {
    HANDLER_CONJUNCTION()
}

conjunct ::= conjunct CONJUNCTION clause. {
    HANDLER_CONJUNCTION()
}

conjunct ::= clause CONJUNCTION literal. {
    HANDLER_CONJUNCTION()
}

conjunct ::= comparison CONJUNCTION literal. {
    HANDLER_CONJUNCTION()
}

conjunct ::= comparison CONJUNCTION clause. {
    HANDLER_CONJUNCTION()
}

conjunct ::= comparison CONJUNCTION comparison. {
    HANDLER_CONJUNCTION()
}

conjunct ::= conjunct CONJUNCTION comparison. {
    HANDLER_CONJUNCTION()
}


parenthesizedconjunct ::= LPAREN conjunct RPAREN.

conjunct ::= parenthesizedconjunct.
conjunct ::= NOT parenthesizedconjunct. {
    HANDLER_NEGATION()
}

//implication
implications ::= literal IMPLICATION literal. {
    HANDLER_IMPLICATION()
}

implications ::= conjunct IMPLICATION literal. {
    HANDLER_IMPLICATION()
}

implications ::= conjunct IMPLICATION clause. {
    HANDLER_IMPLICATION()
}

implications ::= literal IMPLICATION clause. {
    HANDLER_IMPLICATION()
}

implications ::= disjunct IMPLICATION clause. {
    HANDLER_IMPLICATION()
}

implications ::= disjunct IMPLICATION literal. {
    HANDLER_IMPLICATION()
}

implications ::= quantifier implications.

// rules
softrule ::= weight quantifier literal. {
    HANDLER_ENDOFFORMULA()
}

softrule ::= weight literal. {
    HANDLER_ENDOFFORMULA()
}

softrule ::= weight conjunct. {
    HANDLER_ENDOFFORMULA()
}

softrule ::= weight implications. {
    HANDLER_ENDOFFORMULA()
}


hardrule ::= quantifier literal PERIOD. {
    HANDLER_ENDOFFORMULA()
}

hardrule ::= literal PERIOD. {
    HANDLER_ENDOFFORMULA()
}

hardrule ::= conjunct PERIOD. {
    HANDLER_ENDOFFORMULA()
}

hardrule ::= implications PERIOD. {
    HANDLER_ENDOFFORMULA()
}

statement ::= softrule NEWLINE. {  }
statement ::= hardrule NEWLINE. {  }
statement ::= FILE_END.
statement ::= NEWLINE.
