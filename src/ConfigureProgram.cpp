//
// Created by shahrukhqasim on 6/28/16.
//

#include "ConfigureProgram.h"


string ConfigureProgram::getEnvironmentalVariable(const string& name) {
    const char * val = ::getenv( name.c_str() );
    if ( val == 0 ) {
        return "";
    }
    else {
        return val;
    }
}