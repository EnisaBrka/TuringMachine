#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include "TuringMachine.h"

using namespace std;

void GenerateString(int n);
void Test(int& number);

void GenerateString(int n)
{
    ofstream stringInput;
    stringInput.open("inputed_string.txt", ios::out);
    if(!stringInput){
        cout << "File cannot be opened!!" << endl;
    }

    char i;
    for (int j=1; j<=n; j++){
        i=rand()%2;
        if (i==0){
            i='0';
        }else if(i==1){
            i='1';
        }
        stringInput<<i;
    }
    stringInput.close();
}


int main()
{
    srand(time(NULL));
    int number=rand()%10+4;
    int i,j;
    int string_length=1;
    string str;
    TuringMachine tm1,tm2;
    for(i=4; i<16;i++){
        tm1.SetNumberOfStates(number);
        tm1.Generate(number);
        tm1.all_states.resize(number);
        for(string_length=number; string_length<1001; string_length=string_length+100){
            for(j=1; j<11; j++){
                GenerateString(string_length);
                tm2.Test(tm1,i);
            }
        }
    }



    system("pause");
    return 0;
}
