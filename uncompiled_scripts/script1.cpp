//
// Created by shahrukhqasim on 6/24/16.
//

void convertPdfs() {
    ifstream inputStream("/home/shahrukhqasim/Desktop/bb/All/files.txt");
    std::string str;
    while (getline(inputStream, str)) {
        std::string withoutExtension=HelperMethods::removeFileExtension(str);

        std::string inputFileName="\"/home/shahrukhqasim/Desktop/bb/All/";
        inputFileName+=str;
        inputFileName+='"';

        std::string outputFileName="\"/home/shahrukhqasim/Desktop/bb/All/";
        outputFileName+=withoutExtension;
        outputFileName+=".png\"";
        std::string command="convert -density 300 "+inputFileName;
        command+=" -quality 100 ";
        command+=outputFileName;
        system(command.c_str());
    }
}

void runBatchJob() {
    ifstream inputStream("/home/shahrukhqasim/Desktop/bb/All/programInput/files.txt");
    std::string str;

    string inputFolder="/home/shahrukhqasim/Desktop/bb/All/programInput";
    string outputFolder="/home/shahrukhqasim/Desktop/bb/All/programOutput";
    while (getline(inputStream, str)) {
        cout<<"Running on "<<str<<endl;
        Program runner(str,inputFolder,outputFolder);
        runner.run();
        cout<<"Run complete on "<<str<<endl;
    }
}