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

using namespace std;
class Image
{
public:
    int width;
    int height;
    int scale;
    vector<vector<int> > ppm;
    Image(int w, int h, int s)
    {
        scale = s;
        int ppm[w][h];
        width = w * 3;
        height = h;
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                ppm[i][j] = 0;
            }
        }
    }
    void setPixel(int x, int y, int r, int g, int b)
    {
        ppm[x][y] = r;
        ppm[x][y + 1] = g;
        ppm[x][y + 2] = b;
    }
    Image toGrayScale()
    {
        Image gray = Image(width, height, scale);
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                int val = (ppm[i][j] + ppm[i][j + 1] + ppm[i][j + 2]) / 3;
                gray.setPixel(i, j, val, val, val);
            }
        }
        return gray;
    }
    Image SobelOperator()
    {
    }
    void writePPM(string filename)
    {
        ofstream outfile;
        outfile.open(filename);
        outfile << "P3" << endl;
        outfile << width << " " << height << endl;
        outfile << scale << endl;
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                outfile << ppm[i][j] << " ";
            }
            outfile << endl;
        }
        outfile.close();
    }
};
Image read_image(string file)
{
    ifstream infile;
    infile.open(file);
    string line;
    getline(infile, line);
    getline(infile, line);
    int width = stoi(line.substr(0, line.find(" ")));
    int height = stoi(line.substr(line.find(" ") + 1));
    getline(infile, line);
    int scale = stoi(line);
    Image img = Image(width, height, scale);
    for (int i = 0; i < width * 3; i++)
    {
        for (int j = 0; j < height; j++)
        {
            getline(infile, line, ' ');
            int r = stoi(line);
            getline(infile, line, ' ');
            int g = stoi(line);
            if(j == height - 1)
            {
                getline(infile, line);
            }
            else
            {
                getline(infile, line, ' ');
            }
            int b = stoi(line);
            img.setPixel(i, j, r, g, b);
        }
    }
    infile.close();
    return img;
}
void part1()
{
    Image test = read_image("sample.ppm");
}
int main()
{
    part1();
    return 0;
}
