#include <iostream>
#include "digitRcognize.h"

using namespace std;


int main(){
	int PhoneNum[11];
	Mat image = imread("digit.jpg");
	int label = digitRecognize(image, PhoneNum);
	cout << label <<endl ;
	for(int i=0;i<11;i++)
		cout<<PhoneNum[i];
	cout << endl;
}