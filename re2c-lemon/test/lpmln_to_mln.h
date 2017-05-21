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
#define SYMBOL_COMMA                   ","
#define SYMBOL_PERIOD                  "."
#define SYMBOL_IMPLICATION             "=>"
#define SYMBOL_SPACE                   " "
#define SYMBOL_LPAREN                  "("
#define SYMBOL_RPAREN                  ")"
#define SYMBOL_CONJUNCTION_SPACED      "^ "
#define SYMBOL_DISJUNCTION_SPACED      "v "

// define atoms
#define ATOM_TYPE_TERM                 201
#define ATOM_TYPE_PREDICATE            202
#define ATOM_TYPE_NEGATION             203
#define ATOM_TYPE_LPAREN               204
#define ATOM_TYPE_RPAREN               205
#define ATOM_TYPE_CONJUNCTION          206
#define ATOM_TYPE_DISJUNCTION          207
#define ATOM_TYPE_COMMA                208

// define handler
#define HANDLER_IMPLICATION() expressiontree headnode = container->treehead.top();\
    container->treehead.pop();\
    typeimplication implication;\
    implication.headhead = &headnode;\
    expressiontree bodynode = container->treehead.top();\
    cout << headnode.inorder_traversal();\
    container->treehead.pop(); \
    cout << bodynode.inorder_traversal();\
    implication.bodyhead = &bodynode;

#define HANDLER_NEGATION() expressiontree node = container->treehead.top();\
    container->treehead.pop();\
    predicate* temp_predicate = &node.pr;\
    temp_predicate->setnegation(true);\
    container->treehead.push(node);

#define HANDLER_CONJUNCTION() expressiontree rightnode = container->treehead.top(); \
    expressiontree currentnode;\
    currentnode.opr = lpmlnoperator(SYMBOL_CONJUNCTION);\
    currentnode.type = CONSTRUCT_CONJUNCTION;\
    currentnode.right = &rightnode;\
    container->treehead.pop();\
    expressiontree leftnode = container->treehead.top();\
    currentnode.left = &leftnode;\
    container->treehead.pop();\
    container->treehead.push(currentnode);

#define HANDLER_DISJUNCTION() expressiontree rightnode = container->treehead.top();\
    expressiontree currentnode;\
    currentnode.opr = lpmlnoperator(SYMBOL_DISJUNCTION);\
    currentnode.type = CONSTRUCT_DISJUNCTION;\
    currentnode.right = &rightnode;\
    container->treehead.pop();\
    expressiontree leftnode = container->treehead.top();\
    currentnode.left = &leftnode;\
    container->treehead.pop();\
    container->treehead.push(currentnode);

#define HANDLER_ENDOFFORMULA() string new_token = "";\
    parsetoken new_parsetoken;\
    new_parsetoken.text = new_token;\
    new_parsetoken.type = END_OF_FORMULA;\
    container->tokens.push(new_parsetoken);

using namespace std;

class variable {
    private:
        int counter;
        int index;
        char vars[6];

    public:
       variable() {
           counter = 0;
           index = 0;
           vars[0] = 'u';
           vars[1] = 'v';
           vars[2] = 'w';
           vars[3] = 'x';
           vars[4] = 'y';
           vars[5] = 'z';
       }

       string next_var(bool reset) {
          if(index % sizeof(vars) == 0) {
              counter++;
              index = 0;
          }

          if(reset) {
              counter = 1;
              index = 0;
          }

          string var = string(counter, vars[index++]);
          return var; 
       }
};

class term {
    private:
        string text;

    public:
        void settext(string _text) {
            text = _text;
        }

        string gettext() {
            return text;
        }
        term() {
        }
  
        term(string _text) {
            text = text;
        }
};

class predicate {
    private:
        bool negation;
        string name;
        list<term> terms;
        variable var;

    public:
        void setnegation(bool flag) {
            negation = flag;
        }

        bool isnegation() {
            return negation;
        }

        void setname(string text) {
            name = text;
        }

        string getname() {
            return name;
        }
       
        void insertterm(term t) {
            terms.push_back(t);
        }

        string nameandvariable() {
            string value = name ;
            if(terms.size() != 0) {
                 value += SYMBOL_LPAREN;
                 for(int i = 0; i < terms.size(); i++) {
                     if(i == 0) {
                         value += var.next_var(false);
                     } else {
                         value += SYMBOL_COMMA + var.next_var(false);
                     }
                 }
                 value += SYMBOL_RPAREN;
            }
            return value;
        }
};

class parsetoken {
    public:
        int type;
        string text;
};

class lpmlnoperator {
    private:
        string opr;

    public:
        lpmlnoperator() {
        }

        lpmlnoperator(string opr_type) {
            opr = opr_type;
        }

        string getopr() {
            return opr;
        }
};

class expressiontree {

    public:
        lpmlnoperator opr;
        predicate pr;
        term tr;
        int type;
         
        expressiontree *left, *right;
        expressiontree() {
            left = NULL;
            right = NULL;
        }

        string inorder_traversal() {
            string inorder;
            if(left && right) {
                inorder = SYMBOL_LPAREN;
                inorder += left->inorder_traversal();
                switch(type) {
                    case CONSTRUCT_DISJUNCTION:
                                               inorder += opr.getopr(); break;
                    case CONSTRUCT_CONJUNCTION:
                                               inorder += opr.getopr(); break;
                    case CONSTRUCT_PREDICATE:
                                               inorder += pr.getname(); break;
                }

                inorder += right->inorder_traversal();
                inorder += SYMBOL_RPAREN;
            } else {
                switch(type) {
                    case CONSTRUCT_DISJUNCTION:
                                               return opr.getopr(); 
                    case CONSTRUCT_CONJUNCTION:
                                               return opr.getopr();
                    case CONSTRUCT_PREDICATE: 
                                               return pr.getname();
                }
            }
        }
        
        bool search(string src) {
            int cmp;
            switch(type) {
                case CONSTRUCT_DISJUNCTION:
                                           cmp = opr.getopr().compare(src);
                case CONSTRUCT_CONJUNCTION:
                                           cmp = opr.getopr().compare(src);
                case CONSTRUCT_PREDICATE:
                                           cmp = pr.getname().compare(src);
            }
            if(cmp >= 0) {
                return true;
            } else if(left->search(src)) {
                return true;
            } else if(right->search(src)) {
                return true;
            } else { 
                return false;
            }
        }

};

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
        expressiontree *headhead;
        typebody body;
        expressiontree *bodyhead;
};

class datacontainer {
    public:
        std::stack<predicate> predicates;
        std::list<predicate> predicatelist;
        std::list<parsetoken> objects;
        std::stack<parsetoken> tokens;
        std::stack<typeimplication> rules;
        std::vector<typeimplication> ruleslist;
        std::stack<expressiontree> treehead;
};

struct token { int type; string text; char * next; string last; int construct_type; };
