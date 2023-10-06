#include<iostream>
#include<iomanip>
#include<cmath>
using namespace std;
const double pi = 3.141592653589793238;
int main() {
	double h, d, angle;
	cin >> h >> d >> angle;
	h /= 12;                        //转换成英尺
	d = floor(d) + (d - floor(d)) * 12.0;//转换成英寸
	d /= 12;//转换成英尺
	cout << h + d * tan(angle * pi / 180) << endl;
}