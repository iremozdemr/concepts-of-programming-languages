#include <stdio.h>
#include <ctype.h>

//global değişkenler
int charClass;
char lexeme[100];
char nextChar; 
//işlenecek bir sonraki karakter
token;
int lexLen; 
//lexeme'in uzunluğu
int nextToken; 
//bir sonraki token'ın kodu
FILE *in_fp, *fopen(); 

//fonksiyonlar
void addChar(); 
//karakteri lexeme'e ekler
void getChar(); 
//sonraki karakteri alır ve sınıflandırır
void getNonBlank(); 
//boşlukları atlar
int lex(); 
//lexical analiz işlemini gerçekleştirir

//karakter sınıfları
#define LETTER 0 
//harfler için sınıf
#define DIGIT 1  
//rakamlar için sınıf
#define UNKNOWN 99 
//bilinmeyen karakterler için sınıf

//token kodları
#define INT_LIT 10      
//integer literal
#define IDENT 11        
//identifier
#define ASSIGN_OP 20    
//"="
#define ADD_OP 21       
//"+"
#define SUB_OP 22       
//"-"
#define MULT_OP 23      
//"*"
#define DIV_OP 24       
//"/"
#define LEFT_PAREN 25   
//"("
#define RIGHT_PAREN 26  
//")"

//operatörleri ve parantezleri kontrol eder
//uygun token kodunu döndürür
int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        default:
            addChar();
            nextToken = EOF; 
            break;
    }
    return nextToken;
}

//karakteri lexeme'e ekler
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar; 
        //karakteri lexeme'e ekle
        lexeme[lexLen] = 0; 
        //sonuna null karakter koy
    } 
    else {
        printf("Error - lexeme is too long\n");
    }
}

//sonraki karakteri okur
//karakter sınıfını belirler
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER; 
        else if (isdigit(nextChar))
            charClass = DIGIT; 
        else
            charClass = UNKNOWN; 
    } 
    else {
        charClass = EOF; 
    }
}

//boşlukları atlar
void getNonBlank() {
    while (isspace(nextChar)) 
        getChar();
}

//lexical analiz fonksiyonu
int lex() {
    lexLen = 0; 
    //lexeme'in uzunluğunu sıfırla
    getNonBlank(); 
    //boşlukları atla
    switch (charClass) {
        //identifier'ları çözümle
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) { 
                addChar();
                getChar();
            }
            nextToken = IDENT; 
            break;
        
        //integer literal'ları çözümle
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) { 
                addChar();
                getChar();
            }
            nextToken = INT_LIT; 
            break;
        
        //operatörleri ve parantezleri çözümle
        case UNKNOWN:
            lookup(nextChar); 
            getChar();
            break;
        
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    }
    printf("next token is: %d next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}

//<expr> → <term> {(+ | -) <term>}
//<term> → <factor> {(* | /) <factor>}
//<factor> → id | int_constant | ( <expr> )

void expr() {
    printf("enter <expr>\n");

    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();
    }

    printf("exit <expr>\n");
}

void term() {
    printf("enter <term>...\n");
    factor();

    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex();
        factor();
    }

    printf("exit <term>\n");
}

void factor() {
    printf("enter <factor>\n");

    if (nextToken == IDENT || nextToken == INT_LIT) {
        lex();
    } 
    else {
        if (nextToken == LEFT_PAREN) {
            lex();
            expr();
            if (nextToken == RIGHT_PAREN) {
                lex();
            } 
            else {
                error();
            }
        } 
        else {
            error();
        }
    }

    printf("exit <factor>\n");
}

//<ifstmt> → if (<boolexpr>) <statement> [else <statement>]

void ifstmt() {
    if (nextToken != IF_CODE) {
        error();
    } 
    else {
        lex();

        if (nextToken != LEFT_PAREN) {
            error();
        } 
        else {
            boolexpr();

            if (nextToken != RIGHT_PAREN) {
                error();
            } 
            else {
                statement();

                if (nextToken == ELSE_CODE) {
                    lex();
                    statement();
                }
            }
        }
    }
}