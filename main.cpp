#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cctype>
#define clearConsole() printf("\033[H\033[J")

using namespace std;

enum TOKEN_ID{
    NUM,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW,
    OP_ROOT,
    LPAREN,
    RPAREN,
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

    void lexInput(const string& input);
    double calculate();

    const Token& peek() const;
    const Token& advance();

    const double parseExpr(); 
    const double parseTerm(); 
    const double parseSign();
    const double parsePower();
    const double parseFactor(); 
public:
    Parser(const string& input);

    double result();
};

void useCalc(){
    clearConsole();
    cout<<"<================= LITTLE LESS SIMPLE CALCULATOR! =================>\n\n";
    cout<<"Recursive Descent Method\n\n";
    cout<<"Available operations: +, -, *, /, (), ^\n";
    cout<<"Incoming: root(), %, !\n\n";
    cout<<"<==================================================================>\n\n\n";

    cout<<"Equation:\n>> ";
    string input;//= "2+2*4*(2+7)";
    getline(cin, input);
    Parser output(input);
    printf("output: %.15g\n\n", output.result());
}

int main(){
    while(1){
        cout<<"[1] calculate\n[2] Exit\n>> ";
        char c;

        if(!(cin >> c)) break;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch(c){
            case '1': useCalc(); break;
            case '2': return 0;
            default: cout<<"what\?\n"; break;
        }
    }
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
            case '(': token.tid = TOKEN_ID::LPAREN; break;
            case ')': token.tid = TOKEN_ID::RPAREN; break;
            case '^': token.tid = TOKEN_ID::OP_POW; break;
            case '\0':token.tid = TOKEN_ID::TERMINATOR; break;
            case ' ': continue;
            default: 
                if(isdigit(static_cast<unsigned char>(input[i]))){
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

double Parser::calculate(){
    index = 0;
    double output = parseExpr();

    if(peek().tid != TOKEN_ID::TERMINATOR){
        cout<<"calculate() error!\n\n";
        return NAN;
    }

    return output;
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
        TOKEN_ID op = peek().tid;
        advance();
        if(op == TOKEN_ID::OP_ADD) left += parseTerm();
        else left -= parseTerm();
    }
    return left;
}

const double Parser::parseTerm(){
    double left = parseSign();

    while(peek().tid == TOKEN_ID::OP_MUL || peek().tid == TOKEN_ID::OP_DIV){
        TOKEN_ID op = peek().tid;
        advance(); 
        if(op == TOKEN_ID::OP_MUL) left *= parseSign();
        else left /= parseSign();
    }
    return left;
}

const double Parser::parseSign(){
    double sign = 1.0;
    
    while(peek().tid == TOKEN_ID::OP_SUB){
        advance();
        sign *= -1.0;
    }

    return parsePower() * sign;
}

const double Parser::parsePower(){
    double output = parseFactor();

    if(peek().tid == TOKEN_ID::OP_POW){
        advance();
        double power = parseSign();
        if(isnan(output) || isnan(power)) return NAN;
        return pow(output, power);
    }

    return output;
}

 const double Parser::parseFactor(){
    double output = NAN;
    switch(peek().tid){
        case TOKEN_ID::NUM: output = advance().value; break;
        case TOKEN_ID::LPAREN: 
        advance(); 
        output = parseExpr();
        if(peek().tid == TOKEN_ID::RPAREN) advance();
        else output = NAN;
        break;
        default: break;
    }
    return output;
}



//EXTERNAL FUNCTIONS

Parser::Parser(const string& input) {
    index = 0;
    inputTokens.reserve(input.length()+1);
    lexInput(input);
}

double Parser::result() {
    return calculate();
}