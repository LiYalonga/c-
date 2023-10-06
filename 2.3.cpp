#include<iostream>
#include<iomanip>
#include<cmath>
using namespace std;
const double pi = 3.141592653589793238;
int main() {
	cout << "输入英寸: ";
	int n;
	cin >>n;
	cout << n / 12 << "英尺" << n % 12 << "英寸" << endl;
}