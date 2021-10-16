#include "bits/stdc++.h"

using namespace std;
const int size = 800;
int(*result)[size] = new int[size][size];
int(*ppm)[size] = new int[size*3][size];
void set_pixel(int x, int y, int c = 1){
	if(x > size or x < 0 or y > size or y < 0){
		return;
	}
	result[x][y] = c;
}
class Point{
	public:
	double x;
	double y;
	Point(double x1, double y1){
		x = x1;
		y = y1;
	};
	Point(){
		x = 0;
		y = 0;
	};
	void plot(){
		set_pixel(x, y);
	}
	double distance(Point p){
		return sqrt(pow((x - p.x), 2) + pow((y - p.y), 2));
	}
	void drawCircle(double radius1, int c = 1){ // 1 = black, 2 = red
		double a1 = x;
		double b1 = y;
		int a = a1 * 800;
		int b = b1 * 800;
		int radius = radius1 * 800;
		int xmax = (int)(radius * 0.70710678);
		int y = radius;
		int y2 = y * y;
		int ty = (2 * y) - 1;
		int y2_new = y2;
		for (int x = 0; x <= xmax + 1; x++) {
			if ((y2 - y2_new) >= ty) {
				y2 -= ty;
				y -= 1;
				ty -= 2;
			}
			set_pixel (x + a, y + b, c);
			set_pixel (x + a, -y + b, c);
			set_pixel (-x + a, y + b, c);
			set_pixel (-x + a, -y + b, c);	
			set_pixel (y + a, x + b, c);	
			set_pixel (y + a, -x + b, c);
			set_pixel (-y + a, x + b, c);
			set_pixel (-y + a, -x + b, c);
			y2_new -= (2 * x) - 3;
		}
	};
};
list<Point> ps;
void gen_points(){
	ofstream myfile;
	myfile.open("points.txt");
	myfile << setprecision(23);
	myfile << fixed;
	srand(time(0));
	for(int i = 0; i < 60; i++){
		Point p = Point(rand() / double(RAND_MAX), rand() / double(RAND_MAX));
		//ps.push_back(p);

		myfile << p.x << "  " << p.y << "\n";
	}
}
void get_points()
{
	ps.clear();
	string line;
	ifstream po("points.txt");
	if (po.is_open())
	{
		while (getline(po, line, '\n'))
		{
			int pos = line.find(" ");
			double x = stod(line.substr(0, pos));
			double y = stod(line.substr(pos));
			//cout << x << " " << y;
			Point p = Point(x, y);
			p.drawCircle(3.0 / 800.0);
			ps.push_back(p);
		}
	}
}
pair<Point, Point> brute_force()
{
	double min = INT_MAX;
	Point a;
	Point b;
	for (list<Point>::iterator i = ps.begin(); i != ps.end(); i++)
	{
		for (list<Point>::iterator j = next(i); j != ps.end(); j++)
		{
			double dist = (*i).distance(*j);
			if (dist < min)
			{
				min = dist;
				a = *i;
				b = *j;
			}
		}
	}
	pair<Point, Point> out;
	a.drawCircle(2.0 / 800.0, 2);
	b.drawCircle(2.0 / 800.0, 2);
	a.drawCircle(3.0 / 800.0, 2);
	b.drawCircle(3.0 / 800.0, 2);
	out.first = a;
	out.second = b;
	return out;
}

void arrToFile()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (result[i][j] == 2)
			{
				ppm[i * 3][j] = 1;
				ppm[i * 3 + 1][j] = 0;
				ppm[i * 3 + 1][j] = 0;
			}
			else if (result[i][j] == 1)
			{
				ppm[i * 3][j] = 0;
				ppm[i * 3 + 1][j] = 0;
				ppm[i * 3 + 2][j] = 0;
			}
			else
			{
				ppm[i * 3][j] = 1;
				ppm[i * 3 + 1][j] = 1;
				ppm[i * 3 + 2][j] = 1;
			}
		}
	}
	ofstream myfile;
	myfile.open("points.ppm");
	myfile << "P3 " << size << " " << size <<" 1\n";
	for(int j = 0; j < size; j++){
		for(int i = 0; i < size; i++){
			myfile << ppm[i * 3][j] << " ";
			myfile << ppm[i * 3 + 1][j] << " ";
			myfile << ppm[i * 3 + 2][j] << " ";
		}
		myfile << "\n";
	}
	myfile.close();
}
void part1(){
	gen_points();
	get_points();
	brute_force();
	//pair<Point, Point> smalldist = brute_force();
	arrToFile();
}

int main(){
	part1();
}