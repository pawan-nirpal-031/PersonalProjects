#include <iostream>
using namespace std;

int foo(int *a,int n){
    int s = 0;
    for(int i =0;i<n;i++){
        s+=a[i];
    }
    return s;
}

int purr(int v){
    int t = 0;
    for(int i =0;i<v;i++){
        t+=i;
    }
    return t;
}

int boo(){
    int x =0;
    for(int i =0;i<100;i++)
        x+=i;
    return x;
}

int main(){
    int x = boo();
    x+=100;
    if(x > 100){
        int y =0;
        int *a = new int[x];
        for(int i = 0;i<x;i++){
            y+=purr(i*x);
            a[i] = y;
        }
        int x = foo(a,x);
        if(x < 100){
            return 1;
        }
    }
    return 0;
}