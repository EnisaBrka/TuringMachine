#include "TuringMachine.h"
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

TuringMachine::TuringMachine()
{
    //ctor
    q_b='b';
    q_a='a';
    q_r='r';
    number_of_states=3;
    alphabet.resize(3);
    alphabet[0]='0';
    alphabet[1]='1';
    alphabet[2]='*';

}

TuringMachine::~TuringMachine()
{
    //dtor
}

void TuringMachine::SetNumberOfStates(int n)
{
    number_of_states=n;
}

void TuringMachine::InputState()
{
    cout<<"Input number  of states equal or larger then 3: "<<'\t';
    cin>>number_of_states;
    if (number_of_states<3){
        cout<<"Input number  of states equal or larger then 3: "<<flush;
        cin>>number_of_states;
    }

    states.resize(number_of_states-3);
    all_states.resize(number_of_states);

    ofstream inputFile;
    inputFile.open("states.txt",ios::out);

        cout<<"Input accept state: "<<flush;
        cin>>q_a;
        inputFile<<q_a;
        all_states[0]=q_a;

        cout<<"Input reject state different then accept state: "<<flush;
        cin>>q_r;
        if(q_a==q_r){
            cout<< "Input reject state different then accept state: "<<endl;
            cin>>q_r;
            }
        inputFile<<q_r;
        all_states[1]=q_r;

        cout<<"Input start state: "<<flush;
        cin>>q_b;
        inputFile<<q_b;
        all_states[2]=q_b;

    if(number_of_states>3){
        for(vector<char>::size_type i=0; i<number_of_states-3; i++){
            cout<<"Input state #"<<i+1<<": "<<flush;
            cin>>states[i];
            inputFile<<states[i];
            all_states[i+3]=states[i];
            }
    }
    inputFile.close();
}


bool TuringMachine::CheckSymbol(char symbol)
{
    if(find(alphabet.begin(),alphabet.end(),symbol)!=alphabet.end()) return true;
    return false;
}

bool TuringMachine::CheckState(char state)
{
    if(find(all_states.begin(),all_states.end(),state)!=all_states.end())return true;
    return false;
}

bool TuringMachine::CheckShifting(char shift)
{
    if(shift=='R' || shift=='r' || shift=='L' || shift=='l' || shift=='S' || shift=='s')return true;
    return false;
}


void TuringMachine::Transition()
{
    char new_symbol;
    char new_state;
    char shifting;

    ofstream removeData;
    removeData.open("transition_function.txt", ios::trunc);
    removeData.close();

    ofstream inputFile;
    inputFile.open("transition_function.txt", ios::out | ios::app);

    for(int j=2; j<number_of_states; j++){
        for(int k=0; k<3; k++){
            cout<<"f(current state, current symbol) = (new state, new symbol, shifting): "<<endl;
            cout<<"("<<all_states[j]<<","<<alphabet[k]<<") = ";
            cin>>new_state;
            while (CheckState(new_state)==false){
                cout<<"Input state from state convolution: ";
                cin>>new_state;
            }

            cin>>new_symbol;
            while (CheckSymbol(new_symbol)==false){
                cout<<"Input symbol from given alphabet: ";
                cin>>new_symbol;
            }

            cin>>shifting;
            while (CheckShifting(shifting)==false){
            cout<<"Input R, L or S to shift right, left or stay on same place: ";
            cin>>shifting;
            }
            if(alphabet[k]=='*'){
                inputFile<<"f("<<all_states[j]<<","<<alphabet[k]<<")=("<<new_state<<",*,"<<shifting<<")"<<endl;
            }else{
                inputFile<<"f("<<all_states[j]<<","<<alphabet[k]<<")=("<<new_state<<","<<new_symbol<<","<<shifting<<")"<<endl;
            }
        }
    }

    inputFile.close();

}


