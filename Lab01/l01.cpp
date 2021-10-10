#include "bits/stdc++.h"

using namespace std;

const int size = 800;
int(*result)[size] = new int[size][size];
int(*ppm)[size] = new int[size*3][size];

pair<double, double> inCircleCenter(double x1, double x2, double x3, double y1, double y2, double y3, double a, double b, double c){

    double x = (a * x1 + b * x2 + c * x3) / (a + b + c);
    double y = (a * y1 + b * y2 + c * y3) / (a + b + c);
    pair<double, double> coords;
    coords.first = x;
    coords.second = y;
    return coords;   
}
pair<double, double> circumCircleCenter(double x1, double x2, double x3, double y1, double y2, double y3){
	double perpslope1, perpslope2, bisector1x, bisector1y, bisector2x, bisector2y;
	if(y2 - y1 == 0){
		bisector1x = (x3 + x1) / 2;
		bisector1y = (y3 + y1) / 2;
		bisector2x = (x3 + x2) / 2;
		bisector2y = (y3 + y2) / 2;
		perpslope1 =  -1 * (x3 - x1)/(y3 - y1);
		perpslope2 = -1 * (x3 - x2)/(y3 - y2); // (y - midy) = slope(x - x1), y = slope * x - (slope * midx) + midy
	}
	else if(y3 - y2 == 0){
		bisector1x = (x3 + x1) / 2;
		bisector1y = (y3 + y1) / 2;
		bisector2x = (x1 + x2) / 2;
		bisector2y = (y1 + y2) / 2;
		perpslope1 =  -1 * (x3 - x1)/(y3 - y1);
		perpslope2 = -1 * (x1 - x2)/(y1 - y2); // (y - midy) = slope(x - x1), y = slope * x - (slope * midx) + midy
	}
	else{
		bisector1x = (x2 + x1) / 2;
		bisector1y = (y2 + y1) / 2;
		bisector2x = (x3 + x2) / 2;
		bisector2y = (y3 + y2) / 2;
		perpslope1 =  -1 * (x2 - x1)/(y2 - y1);
		perpslope2 = -1 * (x3 - x2)/(y3 - y2); // (y - midy) = slope(x - x1), y = slope * x - (slope * midx) + midy
	}
	double a1 = -1 * perpslope1;	
	double b1 = 1;
	double c1 = -1 * perpslope1 * bisector1x + bisector1y;

	double a2 = -1 * perpslope2;
	double b2 = 1;
	double c2 = -1 * perpslope2 * bisector2x + bisector2y;

	double x = (c1*b2 - c2*b1) /(a1*b2 - b1*a2); // Cramers Rule
	double y = (c1*a2 - c2*a1) /(b1*a2 - b2*a1); // Cramers Rule
    pair<double, double> coords;
    coords.first = x;
    coords.second = y;
    return coords;
}
void set_pixel(int x, int y){
	if(x > size or x < 0 or y > size or y < 0){
		return;
	}
	result[x][y] = 1;
}
void drawLine(double x1d, double y1d, double x2d, double y2d){
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
void drawCircle(double a1, double b1, double radius1){
	int a = a1 * 800;
	int b = b1 * 800;
	int radius = radius1 * 800;
	int xmax = (int)(radius * 0.70710678);
	int y = radius;
	int y2 = y * y;
	int ty = (2 * y) - 1;
	int y2_new = y2;
	for (int x = 0; x <= xmax + 2; x++) {
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
}
void drawEulerLine(double x1, double y1, double x2, double y2){
	if(x2 - x1 == 0){
		drawLine(x1, y1, x2, y2);
	}
	else{
		double slope = (y2 - y1) / (x2 - x1);
		double point1y = slope * 0 - (slope * x1) + y1;
		double point2y = slope * 0.99 - (slope * x1) + y1;
		drawLine(0, point1y, 0.99, point2y);  
	}
}
void drawNinePointCircle(double ccenterx, double ccentery, double ccr){
	drawCircle(ccenterx, ccentery, ccr);  
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
	myfile.open ("triangle.ppm");
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
int main() {
	srand(time(0));

	double x1 = rand() / double(RAND_MAX);
	double y1 = rand() / double(RAND_MAX);
	double x2 = rand() / double(RAND_MAX);
	double y2 = rand() / double(RAND_MAX);
	double x3 = rand() / double(RAND_MAX);
	double y3 = rand() / double(RAND_MAX);
	/*
	x1 = 0.00125126;
	y1 = 0.563585;
	x2 = 0.193304;
	y2 = 0.808741;
	x3 = 0.585009;
	y3 = 0.479873;*/
	

    double a = sqrt((x3 - x2)*(x3 - x2) + (y3 - y2)*(y3 - y2));
    double b = sqrt((x3 - x1)*(x3 - x1) + (y3 - y1)*(y3 - y1));
	double c = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));

	//drawLine(0.1, 0.1, 0.9, 0.9);
	
    double semi = (a + b + c)/2;
    pair<double, double> ic_center =  inCircleCenter(x1, x2, x3, y1, y2, y3, a, b, c);
    double ic_radius = sqrt(((semi-a)*(semi-b)*(semi-c))/semi);
    pair<double, double> cc_center =  circumCircleCenter(x1, x2, x3, y1, y2, y3);
    double cc_radius = (a*b*c) / (4 * ic_radius * semi);

    drawLine(x1, y1, x2, y2);
    drawLine(x3, y3, x2, y2);
    drawLine(x3, y3, x1, y1);
    cout << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3;

    drawCircle(ic_center.first, ic_center.second, ic_radius);
    drawCircle(cc_center.first, cc_center.second, cc_radius);
    
    pair<double, double> npc_center =  circumCircleCenter((x1 + x2) / 2, (x2 + x3) / 2, (x1 + x3) / 2, (y1 + y2) / 2, (y2 + y3) / 2, (y1 + y3) / 2);
    drawEulerLine(npc_center.first, npc_center.second, cc_center.first, cc_center.second);
    drawNinePointCircle(npc_center.first, npc_center.second, cc_radius / 2);
    cout << "\nInCenter Circle " << ic_center.first << ", " << ic_center.second << " Radius = " << ic_radius;
    cout << "\nCircumcenter Circle " << cc_center.first << " " << cc_center.second << " Radius = " << cc_radius;
    cout << "\nNine Point Circle " << npc_center.first << " " << npc_center.second << " Radius = " << cc_radius / 2 << "\n";
    arrToFile();




  return 0;
}




