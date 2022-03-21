#include <stdio.h>
#include <algorithm>
#include <list>
#include <iterator>
#include <chrono>
#include <climits>
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <math.h>
// Arnav Jain
using namespace std;
class Coin
{
public:
    int radius;
    int xc;
    int yc;
    double percentage;
    double color;
    string type;
    int r, g, b;
    Coin(int x, int y, int rad, double perc, double col)
    {
        r = 0;
        g = 0;
        b = 0;
        radius = rad;
        xc = x;
        yc = y;
        percentage = perc;
        color = col;
        if (rad < 93)
        {
            if(col > 1.1){
                type = "p";
                r = 255;
            }
            else{
                b = 255;
                type = "d"; // penny or dime
            }

        }
        else if (rad < 108)
        {
            r = 255;
            b = 255;
            type = "n"; // nickel
        }
        else if (rad < 140)
        {
            g = 255;
            type = "q"; // quarter
        }
        else
        {
            r = 255;
            g = 255;
            type = "h"; // half dollar
        }
    }
    double distance(Coin c){
        return (c.xc - xc) * (c.xc - xc) + (c.yc - yc) * (c.yc - yc);
    }
};
class Image
{
public:
    int width;
    int height;
    int scale;
    vector<vector<double> > ppm;
    Image(int w, int h, int s)
    {
        scale = s;
        width = w * 3;
        height = h;
        for (int i = 0; i < height; i++)
        {
            vector<double> temp;
            for (int j = 0; j < width; j++)
            {
                temp.push_back(0);
            }
            ppm.push_back(temp);
        }
    }
    vector<int> points(int x, int y, double slope){
        vector<int> temp;
        double m = pow((1 + slope * slope), 0.5);
        double dx = 1 / m;
        double dy = slope / m;
        temp.push_back(x + dx * 200);
        temp.push_back(y + dy * 200);
        temp.push_back(x - dx * 200);
        temp.push_back(y - dy * 200);
        // // cout << x << " " << y << " " << slope << endl;
        // double intercept = y - slope * x;
        // int x1 = 0;
        // int y1 = x1 * slope + intercept;
        // //cout << x1 << " " << y1 << endl;
        // if(y1 >= 0 && y1 < height){
        //     temp.push_back(x1);
        //     temp.push_back(y1);
        // }
        // x1 = (width / 3) - 1;
        // y1 = x1 * slope + intercept;
        // //cout << x1 << " " << y1 << endl;
        // if (y1 >= 0 && y1 < height)
        // {
        //     temp.push_back(x1);
        //     temp.push_back(y1);
        // }
        // y1 = 0;
        // x1 = (y1 - intercept) / slope;
        // //cout << x1 << " " << y1 << endl;
        // if (x1 >= 0 && x1 < width / 3)
        // {
        //     temp.push_back(x1);
        //     temp.push_back(y1);
        // }
        // y1 = height - 1;
        // x1 = (y1 - intercept) / slope;
        // //cout << x1 << " " << y1 << endl;
        // if (x1 >= 0 && x1 < width / 3)
        // {
        //     temp.push_back(x1);
        //     temp.push_back(y1);
        // }
        return temp;
    }
    void setPixel(int x, int y, double r, double g, double b)
    {
        if(x < height && x >= 0 && y + 2 < width && y >= 0){
            ppm[x][y] = r;
            ppm[x][y + 1] = g;
            ppm[x][y + 2] = b;
        }
    }
    void drawLine(int x, int y, double angle)
    {
        double slope = tan(angle);
        double intercept = y - slope * x;
        double x1 = 0;
        double x2 = width / 3;
        double y1 = slope * x1 + intercept;
        double y2 = slope * x2 + intercept;
        int dx = x2 - x1;
        int dy = y2 - y1;

        if ((dx <= 0 and dy <= 0) or (dx <= 0 and dy > 0))
        {
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
        if (abs(dx) > abs(dy))
        {
            if (dy < 0)
            {
                for (int i = x1; i < x2; i++)
                {
                    setPixel(i, j, 1, 1, 1);
                    if (c > 0)
                    {
                        j -= 1;
                        c -= dx;
                    }
                    c -= dy;
                }
            }
            else
            {
                for (int i = x1; i < x2; i++)
                {
                    setPixel(i, j, 1, 1, 1);
                    if (c > 0)
                    {
                        j += 1;
                        c -= dx;
                    }
                    c += dy;
                }
            }
        }
        else
        {
            j = x1;
            int c = dx - dy;
            if (dy < 0)
            {
                for (int i = y1; i > y2; i--)
                {
                    setPixel(j, i, 1, 1, 1);
                    if (c > 0)
                    {
                        j += 1;
                        c += dy;
                    }
                    c += dx;
                }
            }
            else
            {
                for (int i = y1; i < y2; i++)
                {
                    setPixel(j, i, 1, 1, 1);
                    if (c > 0)
                    {
                        j += 1;
                        c -= dy;
                    }
                    c += dx;
                }
            }
        }
    }
    void drawLineAdditive(int y, int x, double angle, int increment)
    {
        double slope = tan(angle);
        vector<int> p1 = points(x, y, slope);
        //cout << p1.size() << endl;
        int x1 = p1[0];
        int y1 = p1[1];
        int x2 = p1[2];
        int y2 = p1[3];

        
        //cout << x1 << ", " << y1 << " : " << x2 << ", " << y2 << "\n";
        double dx = x2 - x1;
        double dy = y2 - y1;

        if ((dx <= 0 and dy <= 0) or (dx <= 0 and dy > 0)) // flip point 1 and point 2
        {
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
        if (abs(dx) > abs(dy))
        {
            if (dy < 0)
            {
                for (int i = x1; i < x2; i++)
                {
                    if (j < height && j  >= 0 && i * 3 < width && i * 3 >= 0)
                    {
                        int v = ppm[j][i * 3] + increment;
                        setPixel(j, i * 3, v, v, v);
                    }
                    
                    if (c > 0)
                    {
                        j -= 1;
                        c -= dx;
                    }
                    c -= dy;
                }
            }
            else
            {
                for (int i = x1; i < x2; i++)
                {
                    if (j < height && j  >= 0 && i * 3 < width && i * 3 >= 0)
                    {
                        int v = ppm[j][i * 3] + increment;
                        setPixel(j, i * 3, v, v, v);
                    }
                    if (c > 0)
                    {
                        j += 1;
                        c -= dx;
                    }
                    c += dy;
                }
            }
        }
        else // change in y > change in x aka slope > 1
        {
            j = x1;
            int c = dx - dy;
            if (dx < 0) // downwards
            {
                for (int i = y1; i > y2; i--)
                {
                    if (i < height && i  >= 0 && j * 3 < width && j * 3 >= 0)
                    {
                        int v = ppm[i][j * 3] + increment;
                        setPixel(i, j * 3, v, v, v);
                    }
                    if (c > 0)
                    {
                        j -= 1;
                        c -= dy;
                    }
                    c -= dx;
                }
            }
            else // upwards
            {
                for (int i = y1; i < y2; i++)
                {
                    if (i < height && i >= 0 && j * 3 < width && j * 3 >= 0)
                    {
                        int v = ppm[i][j * 3] + increment;
                        setPixel(i, j * 3, v, v, v);
                    }
                    if (c > 0)
                    {
                        j += 1;
                        c -= dy;
                    }
                    c += dx;
                }
            }
        }
    }
    void drawCircle(int x, int y, double radius1)
    {
        int a = x;
        int b = y;
        int radius = radius1;
        int xmax = (int)(radius * 0.70710678);
        y = radius;
        int y2 = y * y;
        int ty = (2 * y) - 1;
        int y2_new = y2;
        for (int x = 0; x <= xmax + 2; x++)
        {
            if ((y2 - y2_new) >= ty)
            {
                y2 -= ty;
                y -= 1;
                ty -= 2;
            }
            setPixel(x + a, (-y + b) * 3, 1, 1, 1);
            setPixel(-x + a, (y + b) * 3, 1, 1, 1);
            setPixel(x + a, (y + b) * 3, 1, 1, 1);
            setPixel(-x + a, (-y + b) * 3, 1, 1, 1);
            setPixel(y + a, (x + b) * 3, 1, 1,  1);
            setPixel(y + a, (-x + b) * 3, 1, 1, 1);
            setPixel(-y + a, (x + b) * 3, 1, 1, 1);
            setPixel(-y + a, (-x + b) * 3, 1, 1, 1);
            y2_new -= (2 * x) - 3;
        }
    }
    void drawCirclergb(int x, int y, double radius1, int r, int g, int blue)
    {
        int a = x;
        int b = y;
        int radius = radius1;
        int xmax = (int)(radius * 0.70710678);
        y = radius;
        int y2 = y * y;
        int ty = (2 * y) - 1;
        int y2_new = y2;
        for (int x = 0; x <= xmax + 2; x++)
        {
            if ((y2 - y2_new) >= ty)
            {
                y2 -= ty;
                y -= 1;
                ty -= 2;
            }
            setPixel(x + a, (-y + b) * 3, r, g, blue);
            setPixel(-x + a, (y + b) * 3, r, g, blue);
            setPixel(x + a, (y + b) * 3, r, g, blue);
            setPixel(-x + a, (-y + b) * 3, r, g, blue);
            setPixel(y + a, (x + b) * 3, r, g, blue);
            setPixel(y + a, (-x + b) * 3, r, g, blue);
            setPixel(-y + a, (x + b) * 3, r, g, blue);
            setPixel(-y + a, (-x + b) * 3, r, g, blue);
            y2_new -= (2 * x) - 3;
        }
    }
    
    Image toGrayScale()
    {
        Image gray = Image(width / 3, height, scale);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j += 3)
            {
                int val = (ppm[i][j] + ppm[i][j + 1] + ppm[i][j + 2]) / 3;
                // cout << i << " " << j << " " << val << endl;
                gray.setPixel(i, j, val, val, val);
            }
        }
        return gray;
    }
    Image SobelOperator(bool horizontal)
    {
        Image sobel = Image(width / 3, height, scale);
        vector<int> oper;
        if (horizontal)
        {
            oper.push_back(-1);
            oper.push_back(0);
            oper.push_back(1);
            oper.push_back(-2);
            oper.push_back(0);
            oper.push_back(2);
            oper.push_back(-1);
            oper.push_back(0);
            oper.push_back(1);
        }
        else
        {
            oper.push_back(-1);
            oper.push_back(-2);
            oper.push_back(-1);
            oper.push_back(0);
            oper.push_back(0);
            oper.push_back(0);
            oper.push_back(1);
            oper.push_back(2);
            oper.push_back(1);
        }
        for (int i = 1; i < height - 1; i++)
        {
            for (int j = 3; j < width - 3; j += 3)
            {
                int val = 0;

                for (int k = -1; k <= 1; k++)
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        if (i + k >= 0 && i + k < height && j + l >= 0 && j + l < width)
                        {
                            val += ppm[i + k][j + (l * 3)] * oper[(k + 1) * 3 + l + 1];
                        }
                    }
                }
                sobel.setPixel(i, j, val, val, val);
            }
        }
        return sobel;
    }
    Image magnitude()
    {
        Image gx = SobelOperator(true);
        Image gy = SobelOperator(false);
        Image gradient = Image(width / 3, height, 1);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j += 3)
            {
                int val = pow(gx.ppm[i][j], 2) + pow(gy.ppm[i][j], 2);
                gradient.setPixel(i, j, val, val, val);
            }
        }
        return gradient;
    }
    Image theta(){
        Image gx = SobelOperator(true);
        Image gy = SobelOperator(false);
        Image gradient = Image(width / 3, height, 1);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j += 3)
            {
                double val = 1/atan2(gy.ppm[i][j], gx.ppm[i][j]);
                gradient.setPixel(i, j, val, val, val);
            }
        }
        return gradient;
    }
    Image Gradient(int threshold)
    { // true means magnitude, false means direction
        Image gradient = Image(width / 3, height, 1);
        if (threshold != -1)
        {
            Image mag = magnitude();
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j += 3)
                {
                    if (mag.ppm[i][j] > threshold)
                    {
                        gradient.setPixel(i, j, 1, 1, 1);
                    }
                    else
                    {
                        gradient.setPixel(i, j, 0, 0, 0);
                    }
                }
            }
        }
        else
        {
            int angles[] = {-180, -135, -90, -45, 0, 45, 90, 135, 180};
            Image t = theta();
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j += 3)
                {
                    double val = t.ppm[i][j];
                    double mindist = 180;
                    double index = 0;

                    for (int k = 0; k < 9; k++)
                    {
                        double dist = abs(val - angles[k]);
                        if (dist < mindist)
                        {
                            mindist = dist;
                            index = k;
                        }
                    }
                    // cout << index;
                    gradient.setPixel(i, j, index, index, index);
                }
            }
        }
        return gradient;
    }
    Image minthreshold(Image &magnitude)
    {
        pair<int, int> direction[] = {make_pair(0, 1), make_pair(1, 1), make_pair(1, 0), make_pair(-1, 1), make_pair(-1, 0), make_pair(-1, -1), make_pair(0, -1), make_pair(1, -1), make_pair(0, 1)};
        Image min = Image(width / 3, height, 1);
        for (int i = 1; i < height - 1; i++)
        {
            for (int j = 3; j < width - 3; j += 3)
            {

                int val = ppm[i][j];
                int mag = magnitude.ppm[i][j];
                // cout << i << " " << j << " " << val << endl;
                if (mag >= magnitude.ppm[i + direction[val].first][j + direction[val].second * 3] && mag >= magnitude.ppm[i - direction[val].first][j - direction[val].second * 3])
                {
                    min.setPixel(i, j, 1, 1, 1);
                }
                else
                {
                    min.setPixel(i, j, 0, 0, 0);
                }
            }
        }
        return min;
    }
    void writePPM(string filename)
    {
        ofstream outfile;
        outfile.open(filename);
        outfile << "P3" << " " << width / 3 << " " << height << " " << scale << endl;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                outfile << ppm[i][j] << " ";
            }
            outfile << endl;
        }
        outfile.close();
    }
    Image combineImage(Image &image)
    {
        Image newIm = Image(width / 3, height, scale + image.scale);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j += 3)
            {
                int k = ppm[i][j] + image.ppm[i][j];
                newIm.setPixel(i, j, k, k, k);
            }
        }
        return newIm;
    }
    void floodfill()
    {
        vector<pair<int, int> > hardEdges;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j += 3)
            {
                if (ppm[i][j] == 2)
                {
                    hardEdges.push_back(make_pair(i, j));
                }
            }
        }
        for (int i = 0; i < hardEdges.size(); i++)
        {
            floodFillRecur(hardEdges[i].first, hardEdges[i].second, 1, 2);
        }
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j += 3)
            {
                if (ppm[i][j] == 2)
                {
                    setPixel(i, j, 1, 1, 1);
                }
                else
                {
                    setPixel(i, j, 0, 0, 0);
                }
            }
        }
        scale = 1;
    }
    void floodFillRecur(int x, int y, int prevC, int newC, int t = 0)
    {
        if (x < 0 || x >= height || y < 0 || y >= width)
            return;
        if (t != 0 && ppm[x][y] != prevC)
            return;
        if (t != 0 && ppm[x][y] == newC)
        {
            return;
        }
        // cout << "replacing " << prevC << " with " << newC << "\n";
        //  Replace the color at (x, y)
        setPixel(x, y, newC, newC, newC);

        // Recur for north, east, south and west
        floodFillRecur(x + 1, y, prevC, newC, t + 1);
        floodFillRecur(x - 1, y, prevC, newC, t + 1);
        floodFillRecur(x, y + 3, prevC, newC, t + 1);
        floodFillRecur(x, y - 3, prevC, newC, t + 1);
        floodFillRecur(x + 1, y + 3, prevC, newC, t + 1);
        floodFillRecur(x - 1, y - 3, prevC, newC, t + 1);
        floodFillRecur(x - 1, y + 3, prevC, newC, t + 1);
        floodFillRecur(x + 1, y - 3, prevC, newC, t + 1);
    }
    Image andfunction(Image &im)
    {
        Image newIm = Image(width / 3, height, 1);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (ppm[i][j] == 0 || im.ppm[i][j] == 0)
                {
                    newIm.setPixel(i, j, 0, 0, 0);
                }
                else
                {
                    newIm.setPixel(i, j, 1, 1, 1);
                }
            }
        }
        return newIm;
    }
    Image generateIntermediaryLines(Image &t){
        Image o = Image(width / 3, height, 10);
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width ; j+= 3){
                if(ppm[i][j] == 1){
                    o.drawLineAdditive(i + 1, j / 3, t.ppm[i][j], 1);
                    o.drawLineAdditive(i, j/3, t.ppm[i][j], 2);
                    o.drawLineAdditive(i - 1, j / 3, t.ppm[i][j], 1);
                }
            }
        }
        int max = 0;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j += 3)
            {
                if (o.ppm[i][j] > max)
                {
                    max = o.ppm[i][j];
                }
            }
        }
        o.scale = max;
        return o;
    }
    Image threshold(int t){
        Image o = Image(width / 3, height, 1);
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width ; j+= 3){
                if(ppm[i][j] > t){
                    o.drawCircle(i, j/3, 1);
                    o.drawCircle(i, j / 3, 2);
                    o.drawCircle(i, j / 3, 3);
                    o.drawCircle(i, j / 3, 4);
                }
            }
        }
        return o;
    }
    vector<pair<int, int> > centerPoints(int t)
    {
        vector<pair<int, int> > centers;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j += 3)
            {
                if (ppm[i][j] > t)
                {
                    centers.push_back(make_pair(i, j/3));
                }
            }
        }
        return centers;
    }
    vector<pair<int, int> > centerPointsSquare(int t) //splits the image into 10x10 regions, computes the average value of each region
    {
        vector<pair<int, int> > centers;
        int xsec = height / 20;
        int ysec = width / 60;
        for (int i = 0; i < height - xsec + 1; i += xsec)
        {
            for (int j = 0; j < width - ysec + 1; j += ysec)
            {
                int sum = 0;
                for (int k = 0; k < xsec; k++)
                {
                    for (int l = 0; l < ysec; l+= 3)
                    {
                        sum += ppm[i + k][j + l];
                    }
                }
                int average = sum / (xsec * ysec / 3);
                for (int k = 0; k < xsec; k++)
                {
                    for (int l = 0; l < ysec; l+= 3)
                    {
                        if(ppm[i + k][j + l] > 1.3 * average + 20 || ppm[i + k][j + l] > t){
                            centers.push_back(make_pair(i + k, (j + l)/3));
                        }
                    }
                }
            }
        }
        return centers;

    }
    Image thresholdSquare(int t)
    {
        Image o = Image(width / 3, height, 1);
        for (int i = 0; i < height; i += 20)
        {
            for (int j = 0; j < width; j += 60)
            {
                int min = -1, x1 = -1, y1 = -1;
                for (int k = 0; k < 20; k++)
                {
                    for (int l = 0; l < 60; l += 3)
                    {
                        if (ppm[i + k][j + l] > min)
                        {
                            min = ppm[i + k][j + l];
                            x1 = i + k;
                            y1 = j + l;
                        }
                    }
                }
                if (min > t)
                {
                    o.drawCircle(i, j / 3, 1);
                    o.drawCircle(i, j / 3, 2);
                    o.drawCircle(i, j / 3, 3);
                    o.drawCircle(i, j / 3, 4);
                }
            }
        }
        return o;
    }
    void overlayMask(Image &im){
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j+= 3){
                if(im.ppm[i][j] == 1){
                    setPixel(i, j, im.ppm[i][j] * scale, im.ppm[i][j + 1] * scale, im.ppm[i][j + 2] * scale);
                }
            }
        }
    }
    double circlePercentage(int x, int y, int r){
        int a = x;
        int b = y;
        int xmax = (int)(r * 0.70710678);
        y = r;
        int y2 = y * y;
        int ty = (2 * y) - 1;
        int y2_new = y2;
        int count = 0;
        int total = 0;
        for (int x = 0; x <= xmax + 2; x++)
        {
            if ((y2 - y2_new) >= ty)
            {
                y2 -= ty;
                y -= 1;
                ty -= 2;
            }
            if(inRange(x + a, (-y + b) * 3) && ppm[x + a][(-y + b) * 3] == 1){
                count++;
            }
            if (inRange(-x + a, (y + b) * 3) && ppm[-x + a][(y + b) * 3] == 1)
            {
                count++;
            }
            if (inRange(x + a, (y + b) * 3) && ppm[x + a][(y + b) * 3] == 1)
            {
                count++;
            }
            if (inRange(-x + a, (-y + b) * 3) && ppm[-x + a][(-y + b) * 3] == 1)
            {
                count++;
            }
            if (inRange(y + a, (x + b) * 3) && ppm[y + a][(x + b) * 3] == 1)
            {
                count++;
            }
            if (inRange(y + a, (-x + b) * 3) && ppm[y + a][(-x + b) * 3] == 1)
            {
                count++;
            }
            if (inRange(-y + a, (x + b) * 3) && ppm[-y + a][(x + b) * 3] == 1)
            {
                count++;
            }
            if (inRange(-y + a, (-x + b) * 3) && ppm[-y + a][(-x + b) * 3] == 1)
            {
                count++;
            }
            total += 8;
            y2_new -= (2 * x) - 3;
        }
        return (double)count / (double)total;
    }
    bool inRange(int x1, int y1){
        if(x1 >= 0 && x1 < height && y1 >= 0 && y1 < width){
            return true;
        }
        return false;
    }
    vector<Coin> coinDetection(vector<pair<int, int> > &centers, Image &mask, double threshold = 0.2)
    {
        vector<Coin> coins;
        for(int i = 0; i < centers.size(); i++){
            double maxval = 0;
            int maxrad = -1;
            int x = centers[i].first;
            int y = centers[i].second;
            for(int j = 80; j < 180; j++){
                double percentage = mask.circlePercentage(x, y, j);
                //cout << percentage << endl;
                if(maxval < percentage && percentage > threshold){
                    maxval = percentage;
                    maxrad = j;
                }
            }
            if(maxrad != -1 && maxval > threshold / 2){
                Coin c = Coin(x, y, maxrad, maxval, (ppm[x][y * 3] / (double)ppm[x][y * 3 + 1]));
                coins.push_back(c);
                drawCirclergb(x, y, maxrad, c.r, c.g, c.b);
            }
        }
        return coins;
    } 
    Image drawCenters(vector<pair<int, int> > &centers, Image &im)
    {
        Image o = im;
        for(int i = 0; i < centers.size(); i++){
            o.drawCirclergb(centers[i].first, centers[i].second, 1, 255, 0, 0);
            o.drawCirclergb(centers[i].first, centers[i].second, 2, 255, 0, 0);
            o.drawCirclergb(centers[i].first, centers[i].second, 3, 255, 0, 0);
            o.drawCirclergb(centers[i].first, centers[i].second, 4, 255, 0, 0);
        }
        return o;
    }
    void drawCoins(vector<Coin> &coins){
        for(int i = 0; i < coins.size(); i++){
            Coin c = coins[i];
            drawCirclergb(c.xc, c.yc, c.radius, c.r, c.g, c.b);
        }
    }
};