void TuringMachine::Simulate(TuringMachine tm, string omega)
{

    string beg_conf, end_conf, begining;
    char current_state, current_symbol, new_state, new_symbol, shifting;
    int head_position;
    string transition;

    cout<<endl<<endl<<"-----------------------"<<endl;
    cout<<"Simulation of inputed Turing machine for given string: "<<endl<<endl;

    ofstream conf;
    conf.open("configurations.txt", ios::out);
    cout<<"Begining state of the tape: "<<'\t';
    for(int i=0; i<omega.size()+3;i++){
        cout<<"*";
        conf<<"*";
    }
    cout<<endl<<endl<<"-----------------------"<<endl;
    conf<<endl;

    ifstream stateFile;
    string str;
    stateFile.open("states.txt");
    getline(stateFile,str);
    number_of_states=str.size();
    q_a=str.at(0);
    q_r=str.at(1);
    q_b=str.at(2);

    stateFile.close();

    beg_conf=q_b+omega;
    begining='*'+beg_conf+'*';
    conf<<begining<<endl;
    conf.close();
    for(int i=0; i++; i<tm.all_states.size()){
        cout<<"state pos: "<<i<<" state: "<<tm.all_states[i]<<endl;
    }

    current_state=tm.all_states[2];
    while(current_state!=q_a && current_state!=q_r)
    {
        end_conf=LastConfiguration();
        current_state=GetState(end_conf);
        current_symbol=GetSymbol(end_conf);
        head_position=GetPosition(end_conf);

        cout<<"Transition function that contains current state and current symbol:"<<endl;
        transition=FindState(current_state,current_symbol,tm.number_of_states);
        cout<<transition<<endl;

        new_state=transition.at(8);
        new_symbol=transition.at(10);
        shifting=transition.at(12);

        conf.open("configurations.txt", ios::out | ios::app);

        cout<<endl;

        string temp1=end_conf.substr(0,head_position-1);
        string temp2=end_conf.substr(head_position+1);

        if((shifting=='R' || shifting=='r')&& head_position==omega.size()+2){
            temp2=end_conf.substr(head_position);
            cout<<temp1<<new_state<<temp2<<endl;
            conf<<temp1<<new_state<<temp2<<endl;
        }
        else if(shifting=='R' || shifting=='r'){
            cout<<temp1<<new_symbol<<new_state<<temp2<<endl;
            conf<<temp1<<new_symbol<<new_state<<temp2<<endl;
        }
        else if((shifting=='L' || shifting=='l')&& head_position==2){
            cout<<new_state<<temp1<<new_symbol<<temp2<<endl;
            conf<<new_state<<temp1<<new_symbol<<temp2<<endl;
        }
        else if((shifting=='L' || shifting=='l')&& head_position!=2){
            temp1=end_conf.substr(0,head_position-2);
            cout<<temp1<<new_state<<end_conf.substr(head_position-2,1)<<new_symbol<<temp2<<endl;
            conf<<temp1<<new_state<<end_conf.substr(head_position-2,1)<<new_symbol<<temp2<<endl;
        }
        else if(shifting=='S' || shifting=='s'){
            cout<<temp1<<new_state<<new_symbol<<temp2<<endl;
            conf<<temp1<<new_state<<new_symbol<<temp2<<endl;
        }

        end_conf=LastConfiguration();
        current_state=GetState(end_conf);
        current_symbol=GetSymbol(end_conf);
        head_position=GetPosition(end_conf);

       if (current_state==q_a && current_symbol=='*'){
            cout<<"Inputed string has been accpetd!"<<endl;
            conf<<"---------------------------"<<endl;
            conf<<"Inputed string has been accpetd!"<<endl;
            cout<<"---------------------------"<<endl;
        }
        else if (current_state==q_a && current_symbol!='*'){
            cout<< "Turing machine has come to accpeted state, but string was not read in total."<<endl;
            conf<<"---------------------------"<<endl;
            conf<< "Turing machine has come to accpeted state, but string was not read in total."<<endl;
            cout<<"---------------------------"<<endl;
        }
        if(current_state==q_r && current_symbol=='*'){
            cout<<"Inputed string has been rejected!"<<endl;
            conf<<"---------------------------"<<endl;
            conf<<"Inputed string has been rejected!"<<endl;
            cout<<"---------------------------"<<endl;
        }
        else if (current_state==q_r && current_symbol!='*'){
            cout<< "Turing machine has come to rejected state, but string was not read in total."<<endl;
            conf<<"---------------------------"<<endl;
            conf<< "Turing machine has come to rejeted state, but string was not read in total."<<endl;
            cout<<"---------------------------"<<endl;
        }

        conf.close();
    }

}

char TuringMachine::GetState(string alfa){
    int n,position;
    char current_state;
    n=alfa.find_first_not_of("*01");
    if(n!=string::npos){
        current_state=alfa[n];
        position=int(n);
    }
    return current_state;
}

int TuringMachine::GetPosition(string alfa){
    int n,position;
    char current_state;
    n=alfa.find_first_not_of("*01");
    if(n!=string::npos){
        current_state=alfa[n];
        position=int(n);
    }
    return position+1;
}

char TuringMachine::GetSymbol(string alfa){
    int n,position;
    char current_state;
    char current_symbol;
    n=alfa.find_first_not_of("*01");
    if(n!=string::npos){
        current_state=alfa[n];
        position=int(n);
        current_symbol=alfa.at(position+1);
    }
    return current_symbol;
}


