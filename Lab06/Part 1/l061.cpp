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
    void drawLineAdditive(int x, int y, double angle)
    {
        double slope = tan(angle);
        cout << slope << "\n";
        double intercept = y - slope * x;
        double x1 = 0;
        double x2 = height - 1;
        double y1 = slope * x1 + intercept;
        double y2 = slope * x2 + intercept;
        double dx = x2 - x1;
        double dy = y2 - y1;

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
                    if (i * 3 >= width)
                    {
                        continue;
                    }
                    int v = ppm[i][j*3] + 1;
                    setPixel(i, j*3, v, v, v);
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
                    if (j * 3 >= width)
                    {
                        continue;
                    }
                    int v = ppm[i][j*3] + 1;
                    setPixel(i, j*3, v, v, v);
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
                    if (i * 3 >= width)
                    {
                        continue;
                    }
                    int v = ppm[j][i * 3] + 1;
                    setPixel(j, i * 3, v, v, v);
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
                    if(i * 3 >= width){
                        continue;
                    }
                    int v = ppm[j][i*3] + 1;
                    setPixel(j, i * 3, v, v, v);
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
                val = abs(val);
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
                double val = atan2(gy.ppm[i][j], gx.ppm[i][j]);
                cout << val << "\n";
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
    Image minthreshold(Image magnitude)
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
                if (mag > magnitude.ppm[i + direction[val].first][j + direction[val].second * 3] && mag > magnitude.ppm[i - direction[val].first][j - direction[val].second * 3])
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
    Image combineImage(Image image)
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
    Image andfunction(Image im)
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
    Image generateIntermediaryLines(Image t){
        Image o = Image(width / 3, height, 10);
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width ; j+= 3){
                if(ppm[i][j] == 1){
                    o.drawLineAdditive(i, j, t.ppm[i][j]);
                }
            }
        }
        return o;
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
    int lower = 150, upper = 220;
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
    }
    string outputfile = "imagef.ppm";
    for (int i = 1; i < argc; i++)
    {
        string a = argv[i];
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
    Image mint = t.minthreshold(greyscale.magnitude());
    mint.writePPM("image2.ppm");
    Image canny = comb.andfunction(mint);
    canny.writePPM(outputfile);
    Image th = greyscale.theta();
    th.writePPM("theta.ppm");
    Image lines = canny.generateIntermediaryLines(th);
    lines.writePPM("lines.ppm");
}
int main(int argc, char **argv)
{
    part1(argc, argv);
}