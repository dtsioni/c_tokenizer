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
  printf("%s %s\n", name, token);
}

char *FSM( TokenizerT * tk ){
  char currentChar;
  char nextChar;
  int endOfToken;
  char * token;
  char * temp;
  char * type = "malformed";

  temp = (char *)malloc(sizeof(char)*2);
  token = (char *)malloc(sizeof(char));

  endOfToken = 0;
  token[0] = '\0';
  // loop will run until our token is returned


  while(1){

    //printf("flag %d loop\n", __LINE__);
    // find the state of our fsm that our tokenizer is on

    currentChar = *(tk->inputStream + tk->index);
    nextChar = *(tk->inputStream + tk->index + 1);

    if(currentChar == 0x20 || currentChar == 0x09 || ( currentChar >= 0x0a && currentChar <= 0x0d)){
      while(currentChar == 0x20 || currentChar == 0x09 || ( currentChar >= 0x0a && currentChar <= 0x0d)){
          tk->index = tk->index + 1;
          currentChar = *(tk->inputStream + tk->index);
          nextChar = *(tk->inputStream + tk->index + 1);
      }
      resultOutput(type, token);
      return token;
    }

    if(currentChar == '\0'){
      return NULL;
    }

    // check for white space
    /*if( nextChar == 0x20 || nextChar == 0x09 || ( nextChar >= 0x0a && nextChar <= 0x0d )){
      printf("whitespace found\n");
      endOfToken = 1;
    }*/
    printf("Current char is: %c Next char is: %c\n", currentChar, nextChar);
    printf("Current state is: %d\n", tk->state);
    // add the current character to the end of our token string



    switch(tk->state){
      /* this is the starting state of our fsm */
      case 1:
      //printf("flag %d entered switch case 1\n", __LINE__);
        if( ( currentChar >= 'a' && currentChar <= 'z' ) || ( currentChar >= 'A' && currentChar <= 'Z' ) ){
          tk->state = 14;
        }else if( currentChar == '0' ){
          //printf("flag %d\n", __LINE__);
          tk->state = 2;
          type = "zero";
        }else if( currentChar >= '1' && currentChar <= '9' ){
          tk->state = 8;
        }else if( currentChar == '-' ){
          tk->state = 15;
        }else if( currentChar == '*' ){
          tk->state = 19;
        }else if( currentChar == '&' ){
          tk->state = 21;
        }else if( currentChar == '!' ){
          tk->state = 24;
        }else if( currentChar == '+' ){
          tk->state = 26;
        }else if( currentChar == '/' ){
          tk->state = 29;
        }else if( currentChar == '%' ){
          tk->state = 31;
        }else if( currentChar == '>' ){
          tk->state = 33;
        }else if( currentChar == '<' ){
          tk->state = 37;
        }else if( currentChar == '=' ){
          tk->state = 41;
        }else if( currentChar == '^' ){
          tk->state = 43;
        }else if( currentChar == '|' ){
          tk->state = 45;
        }else{
          /* this is a character not recognized in any token */
          resultOutput(type, token);
          return token;
        }
        break;
      case 2:
      //printf("flag %d entered switch case 2\n", __LINE__);

        if( currentChar >= '0' && currentChar <= '7' ){
          tk->state = 3;
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
      //printf("flag %d entered switch case 3\n", __LINE__);
        type = "octal";
        if( currentChar >= '0' && currentChar <= '7' ){
          tk->state = 3;
        }else{
          /* octal */
          resultOutput(type, token);
          return token;
        }
        break;
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
  tokenizer = TKCreate(argv[1]);
  token = TKGetNextToken(tokenizer);
  while(token != NULL){
    printf("got token: %s\n", token);
    token = TKGetNextToken(tokenizer);
  }
  return 0;
}