string TuringMachine::LastConfiguration(){
    string line;
    string last_line;
    ifstream conf;
    conf.open("configurations.txt");
    if(conf.is_open())
    {
        bool isEmpty=true;
        while(getline(conf,line))
        {
            for(int i=0; i<line.size();i++)
            {
                char ch=line[i];
                isEmpty=isEmpty && isspace(ch);
            }
            if(!isEmpty){
                last_line=line;
            }
        }
        conf.close();
    return last_line;
    }
    else{
        cout<<"Can't open the document."<<endl;
    }
}

string TuringMachine::FindState(char state,char symbol, int number_of_states)
{
    ifstream transition;
    transition.open("transition.txt");
    int find_state,find_symbol;
    string line,first_state,first_symbol;
    string str[(number_of_states-2)*3];
    for (int i=0; i<(number_of_states-2)*3; i++)  {
    getline(transition,str[i]);
    first_state=str[i].at(2);
    first_symbol=str[i].at(4);
    find_state=first_state.find_first_of(state);
    find_symbol=first_symbol.find_first_of(symbol) ;

        if(find_state!=string::npos){
            if(find_symbol!=string::npos){
            line=str[i];
            }
        }
    }
    transition.close();
    return line;
}

void TuringMachine::Generate(int number)
{
    ofstream st;
    st.open("states.txt");
    if (!st) {
        cout << "File cannot be opened!" << endl;
    }

    char state;
    for(int k=0; k<number; k++){
        state=(rand()% 26)+'a';
        st<<state;
    }
    st.close();
    string str;

    ofstream output;
    output.open("transition.txt");
    if (!output) {
        cout << "File cannot be opened!" << endl;
    }
    ifstream inputed_states;
    inputed_states.open("states.txt");
    if (!inputed_states) {
        cout << "File cannot be opened!" << endl;
    }

    getline(inputed_states,str);
    q_a=str.at(0);
    q_r=str.at(1);
    q_b=str.at(2);
    for(int k=2; k<number;k++){
        cout<<"f("<<str.at(k)<<",0)=("<<State()<<","<<Symbol()<<","<<Shift()<<")"<<endl;
        output<<"f("<<str.at(k)<<",0)=("<<State()<<","<<Symbol()<<","<<Shift()<<")"<<endl;
        cout<<"f("<<str.at(k)<<",1)=("<<State()<<","<<Symbol()<<","<<Shift()<<")"<<endl;
        output<<"f("<<str.at(k)<<",1)=("<<State()<<","<<Symbol()<<","<<Shift()<<")"<<endl;
        cout<<"f("<<str.at(k)<<",*)=("<<State()<<",*,"<<Shift()<<")"<<endl;
        output<<"f("<<str.at(k)<<",*)=("<<State()<<",*,"<<Shift()<<")"<<endl;
    }

    inputed_states.close();
    output.close();
}


char TuringMachine::Shift()
{
    char m=rand()%3;
    if (m==0){
           m='S';
    }else if(m==1){
           m='R';
    }else if(m==2){
            m='L';
    }
    return m;
}
char TuringMachine::Symbol()
{
    char i=rand()%2;
    if (i==0){
        i='0';
    }else if(i==1){
        i='1';
    }
    return i;
}

char TuringMachine::State(){
    ifstream inputed_states;
    inputed_states.open("states.txt");
    if (!inputed_states) {
        cout << "File cannot be opened!" << endl;
    }
    string str;
    char state_char;
    getline(inputed_states,str);
    for(int k=0; k<str.size();k++){
        state_char=str[rand()%(str.size()-1)];
    }
    inputed_states.close();
    return state_char;
}

void TuringMachine::Test(TuringMachine tm1,int number)
{
    ofstream test;
    test.open("test_results.txt",ios::out | ios::app);
    if(!test) {
        cout << "File cannot be opened!" << endl;
    }
    test<<"Test results: "<<endl<<endl;

    ifstream input;
    input.open("inputed_string.txt");
    if(!input) {
        cout << "File cannot be opened!" << endl;
    }

    ofstream numbers("table.txt", ios::out | ios::app);
    if(!numbers) {
        cout << "File cannot be opened!" << endl;
    }

    string str;
    getline(input,str);
    int string_length=str.size();
    int start_time=clock();
    TuringMachine tm;
    tm=tm1;
    tm.Simulate(tm1,str);

    int end_time=clock();
    cout<<"Execution time for "<<number<<" states and string length "<<string_length<<" is "<<double(end_time-start_time)/CLOCKS_PER_SEC<<endl;
    test<<"Execution time for "<<number<<" states and string length "<<string_length<<" is "<<double(end_time-start_time)/CLOCKS_PER_SEC<<endl;
    test<<endl<<"-------------------------------------"<<endl<<endl;
    numbers<<number<<'\t'<<string_length<<'\t'<<double(end_time-start_time)/CLOCKS_PER_SEC<<endl;
    numbers.close();
    input.close();
    test.close();

}
