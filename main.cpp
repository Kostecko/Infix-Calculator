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
    cout<<"Available operations: +, -, *, \\\n\n";
    cout<<"<==================================================================>\n\n\n";

    //cout<<"Equation:\n>> ";
    string input = "2+3*6/2";
    //cin>>equation;
    Parser output(input);

    

    printf("output: %.15g\n\n", output.result());
   
    return 0;
}

//INTERNAL FUNCTIONS

void Parser::lexInput(const string& input){
    if(!input.length()) return;

    Token token;

    for(int i =0; i <= input.length(); i++){
        token = {TOKEN_ID::NUL, NAN, -1};

        switch(input[i]){
            case '+': token.tid = TOKEN_ID::OP_ADD; break;
            case '-': token.tid = TOKEN_ID::OP_SUB; break;
            case '*': token.tid = TOKEN_ID::OP_MUL; break;
            case '/': token.tid = TOKEN_ID::OP_DIV; break;
            case '\0':token.tid = TOKEN_ID::TERMINATOR; break;
            default: 
                token.tid = TOKEN_ID::NUM; 
                string number = "";
                while(isdigit(input[i]) || input[i] == '.') 
                    number += input[i++];

                if(number.length()){ 
                    i--; 
                    token.value = stod(number); 
                }
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
    double right = 0.0;
    double output = left;

    switch(peek().tid){
        case TOKEN_ID::OP_ADD: 
            advance(); right = parseTerm(); output = left + right; break;
        case TOKEN_ID::OP_SUB:
            advance(); right = parseTerm(); output = left - right; break;
        default: break;
    }
    return output;
}

const double Parser::parseTerm(){
    double a = parseFactor();

    switch(peek().tid){
        case TOKEN_ID::OP_MUL: advance(); a *= parseExpr(); break;
        case TOKEN_ID::OP_DIV: advance(); a /= parseExpr(); break;
        default: break;
    }
    return a;
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
    bool doneCalc = false;
    index = 0;
    inputTokens.reserve(input.length()+1);
    lexInput(input);
}

double Parser::result() {
    if(!doneCalc) calculate();
    return resultVal;
}