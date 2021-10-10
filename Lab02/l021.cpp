#include "bits/stdc++.h"

using namespace std;
double(*results)[4] = new double[2][4];
double sign (double x1, double y1, double x2, double y2, double x3, double y3)
{
    return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
}
bool inTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y){
	double a = sign(x, y, x1, y1, x2, y2);
	double b = sign(x, y, x2, y2, x3, y3);
	double c = sign(x, y, x3, y3, x1, y1);
	bool neg = a < 0 || b < 0 || c < 0;
	bool pos = a > 0 || b > 0 || c > 0;
	return (pos and neg);
}
void toText(){
	ofstream myfile;
	myfile.open ("points.txt");
	myfile << setprecision(17);
	for(int i = 0; i < 4; i++){
		myfile << "(" << results[0][i] << ", " << results[1][i] << ")";
		if(i != 3){
			myfile <<", \n";
		} 
	}
	myfile.close();
}
void part1(){
	srand(time(0));
	cout << setprecision(17);  
	double x1 = rand() / double(RAND_MAX);
	double y1 = rand() / double(RAND_MAX);
	double x2 = rand() / double(RAND_MAX);
	double y2 = rand() / double(RAND_MAX);
	double x3 = rand() / double(RAND_MAX);
	double y3 = rand() / double(RAND_MAX);
	double points[4][4];
	int count = 0;
	cout << "Points: (" << x1 << ", " << y1 << ") (" << x2 << ", " << y2 << ") (" << x3 << ", " << y3 << ")\n";
	int runs = 0;
	while(count < 4){
		double x = rand() / double(RAND_MAX);
		double y = rand() / double(RAND_MAX);
		bool a = inTriangle(x1, y1, x2, y2, x3, y3, x, y);
		bool b = inTriangle(x, y, x1, y1, x2, y2, x3, y3);
		bool c = inTriangle(x3, y3, x, y, x1, y1, x2, y2);
		bool d = inTriangle(x2, y2, x3, y3, x, y, x1, y1);
		runs++;
		if(a and b and c and d){
			cout << "(" << x << ", " << y <<")\n";
			results[0][count] = x;
			results[1][count] = y;
			count++;
		}
		else if(runs == 100){
			cout << "unable to find point in " << runs << " runs\n";
		}
		else{
			continue; //unsuccessful point
		}
	}
	toText();


}
int main(){
	part1();
}