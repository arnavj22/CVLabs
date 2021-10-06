#include "bits/stdc++.h"

using namespace std;

double(*results)[4] = new double[2][4];
double(*inputs)[4] = new double[2][4];

const int size = 800;
int(*result)[size] = new int[size][size];
int(*ppm)[size] = new int[size*3][size];
void set_pixel(int x, int y){
	if(x > size or x < 0 or y > size or y < 0){
		return;
	}
	result[x][y] = 1;
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
	void drawCircle(double radius1){
		double a1 = x;
		double b1 = y;
		int a = a1 * 800;
		int b = b1 * 800;
		int radius = 2;
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
			set_pixel (x + a, y + b);
			set_pixel (x + a, -y + b);
			set_pixel (-x + a, y + b);
			set_pixel (-x + a, -y + b);	
			set_pixel (y + a, x + b);	
			set_pixel (y + a, -x + b);
			set_pixel (-y + a, x + b);
			set_pixel (-y + a, -x + b);
			y2_new -= (2 * x) - 3;
		}
	};
};
class Line{
	public:
	Point p1;
	Point p2;
	Line(Point x1, Point y1){
		p1 = x1;
		p2 = y1;
	};
	Line(){
		p1 = Point();
		p2 = Point();
	};
	void drawLine(){
		double x1d = p1.x;
		double y1d = p1.y;
		double x2d = p2.x;
		double y2d = p2.y;
		int x1 = x1d * 800;
		int y1 = y1d * 800;
		int x2 = x2d * 800;
		int y2 = y2d * 800;
		int dx = x2 - x1;
		int dy = y2 - y1;


		if(dx <= 0 and dy <= 0 or (dx <= 0 and dy > 0)){
			dx = -1 * (dx); // 150
			dy = -1 * (dy); // -300
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		int j = y1;
		int c = dy - dx;
		if(abs(dx) > abs(dy)){
			if(dy < 0){
				for(int i = x1; i < x2; i++){
					set_pixel(i, j);
					if(c > 0){
						j -= 1;
						c -= dx;
					}
					c -= dy;
				}
			}
			else{
				for(int i = x1; i < x2; i++){
					set_pixel(i, j);
					if(c > 0){
						j += 1;
						c -= dx;
					}
					c += dy;
				}
			}
		}
		else{
			j = x1;
			int c = dx - dy;
			if(dy < 0){
				for(int i = y1; i > y2; i--){
					set_pixel(j, i);
					if(c > 0){
						j += 1;
						c += dy;
					}
					c += dx;
				}
			}
			else{
				for(int i = y1; i < y2; i++){
					set_pixel(j, i);
					if(c > 0){
						j += 1;
						c -= dy;
					}
					c += dx;
				}
			}
		}
	}
	void drawLineExtended(){
		if(p1.x - p2.x == 0){
			drawLine();
		}
		else{
			double slope = (p2.y - p1.y) / (p2.x - p1.x);
			double point1y = slope * 0 - (slope * p1.x) + p1.y;
			double point2y = slope * 0.99 - (slope * p1.x) + p1.y;
			Line(Point(0, point1y), Point(0.99, point2y)).drawLine();  
		}
	}
	vector<Point> findPoint(double d, Point p){ // finds a point d distance away from a point on a line
		vector<Point> o;
		double dy = p2.y - p1.y;
		double dx = p2.x - p1.x;
		double magnitude = sqrt(pow(dx, 2) + pow(dy, 2));
		dy = dy / magnitude;
		dx = dx / magnitude;
		o.push_back(Point(p.x + (dx * d), p.y + (dy * d)));
		o.push_back(Point(p.x - (dx * d), p.y - (dy * d)));
		return o;
	}
	Line findPerp(Point p){ // finds the perpendicular to a line given a point p
		double dy = -1 * (p2.x - p1.x);
		double dx = p2.y - p1.y;
		double magnitude = sqrt(pow(dx, 2) + pow(dy, 2));
		dy = 20 * dy / magnitude;
		dx = 20 * dx / magnitude;
		Point o = Point(p.x + (dx), p.y + (dy));
		return Line(p, o);
	}
	vector<double> getEq(){
		vector<double> v;
		double dx = p2.x - p1.x;
		double dy = p2.y - p1.y;
		if(dx == 0){
			v.push_back(1);
			v.push_back(0);
			v.push_back(p1.x);
			return v;
		}
		double m = dy / dx;
		v.push_back(-1 * m);
		v.push_back(1);
		v.push_back((-1 * m * p1.x) + p1.y);

		return v;
	}
	Point intersection(Line l){
		vector<double> v1 = getEq();
		vector<double> v2 = l.getEq();
		double a1 = v1[0];
		double b1 = v1[1];
		double c1 = v1[2];
		double a2 = v2[0];
		double b2 = v2[1];
		double c2 = v2[2];
		double x = (c1*b2 - c2*b1) /(a1*b2 - b1*a2); // Cramers Rule
		double y = (c1*a2 - c2*a1) /(b1*a2 - b2*a1); // Cramers Rule
		return Point(x, y);
	}
	double distance(){
		return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
	}
};
Point ps[4];
Point(*squares)[4] = new Point[6][4];
void get_points(){
	string line;
	ifstream po ("points.txt");
	int nums = 0;
	if (po.is_open()){
		while (getline(po,line, ',')){
			line.erase(remove(line.begin(), line.end(), ')'), line.end());
			line.erase(remove(line.begin(), line.end(), '('), line.end());
			line.erase(remove(line.begin(), line.end(), ' '), line.end());
			line.erase(remove(line.begin(), line.end(), '\n'), line.end());

      //cout << line << "\n";
			double num1 = stod(line);
      //cout << (num1) << "\n";
			inputs[nums % 2][nums / 2] = num1;
			nums += 1;
		}
		po.close();
	}
	for(int i = 0; i < 4; i++){
		ps[i] = Point(inputs[0][i], inputs[1][i]);
	}
}
void arrToFile(){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			ppm[i * 3][j] = result[i][j];
			ppm[i * 3 + 1][j] = result[i][j];
			ppm[i * 3 + 2][j] = result[i][j];
		}
	}
	ofstream myfile;
	myfile.open ("output.ppm");
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
double sign (double x1, double y1, double x2, double y2, double x3, double y3){
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
void squareToFile(){
	ofstream myfile;
	myfile.open ("output.txt");
	myfile << setprecision(17);
	myfile << fixed;
	for(int i = 0; i < 4; i++){
		myfile << "(" << ps[i].x << "," << ps[i].y << ")";
		if(i != 3){
			myfile <<" , ";
		} 
	}
	myfile << "\n";
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 4; j++){
			myfile << "(" << squares[i][j].x << "," << squares[i][j].y << ")";
			if(j != 3){
				myfile <<" , ";
			} 
		}
		Point p1 = squares[i][0];
		Point p2 = squares[i][1];
		myfile << " Area = " << Line(p1, p2).distance() * Line(p1, p2).distance() << "\n";
	}
	myfile.close();
}
void toText(){
	ofstream myfile;
	myfile.open ("points.txt");
	myfile << setprecision(17);
	myfile << fixed;
	for(int i = 0; i < 4; i++){
		myfile << "(" << results[0][i] << "," << results[1][i] << ")";
		if(i != 3){
			myfile <<" , ";
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
void part2(){
	get_points();
	for(int i = 0; i < 4; i++){
		ps[i].drawCircle(2/800);
	}
	double mx = INT_MAX;
	Line minl1;
	Line minl2;
	Line minl3;
	Line minl4;
	Line l = Line(ps[0], ps[1]);
	//l.drawLineExtended();
	Line perp = l.findPerp(ps[2]);
	//perp.drawLineExtended();
	double dist = l.distance();
 	vector<Point> twoLines  = perp.findPoint(dist, ps[2]);
 	for(int j = 0; j < 2; j++){
 		Line l1 = Line(twoLines[j], ps[3]);
 		Line l2 = l1.findPerp(ps[0]);
 		Line l3 = l1.findPerp(ps[1]);
 		Line l4 = l2.findPerp(ps[2]);

 		Point p1 = l1.intersection(l2);
 		Point p2 = l1.intersection(l3);
 		Point p3 = l3.intersection(l4);
 		Point p4 = l4.intersection(l2);
 		squares[j][0] = p1;
 		squares[j][1] = p2;
 		squares[j][2] = p3;
 		squares[j][3] = p4;
 		if((Line(p1, p2).distance() * Line(p1, p2).distance()) < mx){
 			mx = Line(p1, p2).distance() * Line(p1, p2).distance();
 			minl1 = l1;
 			minl2 = l2;
 			minl3 = l3;
 			minl4 = l4;
 		}
 	}
 	l = Line(ps[0], ps[2]);
	//l.drawLineExtended();
	perp = l.findPerp(ps[3]);
	//perp.drawLineExtended();
	dist = l.distance();
 	twoLines  = perp.findPoint(dist, ps[3]);
 	for(int j = 0; j < 2; j++){
 		Line l1 = Line(twoLines[j], ps[1]);
 		Line l2 = l1.findPerp(ps[0]);
 		Line l3 = l1.findPerp(ps[2]);
 		Line l4 = l2.findPerp(ps[3]);
 		Point p1 = l1.intersection(l2);
 		Point p2 = l1.intersection(l3);
 		Point p3 = l3.intersection(l4);
 		Point p4 = l4.intersection(l2);
 		squares[2 + j][0] = p1;
 		squares[2 + j][1] = p2;
 		squares[2 + j][2] = p3;
 		squares[2 + j][3] = p4;
 		if((Line(p1, p2).distance() * Line(p1, p2).distance()) < mx){
 			mx = Line(p1, p2).distance() * Line(p1, p2).distance();
 			minl1 = l1;
 			minl2 = l2;
 			minl3 = l3;
 			minl4 = l4;
 		}
 	}
 	l = Line(ps[0], ps[3]);
	//l.drawLineExtended();
	perp = l.findPerp(ps[1]);
	//perp.drawLineExtended();
	dist = l.distance();
 	twoLines  = perp.findPoint(dist, ps[1]);
 	for(int j = 0; j < 2; j++){
 		Line l1 = Line(twoLines[j], ps[2]);
 		Line l2 = l1.findPerp(ps[0]);
  		Line l3 = l1.findPerp(ps[3]);
  		Line l4 = l2.findPerp(ps[1]);

 		Point p1 = l1.intersection(l2);
 		Point p2 = l1.intersection(l3);
 		Point p3 = l3.intersection(l4);
 		Point p4 = l4.intersection(l2);
 		squares[4 + j][0] = p1;
 		squares[4 + j][1] = p2;
 		squares[4 + j][2] = p3;
 		squares[4 + j][3] = p4;
 		if((Line(p1, p2).distance() * Line(p1, p2).distance()) < mx){
 			mx = Line(p1, p2).distance() * Line(p1, p2).distance();
 			minl1 = l1;
 			minl2 = l2;
 			minl3 = l3;
 			minl4 = l4;
 		}
 	}
 	minl1.drawLineExtended();
 	minl2.drawLineExtended();
	minl3.drawLineExtended();
 	minl4.drawLineExtended();
 	
	arrToFile();
	squareToFile();
}
int main(){
	//part1();
	part2();
}