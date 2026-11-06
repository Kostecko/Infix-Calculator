#include <iostream>
#include <stack>
#include <string>

using namespace std;

struct word{
    int val = 0;
    bool isOp = 0;
};

int hierarchy(char c){
    switch(c){
        case '+': return 1;
        case '-': return 1;
        case '*': return 2;
        case '/': return 2;
        case '(': return 9;
        case ')': return 9;
        default: break;
    }
    return 0;
}

int calc(stack<word>& st){
    int a = 0, b=0;
    char op = '\0';

    if(st.top().isOp){
        op = char(st.top().val);
        st.pop();
        b = calc(st);
        a = calc(st);
    } 

    switch(op){
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: break;
    }

    int r = st.top().val;
    st.pop();
    return r;
}


int main(){
    string s = "5*5+100/4";
    int output = 0;
    stack<word> st;
    stack<char> op;

    int slen = s.length();

    string num="";
    for(int i=0; i<slen; i++){
        if(s[i] == ' ') continue;

        if(!hierarchy(s[i])){
            num += s[i];
            if(i == slen - 1) st.push({stoi(num), false});
            continue;
        }

        st.push({stoi(num), false});
        num.clear();

        while(op.size() > 0){
            if(hierarchy(s[i]) < hierarchy(op.top())){
                st.push({op.top(), true});
                op.pop();
            }
            else break;
        }
        op.push(s[i]);
    }

    int ops = op.size();
    for(int i = 0; i < ops; i++){
        st.push({op.top(), true});
        op.pop();
    }
   
    output = calc(st);
   
    printf("%i\n\n", output);
   
    return 0;
}