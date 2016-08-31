#include "HelperMethods.h"
#include <iostream>
#include <regex>
using namespace cv;
using namespace std;

void HelperMethods::displayImageResizable(const Mat&image) {
	imwrite("temp/displayImage.png",image);
	system("xdg-open temp/displayImage.png");
}


void HelperMethods::outputImage(const Mat&image, const string& fileName) {
	imwrite(fileName,image);
}

string HelperMethods::removeFileExtension(const string &fileName) {
	int dotIndex=-1;
	for(int i= (int) (fileName.length() - 1); i >= 0; i--) {
		if(fileName[i]=='.') {
			dotIndex = i;
			break;
		}
	}

	if(dotIndex>0)
		return fileName.substr(0, (unsigned long) dotIndex);

	std::cerr<<"Error in file name:"<<fileName<<endl;
	exit(0);
}

bool HelperMethods::isAlphaNumericNotSpace(char a) {
	return (a >= '0') && ((a <= '9') || (a >= 'A')) && ((a <= 'Z') || (a >= 'a')) && (a <= 'z');

}

bool HelperMethods::nearEqualComparison(const string&a,const string&b) {
	string c;
	string d;
	for(unsigned long i=0;i<a.size();i++) {
		if(isAlphaNumericNotSpace(a[i]))
			c+=a[i];
	}
	for(unsigned long i=0;i<b.size();i++) {
		if(isAlphaNumericNotSpace(b[i]))
			d+=b[i];
	}

    for(unsigned long i=0;i<c.size();i++) {
        if(c[i]=='\n') {
            cout<<c<<endl;
            assert(false);
        }
    }

	return c==d;

}

std::vector<std::string> HelperMethods::regexSplit(const std::string &s, string rgx_str) {
	std::vector<std::string> elems;

	std::regex rgx (rgx_str);

	std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
	std::sregex_token_iterator end;

	while (iter != end)  {
		elems.push_back(*iter);
		++iter;
	}

	return elems;
}
