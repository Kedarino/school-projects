#include "codegen.h"
#include <stdio.h>

static int ifLabelCounter = 0;

void generateCode(const Node *node) {
    if (node == NULL) return;

    switch (node->type) {
        case INT_NODE:
            printf("PUSHS int@%d\n", node->data.int_lit);
            break;
        case STRING_NODE:
            printf("PUSHS string@%s\n", node->data.string_lit->data);
            break;
        case DOUBLE_NODE:
            printf("PUSHS float@%a\n", node->data.double_lit);
            break;
        case IDENT_NODE:
            //printf("PUSHS GF@%s\n", node->data.string_lit->data); // tohle idk
            break;
        case TERM_NODE:
            generateTerm(node); // Detailní implementace pro TERM_NODE
            break;
        case EXPR_NODE:
            generateExpression(node); // Detailní implementace pro EXPR_NODE
            break;
        case EXPR_PAREN_NODE:
            generateParenExpression(node); // Detailní implementace pro EXPR_PAREN_NODE
            break;
        case RETURN_NODE:
            printf("RETURN\n");
            break;
        case TYPE_NODE:
            generateType(node); // Detailní implementace pro TYPE_NODE
            break;
        case VAR_NODE:
        case LET_NODE:
        case DEF_NODE:
            printf("DEFVAR LF@%s\n", node->data.string_lit->data); // Předpokládáme local frame
            break;
        case LESS_OR_EQUAL_NODE:
        case GREATER_OR_EQUAL_NODE:
        case LESS_THAN_NODE:
        case GREATER_THAN_NODE:
        case NOT_EQUAL_NODE:
        case EQUAL_NODE:
            generateLogicalExpression(node); // Detailní implementace pro logické výrazy
            bre
        case WHILE_NODE:
            generateWhileLoop(node); // Detailní implementace pro WHILE_NODE
            break;
        case SCOPE_NODE:
            generateScope(node); // Detailní implementace pro SCOPE_NODE
            break;
        case IF_NODE:
        case IF_SCOPE_NODE:
            generateIfStatement(node); // Detailní implementace pro IF_NODE
            break;
        case PARAMETR_NODE:
            generateParameter(node); // Detailní implementace pro PARAMETR_NODE
            break;
        case PAR_NAME_NODE:
            generateParameterName(node); // Detailní implementace pro PAR_NAME_NODE
            break;
        case PAR_IDENT_NODE:
            generateParameterIdentifier(node); // Detailní implementace pro PAR_IDENT_NODE
            break;
        case FUNC_NODE:
/**/         generateFunction(node); // Detailní implementace pro FUNC_NODE
            break;
        case FUNC_CALL_NODE:
            generateBuiltin(node); // Specifická implementace pro volání vestavěných funkcí
            break;
        case IF_LET_NODE:
            generateIfLet(node); // Detailní implementace pro IF_LET_NODE
            break;
        case INT2DOUBLE:
            printf("INT2FLOAT LF@temp TF@%s\n", node->data.string_lit->data); // Předpokládáme dočasný rámec
            break;
        case EXCL_MARK_NODE:
            generateExclMark(node); // Detailní implementace pro EXCL_MARK_NODE
            break;
        case DOUBLE_QUEST_MARK_NODE:
            generateDoubleQuestMark(node); // Detailní implementace pro DOUBLE_QUEST_MARK_NODE
            break;
        case NIL_NODE:
            printf("MOVE LF@temp nil@\n"); // Předpokládáme lokalní rámec
            break;
        case STMT_NODE:
            generateCode(node->left);
            generateCode(node->right);
            break;
        default:
            printf("# Neznámý typ uzlu: %d\n", node->type);
    }
}


void generateFunction(const Node *funcNode) {
    if (funcNode == NULL || funcNode->type != FUNC_NODE) return;

    // Předpokládá se, že jméno funkce je uloženo v node->data.string_lit
    printf("LABEL %s\n", funcNode->data.string_lit->data);

    printf("PUSHFRAME\n"); // Přesune TF na zásobník rámců a stane se LF

    // Prochází tělo funkce, které je levým potomkem 'STMT_NODE' pod 'FUNC_NODE'
    Node *bodyNode = funcNode->left; // Přechod na 'STMT_NODE' pod 'FUNC_NODE'
    while (bodyNode != NULL) {
        if (bodyNode->type != PARAMETR_NODE) {
            generateCode(bodyNode); // Generuje kód pro aktuální uzel
        }
        bodyNode = bodyNode->right; // Přechod na další 'STMT_NODE' v těle
    }

    printf("POPFRAME\n"); // Vyjme aktuální rámec zpět do dočasného při návratu z funkce
    printf("RETURN\n"); // Návrat na pozici uloženou instrukcí CALL
}




