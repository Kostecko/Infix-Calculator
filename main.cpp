#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cctype>
#define clear() printf("\033[H\033[J")

using namespace std;

enum TOKEN_ID{
    NUM,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
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
    double resultVal;
    bool doneCalc;


    void lexInput(const string& input);
    void calculate();

    const Token& peek() const;
    const Token& advance();

    const double parseExpr(); 
    const double parseTerm(); 
    const double parseSign();
    const double parseFactor(); 
public:
    Parser(const string& input);

    double result();
};

void useCalc(){
    clear();
    cout<<"<================= LITTLE LESS SIMPLE CALCULATOR! =================>\n\n";
    cout<<"Recursive Descent Method\n";
    cout<<"Available operations: +, -, *, /, ()\n\n";
    cout<<"<==================================================================>\n\n\n";

    cout<<"Equation:\n>> ";
    string input;//= "2+2*4*(2+7)";
    getline(cin, input);
    Parser output(input);
    printf("output: %.15g\n\n", output.result());
}

int main(){
    vector<string> tests = {
        // minus na początku
        "-5", "-5+8", "-2-2", "-0.5",
    
        // minus po operatorze
        "3*-2", "5+-2", "5--2", "8/-2", "5/-2", "-2*-3",
    
        // podwójny minus
        "--5", "---5", "2---3",
    
        // minus przed nawiasem
        "-(2+3)", "-(2+3)*4", "2*-(3+4)", "-(-(2))", "-((1))",
    
        // nawias z ujemnym środkiem
        "(-2)*(-3)", "(-2+5)*2", "((-1))",
    
        // spacje
        "- 5", "2 - -3",
    
        // priorytet i łączność
        "-2*3", "-6/3/2", "10--3-2",
    
        // błędy (powinny dać NAN)
        "-", "--", "2-", "-()", "-*3"
    };


    for(auto& i : tests){
        Parser p(i);
        cout<<p.result()<<endl;
    }

    //while(1){
    //    char c = '\0';
    //    cout<<"[1] calculate\n[2] Exit\n>> ";
    //    cin>>c;
    //    switch(c){
    //        case '1': useCalc(); break;
    //        case '2': return 0;
    //        default: cout<<"what\?\n"; break;
    //    }
    //}
    
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
            case '\0':token.tid = TOKEN_ID::TERMINATOR; break;
            case ' ': continue;
            default: 
                if(isdigit(static_cast<unsigned char>(input[i])) || input[i] == '.'){
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
    double left = parseSign();

    while(peek().tid == TOKEN_ID::OP_MUL || peek().tid == TOKEN_ID::OP_DIV){
        TOKEN_ID sign = peek().tid;
        advance(); 
        if(sign == TOKEN_ID::OP_MUL) left *= parseSign();
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

    return parseFactor() * sign;
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