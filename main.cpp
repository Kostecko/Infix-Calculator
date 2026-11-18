#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cctype>
#include <unordered_map>
#include <limits>
#include <cstdio>

#define clearConsole() printf("\033[H\033[J")

using namespace std;

enum TOKEN_ID{
    NUM,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW,
    FUNCTION,
    COMMA,
    LPAREN,
    RPAREN,
    TERMINATOR,
    INVALID,
    NUL
};

struct Token{
    TOKEN_ID tid = TOKEN_ID::NUL;
    double value = NAN;
    string name = "\0";
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
    bool checkString(const string& input, size_t& i, Token& token);

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
    cout<<"Available operations: +, -, *, /, (), ^, root(r, i)\n";
    cout<<"Incoming: %, !\n\n";
    cout<<"<==================================================================>\n\n\n";

    cout<<"Equation:\n>> ";
    string input;//= "2+2*4*(2+7)";
    getline(cin, input);
    Parser output(input);
    printf("output: %.15g\n\n", output.result());
}

int main(){
    vector<string> rootTests = {
        //1
        "root(9,2)",
        //2
        "root(27,3)",
        //3
        "root(64,3)",
        //4
        "root(16,4)",
        //5
        "root(2,1)",
        //6
        "root(1,5)",
        //7
        "root(0,2)",
        //8
        "root(100,2)",
        //9
        "root(2.25,2)",
        //10
        "root(0.25,2)",
        //11
        "root(9,0.5)",
        //12
        "root(9,-2)",
        //13
        "root(8,3)",
        //14
        "root( 9 , 2 )",
        //15
        "root (9,2)",
        //16
        "root(2+7,1+1)",
        //17
        "root(3*3,4/2)",
        //18
        "root((2+7),(1+1))",
        //19
        "root(-9+18,2)",
        //20
        "root(9,--2)",
        //21
        "root(root(81,2),2)",
        //22
        "root(2^4,2)",
        //23
        "root(16,2^2)",
        //24
        "-root(9,2)",
        //25
        "2*root(16,2)",
        //26
        "root(16,2)^2",
        //27
        "2^root(9,2)",
        //28
        "root(9,2)+1",
        //29
        "root(9,2)*root(16,2)",
        //30
        "root(4,2)^3",
        //31
        "root(9,2)-root(4,2)",
        //32
        "10/root(25,2)",
        //33
        "(root(9,2))",
        //34
        "--root(9,2)",
        //35
        "root(9,2)^root(4,2)",
        //36
        "root(9,0)",
        //37
        "root(1,0)",
        //38
        "root(0,0)",
        //39
        "root(-9,2)",
        //40
        "root(-8,3)",
        //41
        "root(0,-1)",
        //42
        "root",
        //43
        "root(",
        //44
        "root()",
        //45
        "root(9)",
        //46
        "root(9,)",
        //47
        "root(,2)",
        //48
        "root(9 2)",
        //49
        "root(9,2",
        //50
        "root 9,2",
        //51
        "root(9,2))",
        //52
        "root(9,,2)",
        //53
        "root(9,2,3)",
        //54
        "root((9,2))",
        //55
        "root(9;2)",
        //56
        "foo(1,2)",
        //57
        "rooot(9,2)",
        //58
        "root(root,2)",
        //59
        "2root(9,2)",
        //60
        "root(9,2)2",
        //61
        "root(9,2)(3)",
        //62
        "9,2"
        };

    int i = 1;

    for(auto& s : rootTests){
        Parser output(s);
        printf("%i. output: %.15g\n\n", i++, output.result());
    }

    while(0){
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
    for(size_t i =0; i <= input.length(); i++){
        Token token;

        switch(input[i]){
            case '+': token.tid = TOKEN_ID::OP_ADD; break;
            case '-': token.tid = TOKEN_ID::OP_SUB; break;
            case '*': token.tid = TOKEN_ID::OP_MUL; break;
            case '/': token.tid = TOKEN_ID::OP_DIV; break;
            case '(': token.tid = TOKEN_ID::LPAREN; break;
            case ')': token.tid = TOKEN_ID::RPAREN; break;
            case '^': token.tid = TOKEN_ID::OP_POW; break;
            case ',': token.tid = TOKEN_ID::COMMA; break;
            case '\0':token.tid = TOKEN_ID::TERMINATOR; break;
            case ' ': continue;
            default: if(checkString(input, i, token)) i--; break;
        }
        token.index = i;
        inputTokens.push_back(token);
    }
}

double Parser::calculate(){
    index = 0;
    double output = parseExpr();

    TOKEN_ID tid = peek().tid;

    if(tid != TOKEN_ID::TERMINATOR){
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

bool Parser::checkString(const string& input, size_t& i, Token& token){
    string str="";

    if(isdigit(static_cast<unsigned char>(input[i]))){
        int dotCount = 0;
        while(isdigit(static_cast<unsigned char>(input[i])) || input[i] == '.'){
            if(input[i] == '.') dotCount++;
            
            if(dotCount > 1){
                token.tid = TOKEN_ID::INVALID;
                return true;
            }
            str += input[i++];
        }
        if(str.length()){
            token.tid = TOKEN_ID::NUM;
            token.value = stod(str);
            return true;
        }
    }


    while(isalpha(static_cast<unsigned char>(input[i]))) str+=input[i++];
    

    if(str == "root"){
        token.tid = TOKEN_ID::FUNCTION; 
        token.name = "root"; 
        return true;
    }
    
    token.tid = TOKEN_ID::INVALID; 
    
    if(str.length()) return true;
    return false;
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

        case TOKEN_ID::FUNCTION: {
            if(peek().name == "root"){
                advance();

                if(peek().tid != TOKEN_ID::LPAREN) return NAN;
                advance();
                const double radicand = parseExpr();

                if(peek().tid != TOKEN_ID::COMMA) return NAN;
                advance();
                const double index_ = parseExpr();

                if(peek().tid != TOKEN_ID::RPAREN) return NAN;
                advance();
                if(index_ == 0.0 || isnan(index_) || radicand < 0.0 || isnan(radicand)) return NAN;

                output = pow(radicand, 1.0/index_);
            }
            break;
        }

        case TOKEN_ID::LPAREN: {
            advance();
            output = parseExpr();
            if(peek().tid == TOKEN_ID::RPAREN) advance();
            else output = NAN;
            break;
        }


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