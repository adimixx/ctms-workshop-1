//
// Created by Adi Iman on 25/04/2020.
//

#ifndef WORKSHOP1_DEPENDENCY_H
#define WORKSHOP1_DEPENDENCY_H

#include <thread>
#include <chrono>
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

class Dependency {
public:
    static void EndLine(){
        cout<<endl;
    }

    static void ClearScreen(string HeaderText){
        cout<<"\033[2J\033[1;1H";
        cout<<"\e[1m" << "\e[36m"<< HeaderText << "\e[39;49m"<< "\e[0m"<<endl;
        EndLine();
    };
    static void SleepCommand(int sec){
        this_thread::sleep_for(chrono::milliseconds(sec));
    };
};


#endif //WORKSHOP1_DEPENDENCY_H