void generateIfStatement(const Node *ifNode) {
    if (ifNode == NULL || ifNode->type != IF_NODE) return;

    int currentLabel = ifLabelCounter++;

    if (ifNode->left != NULL && ifNode->left->type == IF_LET_NODE) {
        // Zpracování IF_LET_NODE
        Node *identNode = ifNode->left->left; // Přímý přístup k IDENT_NODE

        // Generování kódu pro vyhodnocení podmínky
        printf("PUSHS LF@%s\n", identNode->data.string_lit->data); // Předpokládá se, že identifikátor je uložen v data.string_lit

        // Generování skoku
        printf("JUMPIFEQ $else_%p nil@nil GF@_cond\n", currentLabel);

        // Generování kódu pro sekvenci příkazů, pokud je podmínka pravdivá
        generateCode(ifNode->right->left); // IF_SCOPE_NODE pro pravdivý případ

        // Skok na konec if-else bloku
        printf("JUMP $endif_%p\n", currentLabel);
        printf("LABEL $else_%p\n", currentLabel);

        // Generování kódu pro sekvenci příkazů, pokud je podmínka nepravdivá
        generateCode(ifNode->right->right); // ELSE_SCOPE_NODE pro nepravdivý případ
    } else {
        // Zpracování běžného IF_NODE
        generateCode(ifNode->left); // Vyhodnotí podmínku

        // Generování skoku
        printf("JUMPIFEQ $else_%p bool@false GF@_cond\n", currentLabel);

        // Generování kódu pro sekvenci příkazů, pokud je podmínka pravdivá
        generateCode(ifNode->right->left); // IF_SCOPE_NODE pro pravdivý případ

        // Skok na konec if-else bloku
        printf("JUMP $endif_%p\n", currentLabel);
        printf("LABEL $else_%p\n", currentLabel);

        // Generování kódu pro sekvenci příkazů, pokud je podmínka nepravdivá
        generateCode(ifNode->right->right); // ELSE_SCOPE_NODE pro nepravdivý případ
    }

    // Označení konce if-else bloku
    printf("LABEL $endif_%p\n", currentLabel);
}









void generateLogicalExpression(const Node *Node) {
    if (node == NULL) return;

    switch (node->type) {
        case LESS_OR_EQUAL_NODE:
            printf("PUSHFRAME\n");

            generateCode(node->left); //x
            printf("DEFVAR LF@x\n");
            printf("POPS LF@x\n");

            generateCode(node->right); //y
            printf("DEFVAR LF@y\n");
            printf("POPS LF@y\n");

            printf("DEFVAR LF@eqBool\n");
            printf("DEFVAR LF@ltBool\n");

            // x < y
            printf("LT LF@ltBool LF@x LF@\n");
            // x = y
            printf("EQ LF@eqBool LF@%s LF@cond\n");
            // x <= y
            printf("OR LF@bool LF@eqBool LF@ltBool\n");
            printf("POPFRAME\n");
            break;

        case GREATER_OR_EQUAL_NODE:
            printf("PUSHFRAME\n");
            generateCode(node->left); //x
            printf("DEFVAR LF@x\n");
            printf("POPS LF@x\n");
            generateCode(node->right); //y
            printf("DEFVAR LF@y\n");
            printf("POPS LF@y\n");

            
            printf("DEFVAR LF@eqBool\n");
            printf("DEFVAR LF@gtBool\n");
        
            //x > y
            printf("GT LF@gtBool LF@x LF@y\n");
            //x = y
            printf("EQ LF@eqBool LF@x LF@y\n");
            //x >= y
            printf("OR LF@bool LF@eqBool LF@gtBool\n");
            printf("POPFRAME\n");
            // výsledek x >= y je v TF@bool
            break;

        case LESS_THAN_NODE:
            printf("PUSHFRAME\n");
            generateCode(node->left); //x
            printf("DEFVAR LF@x\n");
            printf("POPS LF@x\n");
            generateCode(node->right); //y
            printf("DEFVAR LF@y\n");
            printf("POPS LF@y\n");
            
            
            // x < y
            printf("LT LF@bool LF@x LF@y\n"); 
            
            printf("POPFRAME\n");
            // výsledek x > y je v TF@bool 
            break;

        case GREATER_THAN_NODE:
            printf("PUSHFRAME\n");
            generateCode(node->left); //x
            printf("DEFVAR LF@x\n");
            printf("POPS LF@x\n");
            generateCode(node->right); //y
            printf("DEFVAR LF@y\n");
            printf("POPS LF@y\n");
            
            
            // x < y
            printf("GT LF@bool LF@x LF@y\n"); 
            
            printf("POPFRAME\n");
            // výsledek x > y je v TF@bool 
            break;
       
        case NOT_EQUAL_NODE:
             // x != int
            printf("PUSHFRAME\n");

            generateCode(node->left); //x
            printf("DEFVAR LF@x\n");
            printf("POPS LF@x\n");

            generateCode(node->right); //y
            printf("DEFVAR LF@y\n");
            printf("POPS LF@y\n");

            // x != y
            printf("EQ LF@bool LF@x LF@y\n");
            printf("NOT LF@bool LF@bool\n");       
            printf("POPFRAME\n");
            // výsledek x != int je v bool 
            break;
        case EQUAL_NODE:
             // x = int
            printf("PUSHFRAME");
            generateCode(node->left); //x
            printf("DEFVAR LF@x\n");
            printf("POPS LF@x\n");

            generateCode(node->right); //y
            printf("DEFVAR LF@y\n");
            printf("POPS LF@y\n");

            // x = y
            printf("EQ LF@bool LF@x LF@y\n") 
            printf("POPFRAME\n");
            // výsledek x != int je v bool 
            break;
        default:
            printf("# Neznámý typ logického uzlu: %d\n", node->type);
    }
}









