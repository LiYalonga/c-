#include<iostream>
#include<iomanip>
#include<cmath>
using namespace std;
const double pi = 3.141592653589793238;
int main() {
	cout << "please cin r and n: ";
	double r;
	int n;
	cin >> r>>n;
	cout <<fixed<<setprecision(n)<<pi * r * r << endl;
}