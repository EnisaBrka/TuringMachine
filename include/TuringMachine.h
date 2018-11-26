#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
using namespace std;

class TuringMachine
{
    public:
        TuringMachine();
        virtual ~TuringMachine();

        void SetNumberOfStates(int);

        void InputState();

        bool CheckSymbol(char);
        bool CheckState(char);
        bool CheckShifting(char);
        void Transition();


        void Simulate(TuringMachine, string);

        int GetPosition(string);
        char GetState(string);
        char GetSymbol(string);
        string LastConfiguration();
        string FindState(char, char, int);
        void Generate(int);
        char Shift();
        char Symbol();
        char State();
        void Test(TuringMachine,int);
        vector<char> all_states;

    protected:

    private:
        char q_b;
        char q_a;
        char q_r;
        int number_of_states;
        vector<char> states;

        vector<char> alphabet;
};

#endif // TURINGMACHINE_H