Image read_image(string file)
{

    ifstream infile;
    infile.open(file);
    int width, height, scale;
    string line;
    infile >> line;
    infile >> width;
    infile >> height;
    infile >> scale;
    int val;
    Image img = Image(width, height, scale);
    vector<int> values;
    while(infile >> val){
        values.push_back(val);
    }
    int counter = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j += 3)
        {
            img.setPixel(i, j, values[counter], values[counter + 1], values[counter + 2]);
            counter += 3;
        }
    }
    infile.close();
    return img;
}
void part1(int argc, char **argv)
{
    int lower = 100, upper = 200, thresh = 100;
    string outputfile = "imagef.ppm";
    for (int i = 1; i < argc; i++)
    {
        string a = argv[i];
        if (a == ("-L"))
        {
            lower = stoi(argv[i + 1]);
        }
        if (a == ("-H"))
        {
            upper = stoi(argv[i + 1]);
        }
        if (a == ("-TC"))
        {
            thresh = stoi(argv[i + 1]);
        }
        if (a == ("-F"))
        {
            outputfile = argv[i + 1];
        }
    }
    Image im = read_image("image.ppm");
    Image greyscale = im.toGrayScale();
    greyscale.writePPM("imageg.ppm");
    Image lowerthreshold = greyscale.Gradient(lower * lower);
    Image upperthreshold = greyscale.Gradient(upper * upper);
    Image comb = lowerthreshold.combineImage(upperthreshold);
    comb.floodfill();
    comb.writePPM("image1.ppm");
    Image t = greyscale.Gradient(-1);
    Image mag = greyscale.magnitude();
    Image mint = t.minthreshold(mag);
    mint.writePPM("image2.ppm");
    Image canny = comb.andfunction(mint);
    canny.writePPM(outputfile);
    Image th = greyscale.theta();
    //th.writePPM("theta.ppm");
    Image lines = canny.generateIntermediaryLines(th);
    lines.writePPM("imagev.ppm");
    Image possibleCenters = lines.threshold(thresh);
    //possibleCenters.writePPM("possibleCenters.ppm");
    im.overlayMask(possibleCenters);
    im.writePPM("imageCC.ppm");
}
void part2(int argc, char **argv)
{
    int lower = 70, upper = 140, thresh = 170;
    double circleThresh = 0.18;
    string inputfile = "image.ppm";
    for (int i = 1; i < argc; i++)
    {
        string a = argv[i];
        if (a == ("-L"))
        {
            lower = stoi(argv[i + 1]);
        }
        if (a == ("-H"))
        {
            upper = stoi(argv[i + 1]);
        }
        if (a == ("-TC"))
        {
            thresh = stoi(argv[i + 1]);
        }
        if (a == ("-F"))
        {
            inputfile = argv[i + 1];
        }
        if(a == ("-TCircle")){
            circleThresh = stod(argv[i + 1]);
        }
    }
    Image im = read_image(inputfile);
    //cout << "image is read" << endl;
    Image greyscale = im.toGrayScale();
    //cout << "image is greyscaled" << endl;
    greyscale.writePPM("imageg.ppm");
    Image lowerthreshold = greyscale.Gradient(lower * lower);
    Image upperthreshold = greyscale.Gradient(upper * upper);
    Image comb = lowerthreshold.combineImage(upperthreshold);
    comb.floodfill();
    //comb.writePPM("image1.ppm");
    Image t = greyscale.Gradient(-1);
    Image mag = greyscale.magnitude();
    Image mint = t.minthreshold(mag);
    //mint.writePPM("image2.ppm");
    Image canny = comb.andfunction(mint);
   // cout << "image is cannied" << endl;
    canny.writePPM("imagef.ppm");
    Image th = greyscale.theta();
    Image lines = canny.generateIntermediaryLines(th);
    lines.writePPM("imagev.ppm");
    // Image possibleCenters = lines.threshold(thresh);
    // im.overlayMask(possibleCenters);
    vector<pair<int, int> > centerPoints = lines.centerPointsSquare(thresh);
    Image cc = im.drawCenters(centerPoints, im);
    //cout << "centers are generated" << endl;
    cc.writePPM("imageCC.ppm");;
    //cout << centerPoints.size() << endl;
    vector<Coin> coins = im.coinDetection(centerPoints, canny, circleThresh);

    im.writePPM("imageCD.ppm");
    vector<Coin> goodCoins;
    for(int i = 0; i < coins.size(); i++){
        Coin coin1 = coins[i];
        bool added = false;
        for(int j = 0; j < goodCoins.size(); j++){
            Coin coin2 = goodCoins[j];
            if (coin1.distance(coin2) < max(coin1.radius, coin2.radius) * max(coin1.radius, coin2.radius))
            {
                added = true;
                if (coin2.percentage > coin1.percentage)
                {
                    goodCoins[j] = coin1;
                }   // cout << i << " " << j << " " << coins.size() << endl;
            }
        }
        if(!added){
            goodCoins.push_back(coin1);
        }
    }
    coins = goodCoins;
    int p = 0, n = 0, d = 0, q = 0, h = 0;
    double money = 0;
    for(int i = 0; i < coins.size(); i++){
        Coin coin = coins[i];
        if(coin.type == "p"){
            p++;
            money += .01;
        }
        else if(coin.type == "n"){
            n++;
            money += .05;
        }
        else if(coin.type == "d"){
            d++;
            money += .1;
        }
        else if(coin.type == "q"){
            q++;
            money += .25;
        }
        else if(coin.type == "h"){
            h++;
            money += 1;
        }

    }
    ofstream outfile;
    outfile.open("results.txt");
    outfile << "Pennies: " << p << "\nNickels: " << n << "\nDimes: " << d << "\nQuarters: " << q << "\nHalf Dollars: " << h << endl;
    outfile << "Total Money: $" << setprecision(2) << fixed <<  money << endl;
    outfile.close();
    cout << "Pennies: " << p << "\nNickels: " << n << "\nDimes: " << d << "\nQuarters: " << q << "\nHalf Dollars: " << h << endl;
    cout << "Total Money: $" << setprecision(2) << fixed << money << endl;
    Image im = read_image(inputfile);
    im.drawCoins(coins);
    im.writePPM("coins.ppm");


}
int main(int argc, char **argv)
{
    //part1(argc, argv);
    part2(argc, argv);
}