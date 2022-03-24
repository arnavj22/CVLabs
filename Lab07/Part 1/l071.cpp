#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace cv;
using namespace std;
void display(int p, int n, int d, int q, int h){
    double money = p * 0.01 + n * 0.05 + d * 0.1 + q * 0.25 + h * 0.5;
    ofstream outfile;
    outfile.open("results.txt");
    outfile << "Pennies: " << p << "\nNickels: " << n << "\nDimes: " << d << "\nQuarters: " << q << "\nHalf Dollars: " << h << endl;
    outfile << "Total Money: $" << setprecision(2) << fixed << money << endl;
    outfile.close();
    cout << "Pennies: " << p << "\nNickels: " << n << "\nDimes: " << d << "\nQuarters: " << q << "\nHalf Dollars: " << h << endl;
    cout << "Total Money: $" << setprecision(2) << fixed << money << endl;
}
int part1(int argc, char **argv){
    const char *filename = argc >= 2 ? argv[1] : "image.jpg";
    // Loads an image
    Mat src = imread(samples::findFile(filename), IMREAD_COLOR);
    // Check if image is loaded properly
    if (src.empty())
    {
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default %s] \n", filename);
        return EXIT_FAILURE;
    }
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    imwrite("gray.jpg", gray);
    medianBlur(gray, gray, 5);
    Mat edges;
    Canny(gray, edges, 100, 200, 3, false);
    imwrite("imagef.jpg", edges);
    vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1, 85, 100, 50, 50, 130);
    int p = 0, n = 0, d = 0, q = 0, h = 0;
    double price = 0;
    for (size_t i = 0; i < circles.size(); i++)
    {
        Vec3i c = circles[i];
        int radius = c[2];
        Point center = Point(c[0], c[1]);
        Vec3b colorAtCenter = src.at<Vec3b>(center);
        if(colorAtCenter[1] == 0)
            colorAtCenter[1] = 1;
        double col = (double)colorAtCenter[2] / (double)colorAtCenter[1];
        if (radius < 93)
        {
            if (col > 1.00 || colorAtCenter[2] > 60)
            {
                circle(src, center, radius, Scalar(0, 0, 255), 3, LINE_AA);
                p += 1;
            }
            else
            {
                circle(src, center, radius, Scalar(255, 0, 0), 3, LINE_AA);
                d += 1; // penny or dime
            }
        }
        else if (radius < 103)
        {
            circle(src, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
            n += 1; // nickel
        }
        else if (radius < 140)
        {
            circle(src, center, radius, Scalar(0, 255, 0), 3, LINE_AA);
            q += 1; // quarter
        }
        else
        {
            circle(src, center, radius, Scalar(255, 255, 0), 3, LINE_AA);
            h += 1; // half dollar
        }
    }
    display(p, n, d, q, h);

    imwrite("coins.jpg", src);
    return 0;
}
int main(int argc, char **argv)
{
    part1(argc, argv);
}