#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TokenizerT_ {
  char * inputStream;
  int index;
  int state;
};

typedef struct TokenizerT_ TokenizerT;

TokenizerT *TKCreate( char * ts ) {
  TokenizerT* foo = (TokenizerT*)malloc(sizeof(TokenizerT));

  foo->inputStream = ts;
  foo->index = 0;
  foo->state = 0;

  return foo;
}

void TKDestroy( TokenizerT * tk ) {
  free(tk);
}

void resultOutput( char * name, char * token ){
  printf("%s \"%s\"\n", name, token);
}

void escapeCharacterResultOutput( char * escapeCharacter){
  printf("escape character: \"[0x%x]\"\n", *escapeCharacter);
}

char *FSM( TokenizerT * tk ){
  char currentChar;
  char nextChar;
  char * token;
  char * temp;
  char * type = "malformed";

  temp = (char *)malloc(sizeof(char)*2);
  token = (char *)malloc(sizeof(char));

  token[0] = '\0';

  while(1){
    currentChar = *(tk->inputStream + tk->index);
    nextChar = *(tk->inputStream + tk->index + 1);
    /* if we encounter a space but we don't have a token, just skip the spaces. if we do have a token, the space ends the token so return it */
    if( token[0] == '\0' ){
      if(currentChar == 0x20 || currentChar == 0x09 || ( currentChar >= 0x0a && currentChar <= 0x0d)){
        while(currentChar == 0x20 || currentChar == 0x09 || ( currentChar >= 0x0a && currentChar <= 0x0d)){
            tk->index = tk->index + 1;
            currentChar = *(tk->inputStream + tk->index);
        }
      }
    }else{
      if(currentChar == 0x20 || currentChar == 0x09 || ( currentChar >= 0x0a && currentChar <= 0x0d)){
        while(currentChar == 0x20 || currentChar == 0x09 || ( currentChar >= 0x0a && currentChar <= 0x0d)){
            tk->index = tk->index + 1;
            currentChar = *(tk->inputStream + tk->index);
        }
        /* a space ends a token */
        resultOutput(type, token);
        return token;
      }
    }

    if(currentChar == '\0'){
      return NULL;
    }

    switch(tk->state){
      case 1:
        if( ( currentChar >= 'a' && currentChar <= 'z' ) || ( currentChar >= 'A' && currentChar <= 'Z' ) ){
          tk->state = 14;
          type = "word";
        }else if( currentChar == '0' ){
          tk->state = 2;
          type = "zero";
        }else if( currentChar >= '1' && currentChar <= '9' ){
          tk->state = 8;
          type = "integer";
        }else if( currentChar == '-' ){
          tk->state = 15;
          type = "minus/subtract";
        }else if( currentChar == '*' ){
          tk->state = 19;
          type = "multiply/indirect";
        }else if( currentChar == '&' ){
          tk->state = 21;
          type = "address/bitwise and";
        }else if( currentChar == '!' ){
          tk->state = 24;
          type = "negate";
        }else if( currentChar == '+' ){
          tk->state = 26;
          type = "add";
        }else if( currentChar == '/' ){
          tk->state = 29;
          type = "divide";
        }else if( currentChar == '%' ){
          tk->state = 31;
          type = "modulo";
        }else if( currentChar == '>' ){
          tk->state = 33;
          type = "greater than";
        }else if( currentChar == '<' ){
          tk->state = 37;
          type = "less than";
        }else if( currentChar == '=' ){
          tk->state = 41;
          type = "assignment";
        }else if( currentChar == '^' ){
          tk->state = 43;
          type = "bitwise exclusive or";
        }else if( currentChar == '|' ){
          tk->state = 45;
          type = "bitwise or";
        }else if( currentChar == '[' ){
          tk->state = 48;
          type = "left square bracket";
        }else if( currentChar == ']' ){
          tk->state = 49;
          type = "right square bracket";
        }else if( currentChar == '(' ){
          tk->state = 50;
          type = "left parenthesis";
        }else if( currentChar == ')' ){
          tk->state = 51;
          type = "right parenthesis";
        }else if (currentChar == '{' ){
          tk->state = 52;
          type = "left curly brace";
        }else if ( currentChar == '}' ){
          tk->state = 53;
          type = "right curly brace";
        }else if( currentChar == ';' ){
          tk->state = 54;
          type = "statement terminator";
        }else if( currentChar == ':' ){
          tk->state = 55;
          type = "colon";
        }else if( currentChar == '.' ){
          tk->state = 56;
          type = "dot operator";
        }else if( currentChar == ',' ){
          tk->state = 57;
          type = "comma operator";
        }else{
          /* this character is not in any token our fsm will recognize */
          temp[0] = currentChar;
          strcat(token, temp);
          escapeCharacterResultOutput(token);
          tk->index = tk->index + 1;
          return token;
        }
        break;
      case 2:
        if( currentChar >= '0' && currentChar <= '7' ){
          tk->state = 3;
          type = "octal";
        }else if( currentChar == 'X' || currentChar == 'x' ){
          tk->state = 4;
        }else if( currentChar == '.' ){
          tk->state = 6;
        }else{
          /* zero */
          resultOutput(type, token);
          return token;
        }
        break;
      case 3:
        if( currentChar >= '0' && currentChar <= '7' ){
          tk->state = 3;
          type = "octal";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 4:
        type = "malformed";
        if(( currentChar >= '0' && currentChar <= '9' ) || (currentChar >= 'A' && currentChar <= 'F') || (currentChar >= 'a' && currentChar <= 'f') ){
          tk->state = 5;
          type = "hexadecimal";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 5:
        if(( currentChar >= '0' && currentChar <= '9' ) || (currentChar >= 'A' && currentChar <= 'F') || (currentChar >= 'a' && currentChar <= 'f') ){
          tk->state = 5;
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 6:
        type = "malformed";

        if( currentChar >= '0' && currentChar <= '9' ){
          tk->state = 7;
          type = "float";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 7:
        if( currentChar >= '0' && currentChar <= '9' ){
          tk->state = 7;
        }else if( currentChar == 'e' || currentChar == 'E' ){
          tk->state = 11;
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 8:
        if( currentChar >= '0' && currentChar <= '9' ){
          tk->state = 8;
          type = "integer";
        }else if( currentChar == '.' ){
          tk->state = 9;
        }else if( currentChar == 'e' || currentChar == 'E' ){
          tk->state = 11;
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 9:
        type = "malformed";
        if( currentChar >= '0' && currentChar <= '9' ){
          tk->state = 10;
          type = "float";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 10:
        if( currentChar >= '0' && currentChar <= '9' ){
          tk->state = 10;
          type = "float";
        }else if( currentChar == 'e' || currentChar == 'E' ){
          tk->state = 11;
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 11:
        type = "malformed";
        if( currentChar >= '0' && currentChar <= '9' ){
          tk->state = 13;
          type = "float";
        }else if( currentChar == '+' || currentChar == '-' ){
          tk->state = 12;
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 12:
        type = "malformed";
        if( currentChar >= '0' && currentChar <= '9' ){
          tk->state = 13;
          type = "float";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 13:
        if( currentChar >= '0' && currentChar <= '9' ){
          tk->state = 13;
          type = "float";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 14:
        if( ( currentChar >= 'a' && currentChar <= 'z' ) || ( currentChar >= 'A' && currentChar <= 'Z' ) || ( currentChar >= '0' && currentChar <= '9' )){
          tk->state = 14;
          type = "word";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 15:
        if( currentChar == '-' ){
          tk->state = 16;
          type = "decrement";
        }else if( currentChar == '=' ){
          tk->state = 17;
          type = "minus equals";
        }else if( currentChar == '>' ){
          tk->state = 18;
          type ="structure pointer";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 19:
        if( currentChar == '=' ){
          tk->state = 20;
          type = "times equals";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 21:
        if( currentChar == '&' ){
          tk->state = 22;
          type = "logical and";
        }else if( currentChar == '=' ){
          tk->state = 23;
          type = "and equals";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 24:
        if( currentChar == '=' ){
          tk->state = 25;
          type = "negate equals";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 26:
        if( currentChar == '+' ){
          tk->state = 27;
          type = "increment";
        }else if( currentChar == '=' ){
          tk->state = 28;
          type = "add equals";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 29:
        if( currentChar == '=' ){
          tk->state = 30;
          type = "divide equals";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 31:
        if( currentChar == '=' ){
          tk->state = 32;
          type = "mod equals";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 33:
        if( currentChar == '>' ){
          tk->state = 34;
          type = "shift right";
        }else if( currentChar == '=' ){
          tk->state = 36;
          type = "greater than or equals";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 34:
        if( currentChar == '=' ){
          tk->state = 35;
          type = "shift right equals";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 37:
        if( currentChar == '<' ){
          tk->state = 38;
          type = "shift left";
        }else if( currentChar == '=' ){
          tk->state = 40;
          type = "less than or equals";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 38:
        if( currentChar == '=' ){
          tk->state = 39;
          type = "shift left equals";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 41:
        if( currentChar == '=' ){
          tk->state = 42;
          type = "logical equals to";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 43:
        if( currentChar == '=' ){
          tk->state = 44;
          type = "bitwise exclusive or equals";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      case 45:
        if( currentChar == '|' ){
          tk->state = 46;
          type = "logical or";
        }else if( currentChar == '=' ){
          tk->state = 47;
          type = "bitwise or equals";
        }else{
          resultOutput(type, token);
          return token;
        }
        break;
      // all of our leaf nodes that just print their token
      case 16: case 17: case 18: case 20:
      case 22: case 23: case 25: case 27:
      case 28: case 30: case 32: case 35:
      case 36: case 39: case 40: case 42:
      case 44: case 46: case 47: case 48:
      case 49: case 50: case 51: case 52:
      case 53: case 54: case 55: case 56:
      case 57:
        resultOutput(type, token);
        return token;
    }
    tk->index = tk->index + 1;
    // add char to tokenizer
    temp[0] = currentChar;
    strcat(token, temp);

    if(nextChar == '\0'){
      resultOutput(type, token);
      return token;
    }
  }

  return NULL;
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk ) {
  char * token;
  tk->state = 1;
  token = FSM(tk);
  return token;
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
  char * token;
  TokenizerT* tokenizer;
  if( argc != 2 ){
    printf("error: wrong number of arguments\n");
    return 1;
  }
  tokenizer = TKCreate(argv[1]);
  token = TKGetNextToken(tokenizer);

  while(token != NULL){
    token = TKGetNextToken(tokenizer);
  }

  TKDestroy(tokenizer);
  return 0;
}
