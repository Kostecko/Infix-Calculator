#include <iostream>
#include <stack>
#include <string>
#include <cmath>
#include <limits>

using namespace std;

struct word{
    double val = 0;
    char operat = '\0';
};

int hierarchy(char c){
    switch(c){
        case '+': return 2;
        case '-': return 2;
        case '*': return 3;
        case '/': return 3;
        case '(': return 9;
        case ')': return 9;
        default: break;
    }
    return 0;
}

double calc(stack<word>& wordStack){
    if(!wordStack.size()) return NAN;

    double output = 0.0, a = 0.0, b = 0.0;
    char operat = wordStack.top().operat;
    
    if(operat){
        wordStack.pop();
        b = calc(wordStack);
        a = calc(wordStack);
    } 

    switch(operat){
        case '+': output = a + b; break;
        case '-': output = a - b; break;
        case '*': output = a * b; break;
        case '/': output = a / b; break;
        default: break;
    }
    
    if(!operat && wordStack.size()){
        output = wordStack.top().val;
        wordStack.pop();
    }
    return output;
}


int main(){
    cout<<"<================= SIMPLE CALCULATOR! =================>\n\n";
    cout<<"Infix -> RPN translation using shunting-yard algorithm\n";
    cout<<"Only +, -, *, \\ works \n\n";
    cout<<"<======================================================>\n\n\n";

    cout<<"Equation:\n>> ";
    string equation;
    cin>>equation;

    double output = 0.0;
    stack<word> wordStack;
    stack<char> operatorStack;

    int eqlen = equation.length();

    string num="";
    for(int i=0; i <= equation.length(); i++){
        if(operatorStack.size() && operatorStack.top() == ')'){
            operatorStack.pop();
            while(operatorStack.size() && operatorStack.top() != '('){
                wordStack.push({NAN, operatorStack.top()});
                operatorStack.pop();
            }
            operatorStack.pop();
        }

        if(!hierarchy(equation[i])){ 
            if(equation[i] != ' ' && equation[i] != '\0') num += equation[i]; 
            continue; 
        }

        if(num.length() > 0){
            wordStack.push({stod(num), '\0'});
            num.clear();
        }

        while(operatorStack.size() > 0){
            int sih = hierarchy(equation[i]);
            int oth = hierarchy(operatorStack.top());

            if(sih != 0 && oth != 9 && sih <= oth){
                wordStack.push({NAN, operatorStack.top()});
                operatorStack.pop();
            }
            else break;
        }
        operatorStack.push(equation[i]);
    }

    if(!num.empty()){
        wordStack.push({stod(num), '\0'});
        num.clear();
    }

    int ops = operatorStack.size();
    for(int i = 0; i < ops; i++){
        wordStack.push({NAN, operatorStack.top()});
        operatorStack.pop();
    }
    
    output = calc(wordStack);

    printf("output: %.15g\n\n", output);
   
    return 0;
}