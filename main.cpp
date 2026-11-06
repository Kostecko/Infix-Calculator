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

int calc(stack<word>& st){
    static int iterator = 0;
    ++iterator;
    //printf("%i\n", iterator);

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
    string s = "2+(2*2)";
    int output = 0;
    stack<word> st;
    stack<char> op;

    int slen = s.length();

    string num="";
    for(int i=0; s[i] != '\0'; i++){
        if(s[i] == ' ') continue;
        
        if(op.size() && op.top() == ')'){
            op.pop();
            while((op.top() != '(') && op.size()){
                st.push({op.top(), true});
                op.pop();
            }
            op.pop();
        }

        if(!hierarchy(s[i])){
            num += s[i];
            if(i == slen - 1) 
                st.push({stoi(num), false});
            continue;
        }
        if(num.length() > 0){
            st.push({stoi(num), false});
            num.clear();
        }

        while(op.size() > 0){
            if((hierarchy(s[i]) < hierarchy(op.top())) && hierarchy(op.top()) != 9){
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
    
    printf("st size: %zu\n\n", st.size());
    output = calc(st);
   
    printf("output: %i\n\n", output);
   
    return 0;
}