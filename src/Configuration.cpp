//
// Created by shahrukhqasim on 6/28/16.
//

#include "Configuration.h"


string Configuration::getEnvironmentalVariable(const string& name) {
    const char * val = ::getenv( name.c_str() );
    if ( val == 0 ) {
        return "";
    }
    else {
        return val;
    }
}

void Configuration::configure() {
//    configuration=make_shared<Configuration>();
//    configuration->parentPath=getEnvironmentalVariable("FORMS_PATH");
//    if(configuration->parentPath[configuration->parentPath.length()-1]!='/')
//        configuration->parentPath=configuration->parentPath+"/";
}

//shared_ptr<Configuration> Configuration::getConfiguration() {
//    if(!configuration) {
//        cerr<<"Getting configuration before configuring"<<endl;
//    }
//    return configuration;

//}