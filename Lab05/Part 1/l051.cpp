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
//Arnav Jain
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
        width = w * 3;
        height = h;
        for (int i = 0; i < height; i++)
        {
            vector<int> temp;
            for (int j = 0; j < width; j++)
            {
                temp.push_back(0);
            }
            ppm.push_back(temp);
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
        Image gray = Image(width/3, height, scale);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j+=3)
            {
                int val = (ppm[i][j] + ppm[i][j + 1] + ppm[i][j + 2]) / 3;
                //cout << i << " " << j << " " << val << endl;
                gray.setPixel(i, j, val, val, val);
            }
        }
        return gray;
    }
    Image SobelOperator(bool horizontal)
    {
        Image sobel = Image(width/3, height, scale);
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
        else{
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
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j+=3)
            {
                int val = 0;

                for(int k = -1; k <= 1; k++){
                    for(int l = -1; l <= 1; l++){
                        if(i + k >= 0 && i + k < height && j + l >= 0 && j + l < width){
                            val += ppm[i + k][j + l] * oper[(k + 1) * 3 + l + 1];

                        }
                    }
                }
                val = abs(val);
                sobel.setPixel(i, j, val, val, val);
            }
        }
        return sobel;
    }
    Image Gradient(){
        Image gx = SobelOperator(true);
        Image gy = SobelOperator(false);
        Image gradient = Image(width/3, height, 1);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j+=3)
            {
                int val = pow(gx.ppm[i][j], 2) + pow(gy.ppm[i][j], 2);
                if(val > 40000){
                    gradient.setPixel(i, j, 1, 1, 1);
                }
                else{
                    gradient.setPixel(i, j, 0, 0, 0);
                }
                
            }
        }
        return gradient;
    }
    void writePPM(string filename)
    {
        ofstream outfile;
        outfile.open(filename);
        outfile << "P3" <<" "<< width/3 << " " << height <<" "<< scale << endl;
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
};
Image read_image(string file)
{
    ifstream infile;
    infile.open(file);
    string line;
    getline(infile, line);

    line = line.substr(3);
    int width = stoi(line.substr(0, line.find(" ")));
    line = line.substr(line.find(" ") + 1);
    int height = stoi(line.substr(0, line.find(" ")));
    line = line.substr(line.find(" ") + 1);
    int scale = stoi(line.substr(0, line.find(" ")));
    Image img = Image(width, height, scale);
    //cout << img.ppm.size() << " " << img.ppm[0].size() << endl;
    for (int i = 0; i < height; i++)
    {
        
        for (int j = 0; j < width * 3; j+= 3)
        {
            getline(infile, line, ' ');
            int r = stoi(line);

            getline(infile, line, ' ');
            int g = stoi(line);
            if(j == (width * 3) - 1)
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
    Image test = read_image("image.ppm");
    Image greyscale = test.toGrayScale();
    greyscale.writePPM("imageg.ppm");
    greyscale.Gradient().writePPM("imagem.ppm");

}
int main()
{
    part1();
}
