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
    Image Gradient(int threshold){ // true means magnitude, false means direction
        Image gx = SobelOperator(true);
        Image gy = SobelOperator(false);
        Image gradient = Image(width/3, height, 1);
        if(threshold != -1){
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j+=3)
                {
                    int val = pow(gx.ppm[i][j], 2) + pow(gy.ppm[i][j], 2);
                    if(val > threshold){
                        gradient.setPixel(i, j, 1, 1, 1);
                    }
                    else{
                        gradient.setPixel(i, j, 0, 0, 0);
                    }
                    
                }
            }
        }
        else{
            int angles[] = { -180, -135, -90, -45, 0, 45, 90, 135, 180 };
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j += 3)
                {
                    double val = atan2(gy.ppm[i][j], gx.ppm[i][j]);
                    if(val > 1){
                        val = val - 2 * M_PI;
                    }
                    val *= 180 / M_PI;
                    double mindist = 180;
                    double index = 0;

                    for(int k = 0; k < 9; k++){
                        double dist = abs(val - angles[k]);
                        if(dist < mindist){
                            mindist = dist;
                            index = k;
                        }
                    }
                    //cout << index;
                    gradient.setPixel(i, j, index, index, index);
                }
            }
        }
        return gradient;
    }
    Image minthreshold(Image magnitude){
        pair<int, int> direction[] = {make_pair(0, 1), make_pair(1, 1), make_pair(1, 0), make_pair(-1, 1), make_pair(-1, 0), make_pair(-1, -1), make_pair(0, -1), make_pair(1, -1), make_pair(0, 1) };
        Image min = Image(width/3, height, 1);
        for (int i = 1; i < height - 1; i++)
        {
            for (int j = 3; j < width - 3; j+=3)
            {

                int val = ppm[i][j];
                int mag = magnitude.ppm[i][j];
                //cout << i << " " << j << " " << val << endl;
                if(mag > magnitude.ppm[i + direction[val].first][j + direction[val].second * 3] && mag > magnitude.ppm[i - direction[val].first][j - direction[val].second * 3]){
                    min.setPixel(i, j, 1, 1, 1);
                }
                else{
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
        outfile << "P3"
                << " " << width / 3 << " " << height << " " << scale << endl;
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
    Image combineImage(Image image){
        Image newIm = Image(width/3, height, scale + image.scale);
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
    void floodfill(){
        vector<pair<int, int> > hardEdges;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j += 3)
            {
                if(ppm[i][j] == 2){
                    hardEdges.push_back(make_pair(i, j));
                }
            }
        }
        for(int i = 0; i < hardEdges.size(); i++){
            floodFillRecur(hardEdges[i].first, hardEdges[i].second, 1, 2);
        }
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j += 3)
            {
                if (ppm[i][j] == 2)
                {
                    setPixel(i,j, 1, 1, 1);
                }
                else{
                    setPixel(i, j, 0, 0, 0);
                }
            }
        }
        scale = 1;
    }
    void floodFillRecur(int x, int y, int prevC, int newC, int t = 0){
        if (x < 0 || x >=  height || y < 0 || y >= width)
            return;
        if (t!=0 && ppm[x][y] != prevC)
            return;
        if (t !=0 && ppm[x][y] == newC){
            return;
        }
        //cout << "replacing " << prevC << " with " << newC << "\n";
        // Replace the color at (x, y)
        setPixel(x, y, newC, newC, newC);

        // Recur for north, east, south and west
        floodFillRecur(x + 1, y, prevC, newC, t+1);
        floodFillRecur(x - 1, y, prevC, newC, t+1);
        floodFillRecur(x, y + 3, prevC, newC, t+1);
        floodFillRecur(x, y - 3, prevC, newC, t+1);
        floodFillRecur(x + 1, y + 3, prevC, newC, t+1);
        floodFillRecur(x - 1, y - 3, prevC, newC, t+1);
        floodFillRecur(x - 1, y + 3, prevC, newC, t+1);
        floodFillRecur(x + 1, y - 3, prevC, newC, t+1);
    }
};
Image read_image(string file)
{
    
    ifstream infile;
    infile.open(file);
    string line;
    infile >> line;
    infile >> line;
    int width = stoi(line);
    infile >> line;
    int height = stoi(line);
    infile >> line;
    int scale = stoi(line);
    Image img = Image(width, height, scale);
    //cout << img.ppm.size() << " " << img.ppm[0].size() << endl;
    for (int i = 0; i < height; i++)
    {
        
        for (int j = 0; j < width * 3; j+= 3)
        {
            infile >> line;
            int r = stoi(line);

            infile >> line;
            int g = stoi(line);
            infile >> line;
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
    greyscale.Gradient(20000).writePPM("imagem.ppm");

}
void part2(char *argv[]){
    int lower, upper;
    lower = 150;
    upper = 220;
    Image im = read_image("image.ppm");
    Image greyscale = im.toGrayScale();
    greyscale.writePPM("imageg.ppm");
    Image lowerthreshold = greyscale.Gradient(lower * lower);
    Image upperthreshold = greyscale.Gradient(upper * upper);
    //cout << "finished thresholding" << "\n";
    Image comb = lowerthreshold.combineImage(upperthreshold);
    //cout << "finished combining" << "\n";
         
    comb.floodfill();
    comb.writePPM("image1.ppm");
}
void part3(char *argv[])
{
    part2(argv);
    Image im = read_image("image.ppm");
    Image greyscale = im.toGrayScale();
    Image comb = read_image("image1.ppm");
    Image theta = greyscale.Gradient(-1);
    //theta.writePPM("imagetheta.ppm");
    Image mint = theta.minthreshold(greyscale);
    mint.writePPM("image2.ppm");
    comb.combineImage(mint).writePPM("image3.ppm");
}
int main(int argc, char *argv[])
{
    //part1();
    //part2(argv);
    part3(argv);
}
