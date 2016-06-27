//
// Created by shahrukhqasim on 6/28/16.
//

#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

#ifndef K_1_PARSING2_CONFIGUREPROGRAM_H
#define K_1_PARSING2_CONFIGUREPROGRAM_H


class ConfigureProgram {
public:
    static string getEnvironmentalVariable(const string& name);

    static void runConfigureProgram();
};


#endif //K_1_PARSING2_CONFIGUREPROGRAM_H
