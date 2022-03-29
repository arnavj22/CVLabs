#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace cv;
using namespace std;
void display(int p, int n, int d, int q, int h){
    double money = p * 0.01 + n * 0.05 + d * 0.1 + q * 0.25 + h * 1;
    ofstream outfile;
    outfile.open("results.txt");
    outfile << "Pennies: " << p << "\nNickels: " << n << "\nDimes: " << d << "\nQuarters: " << q << "\nSilver Dollars: " << h << endl;
    outfile << "Total Money: $" << setprecision(2) << fixed << money << endl;
    outfile.close();
    cout << "Pennies: " << p << "\nNickels: " << n << "\nDimes: " << d << "\nQuarters: " << q << "\nSilver Dollars: " << h << endl;
    cout << "Total Money: $" << setprecision(2) << fixed << money << endl;
}
int part1(int argc, char **argv){
    int minsize = 97, upper = 80, maxsize = 130, circlethresh = 50;
    string filename = "image.jpg";
    for (int i = 1; i < argc; i++)
    {
        string a = argv[i];
        if (a == ("-S"))
        {
            minsize = stoi(argv[i + 1]);
        }
        if (a == ("-T"))
        {
            upper = stoi(argv[i + 1]);
        }
        if (a == ("-F"))
        {
            filename = argv[i + 1];
        }
        if (a == ("-M"))
        {
            maxsize = stoi(argv[i + 1]);
        }
        if (a == ("-TC"))
        {
            circlethresh = stoi(argv[i + 1]);
        }
        if (a == ("-Big"))
        {
            circlethresh = stoi(argv[i + 1]);
        }
    }
    // Loads an image
    Mat src = imread(samples::findFile(filename), IMREAD_COLOR);
    // Check if image is loaded properly
    if (src.empty())
    {
        printf(" Error opening image\n");
        return EXIT_FAILURE;
    }
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    imwrite("imageg.jpg", gray);
    medianBlur(gray, gray, 5);
    Mat edges;
    Canny(gray, edges, 50, 80, 3, false);
    imwrite("imagef.jpg", edges);
    vector<Vec3f> circles;
    vector<Vec3f> circlesLarge;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1, 70, upper, circlethresh, 75, maxsize);
    HoughCircles(gray, circlesLarge, HOUGH_GRADIENT, 1, 70, 65, 45, 150, 180);
    // imageeasy HoughCircles(gray, circles, HOUGH_GRADIENT, 1, 85, 100, 50, 50, 130);
    // imageeasy HoughCircles(gray, circles, HOUGH_GRADIENT, 1, 85, 100, 50, 50, 130);
    // imageeasy HoughCircles(gray, circles, HOUGH_GRADIENT, 1, 85, 100, 50, 50, 170);
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
        if (radius < minsize) // 95 for easy, 90 for med, 90 for hard
        {
            if (col > 1.00 || colorAtCenter[2] < 80)
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
        else if (radius < minsize + 9) // 103 for medium, 110 for small
        {
            circle(src, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
            n += 1; // nickel
        }
        else if (radius < minsize + 63)
        {
            q += 1; // quarter
            circle(src, center, radius, Scalar(0, 255, 0), 3, LINE_AA);
        }
    }
    for (size_t i = 0; i < circlesLarge.size(); i++)
    {
        Vec3i c = circlesLarge[i];
        int radius = c[2];
        Point center = Point(c[0], c[1]);
        circle(src, center, radius, Scalar(0, 255, 255), 3, LINE_AA);
        h += 1; // Silver dollar
    }
    display(p, n, d, q, h);

    imwrite("coins.jpg", src);
    return 0;
}
int main(int argc, char **argv)
{
    part1(argc, argv);
}