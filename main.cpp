#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cctype>

using namespace std;

enum TOKEN_ID{
    NUM,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    TERMINATOR,
    INVALID,
    NUL
};

struct Token{
    TOKEN_ID tid = TOKEN_ID::NUL;
    double value = NAN;
    int index = -1;
};

class Parser{
private:
    int index;
    vector<Token> inputTokens;
    double resultVal;
    bool doneCalc;


    void lexInput(const string& input);
    void calculate();

    const Token& peek() const;
    const Token& advance();

    const double parseExpr(); 
    const double parseTerm(); 
    const double parseFactor(); 
public:
    Parser(const string& input);

    double result();
};



int main(){
    cout<<"<================= LITTLE LESS SIMPLE CALCULATOR! =================>\n\n";
    cout<<"Recursive Descent Method\n";
    cout<<"Available operations: +, -, *, /\n\n";
    cout<<"<==================================================================>\n\n\n";

    //cout<<"Equation:\n>> ";
    string input = "2+3*4";
    //cin>>equation;
    Parser output(input);

    

    printf("output: %.15g\n\n", output.result());
   
    return 0;
}

//INTERNAL FUNCTIONS

void Parser::lexInput(const string& input){
    Token token;

    for(size_t i =0; i <= input.length(); i++){
        token = {TOKEN_ID::NUL, NAN, -1};

        switch(input[i]){
            case '+': token.tid = TOKEN_ID::OP_ADD; break;
            case '-': token.tid = TOKEN_ID::OP_SUB; break;
            case '*': token.tid = TOKEN_ID::OP_MUL; break;
            case '/': token.tid = TOKEN_ID::OP_DIV; break;
            case '\0':token.tid = TOKEN_ID::TERMINATOR; break;
            case ' ': continue;
            default: 
                if(isdigit(input[i])){
                    string number = "";
                    while(isdigit(static_cast<unsigned char>(input[i])) || input[i] == '.') 
                    number += input[i++];
                    
                    if(number.length()){ 
                        token.tid = TOKEN_ID::NUM; 
                        i--; 
                        token.value = stod(number); 
                    }
                }
                else token.tid = TOKEN_ID::INVALID;
                
                break;
        }
        token.index = i;
        inputTokens.push_back(token);
    }
}

void Parser::calculate(){
    index = 0;
    double output = parseExpr();
    doneCalc = true;

    if(peek().tid != TOKEN_ID::TERMINATOR){
        cout<<"calculate() error!\n\n";
        return;
    }
    resultVal = output;
}


const Token& Parser::peek() const{
    return inputTokens.at(index);
}

const Token& Parser::advance(){
    const Token& t = peek();
    if(t.tid != TOKEN_ID::TERMINATOR) index++;
    return t;
}

const double Parser::parseExpr(){
    double left = parseTerm();

    while(peek().tid == TOKEN_ID::OP_ADD || peek().tid == TOKEN_ID::OP_SUB){
        TOKEN_ID sign = peek().tid;
        advance();
        if(sign == TOKEN_ID::OP_ADD) left += parseTerm();
        else left -= parseTerm();
    }
    return left;
}

const double Parser::parseTerm(){
    double left = parseFactor();

    while(peek().tid == TOKEN_ID::OP_MUL || peek().tid == TOKEN_ID::OP_DIV){
        TOKEN_ID sign = peek().tid;
        advance(); 
        if(sign == TOKEN_ID::OP_MUL) left *= parseFactor();
        else left /= parseFactor();
    }
    return left;
}

 const double Parser::parseFactor(){
    switch(peek().tid){
        case TOKEN_ID::NUM: return advance().value;
        default: break;
    }
    return NAN;
}

//EXTERNAL FUNCTIONS

Parser::Parser(const string& input) {
    resultVal = NAN;
    doneCalc = false;
    index = 0;
    inputTokens.reserve(input.length()+1);
    lexInput(input);
}

double Parser::result() {
    if(!doneCalc) calculate();
    return resultVal;
}