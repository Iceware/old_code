#include<fstream>
#include<iostream>
#include<string>
using namespace std;

int main(){
ifstream fs("pure.cpp");
string s;
while(fs>>s)
cout<<s<<'\n';

}
