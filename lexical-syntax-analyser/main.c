#include <stdio.h>
#include "lexer_parser.h"

int main() {
    if((in_fp = fopen("front.in", "r")) == NULL){
        printf("ERROR - cannot open front.in\n");
    } 
    else{
        getChar(); 
        //ilk karakteri oku
        do {
            lex(); 
            //lexical analiz yap
        } while (nextToken != EOF); 
        //dosyanın sonuna kadar işle
    }
    return 0;
}