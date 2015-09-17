#include <stdio.h>
#include <stdlib.h>

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

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {
  free(tk);
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
char *FSM( TokenizerT * tk ){
  firstChar = *(tk->inputStream + tk->index);

  while(true){
    switch(tk->state){
      case 1:
        /* this is the first node in our fsm and acts as a sort of directory to the other main parts of our fsm */
        if( ( firstChar >= 'a' && firstChar <= 'z' ) || ( firstChar >= 'A' && firstChar <= 'Z' ) ){
          tk->state = 14;
        }else if( firstChar == '0' ){
          tk->state = 2;
        }else if( firstChar >= '1' && firstChar <= '9' ){
          tk->state = 8;
        }else if( firstChar == '-' ){
          tk->state = 15;
        }else if( firstChar == '*' ){
          tk->state = 19;
        }else if( firstChar == '&' ){
          tk->state = 21;
        }else if( firstChar == '!' ){
          tk->state = 24;
        }else if( firstChar == '+' ){
          tk->state = 26;
        }else if( firstChar == '/' ){
          tk->state = 29;
        }else if( firstChar == '%' ){
          tk->state = 31;
        }else if( firstChar == '>' ){
          tk->state = 33;
        }else if( firstChar == '<' ){
          tk->state = 37;
        }else if( firstChar == '=' ){
          tk->state = 41;
        }else if( firstChar == '^' ){
          tk->state = 43;
        }else if( firstChar == '|' ){
          tk->state = 45;
        }else{
          /* this is a character not recognized in any token */
          return firstChar;
        }
        break;
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
  TokenizerT* tokenizer = TKCreate(argv[1]);
  token = TKGetNextToken(tokenizer);
  while(token != NULL){
    printf("%s\n", token);
    token = TKGetNextToken(tokenizer);
  }
  return 0;
}