void generateType(const Node *node) {
    if (node == NULL || node->type != TYPE_NODE) return;

    switch (node->data.type) {
        case INT_TYPE:
            printf("int\n"); // For integer type
            break;
        case INT_NULL_TYPE:
            printf("int?\n"); // For nullable integer type
            break;
        case DOUBLE_TYPE:
            printf("float\n"); // For double type
            break;
        case DOUBLE_NULL_TYPE:
            printf("float?\n"); // For nullable double type
            break;
        case STRING_TYPE:
            printf("string\n"); // For string type
            break;
        case STRING_NULL_TYPE:
            printf("string?\n"); // For nullable string type
            break;
        case TRU:
            printf("bool@true\n"); // For boolean true
            break;
        case FAL:
            printf("bool@false\n"); // For boolean false
            break;
        case VOID_TYPE:
            printf("void\n"); // For void type
            break;
        case TERM_INT:
            printf("term_int\n"); // For term integer (specific use case)
            break;
        case TERM_DOUBLE:
            printf("term_float\n"); // For term double (specific use case)
            break;
        case TERM_STRING:
            printf("term_string\n"); // For term string (specific use case)
            break;
        case NIL:
            printf("nil\n"); // For nil type
            break;
        default:
            printf("# Unknown type\n");
    }
}





























































/*
void generateFunction(const Node *node) {
    if (node == NULL || node->type != FUNC_NODE) return;

    const char *functionName = getFunctionName(node);
    printf("LABEL %s\n", functionName);
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    // Generate code for parameters
    Node *currentNode = node->left;
    while (currentNode != NULL && currentNode->type == PARAMETR_NODE) {
        generateParameter(currentNode);
        currentNode = currentNode->right;  // Assuming parameters are linked through the right node
    }

    // Generate code for the function body
    if (currentNode != NULL && currentNode->type == STMT_NODE) {
        generateFunctionBody(currentNode);
    }

    printf("POPFRAME\n");
    printf("RETURN\n");
}
*/
/*

void generateParameter(const Node *node) {
    if (node == NULL || node->type != PARAMETR_NODE) return;

    // Extract the parameter name and identifier


    const char *paramName = node->left->left->data.string_val; 
    
    if (currentNode != NULL && node->left->left->type == TERM_NODE) {
        generateFunctionBody(currentNode);
    }

    const char *paramName = node->left->data.string_val; // PAR_NAME_NODE value
    const char *paramIdent = node->right->data.string_val; // PAR_IDENT_NODE value

    // Type extraction from the type node
    Type paramType = node->right->right->data.type;

    // Check if the parameter name is '_'
    if (strcmp(paramName, "_") == 0) {
        // Special handling for parameters named '_'
        // The identifier is used in the function body
        printf("DEFVAR LF@%s\n", paramIdent);
        // Further code to handle parameter type and initialization goes here
    } else {
        // Regular parameter handling
        printf("DEFVAR LF@%s\n", paramIdent);
        // Further code to handle parameter type and initialization goes here
    }
}

void generateFunctionBody(const Node *node) {
    if (node == NULL) return;

    switch (node->type) {
        case STMT_NODE:
            generateFunctionBody(node->left);
            generateFunctionBody(node->right);
            break;
        case DEF_NODE:
        case LET_NODE:
        case VAR_NODE:
            generateVariableDeclaration(node);
            break;
        case IF_NODE:
            generateIfStatement(node);
            break;
        case FUNC_CALL_NODE:
            generateFunctionCall(node);
            break;
        // Add more cases as needed for other statement types
        default:
            printf("# Unknown statement type in function body\n");
    }
}
*/


// Detailní implementace dalších funkcí...
