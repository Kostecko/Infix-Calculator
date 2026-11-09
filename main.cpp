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

double calc(stack<word>& st){
    if(!st.size()) return NAN;

    double output = 0.0, a = 0.0, b = 0.0;
    char op = st.top().operat;
    
    if(op){
        st.pop();
        b = calc(st);
        a = calc(st);
    } 

    switch(op){
        case '+': output = a + b; break;
        case '-': output = a - b; break;
        case '*': output = a * b; break;
        case '/': output = a / b; break;
        default: break;
    }
    
    if(!op && st.size()){
        output = st.top().val;
        st.pop();
    }
    return output;
}


int main(){
    string s = "3*2";
    double output = 0.0;
    stack<word> st;
    stack<char> op;

    int slen = s.length();

    string num="";
    for(int i=0; i <= s.length(); i++){
        if(op.size() && op.top() == ')'){
            op.pop();
            while(op.size() && op.top() != '('){
                st.push({NAN, op.top()});
                op.pop();
            }
            op.pop();
        }

        if(!hierarchy(s[i])){ 
            if(s[i] != ' ' && s[i] != '\0') num += s[i]; 
            continue; 
        }

        if(num.length() > 0){
            st.push({stod(num), '\0'});
            num.clear();
        }

        while(op.size() > 0){
            int sih = hierarchy(s[i]);
            int oth = hierarchy(op.top());

            if(sih != 0 && oth != 9 && sih <= oth){
                st.push({NAN, op.top()});
                op.pop();
            }
            else break;
        }
        op.push(s[i]);
    }

    if(!num.empty()){
        st.push({stod(num), '\0'});
        num.clear();
    }

    int ops = op.size();
    for(int i = 0; i < ops; i++){
        st.push({NAN, op.top()});
        op.pop();
    }
    
    output = calc(st);

    printf("output: %.15g\n\n", output);
   
    return 0;
}