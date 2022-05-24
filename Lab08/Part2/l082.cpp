#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/videoio.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
using namespace cv;
using namespace std;
Mat matrixMultiply(Mat a, Mat b){
    Mat c = Mat_<double>(a.rows, b.cols);
    for(int i = 0; i < a.rows; i++){
        for(int j = 0; j < b.cols; j++){
            int sum = 0;
            for(int k = 0; k < a.cols; k++){
                sum += a.at<double>(i, k) * b.at<double>(k, j);
            }
            c.at<double>(i, j) = sum;
        }
    }
    return c;
}
vector<Mat> rotate(vector<Mat> a, int deg){
    //rotate each poin in degree along the z axis
    vector<Mat> output;
    double radian = deg * M_PI/180;
    Mat rotate1 = (Mat_<double>(4, 4) << 1, 0, 0, 0, 0, cos(radian), -1 * sin(radian), 0, 0, sin(radian), cos(radian), 0, 0, 0, 0, 1);
    for(int i = 0; i < a.size(); i++){
        Mat temp = matrixMultiply(rotate1, a[i]);
        output.push_back(temp);
    }
    return output;

}
Mat perspective_projection_cube(vector<Mat> a){
    Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));

    Mat transformtocenter = (Mat_<double>(4, 4) << 1, 0, 0, 400, 0, 1, 0, 300, 0, 0, 1, 0, 0, 0, 0, 1);
    vector<Point> points;
    for (int i = 0; i < a.size(); i++)
    {
        Mat temp = a[i];
        int eye = 1200, plane = 500;
        double t = (plane - eye) / (temp.at<double>(2, 0) - eye);

        int x = temp.at<double>(0, 0) * t + 400;
        int y = temp.at<double>(1, 0) * t + 300;
        points.push_back(Point(x, y));
        //cout << Point(x, y) << endl;
        if (x > 0 && x < 800 && y > 0 && y < 600)
        {

            circle(image, Point(x, y), 2, Scalar(0, 0, 0), 3);
            // image.at<Vec3b>(x, y) = Vec3b(0, 0, 0);
        }
    }
    line(image, points[0], points[2], Scalar(0, 0, 0));
    line(image, points[0], points[4], Scalar(0, 0, 0));
    line(image, points[0], points[1], Scalar(0, 0, 0));
    line(image, points[1], points[3], Scalar(0, 0, 0));
    line(image, points[1], points[5], Scalar(0, 0, 0));
    line(image, points[2], points[6], Scalar(0, 0, 0));
    line(image, points[2], points[3], Scalar(0, 0, 0));
    line(image, points[4], points[5], Scalar(0, 0, 0));
    line(image, points[4], points[6], Scalar(0, 0, 0));
    line(image, points[7], points[5], Scalar(0, 0, 0));
    line(image, points[7], points[6], Scalar(0, 0, 0));
    line(image, points[7], points[3], Scalar(0, 0, 0));

    return image;
}
Mat perspective_projection_tetra(vector<Mat> a)
{
    Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));

   // Mat transformtocenter = (Mat_<double>(4, 4) << 1, 0, 0, 400, 0, 1, 0, 300, 0, 0, 1, 0, 0, 0, 0, 1);
    vector<Point> points;       
    for (int i = 0; i < a.size(); i++)
    {
        Mat temp = a[i];
        int eye = 1200, plane = 500;
        double t = (plane - eye) / (temp.at<double>(2, 0) - eye);
        int x = temp.at<double>(0, 0) * t + 400;
        int y = temp.at<double>(1, 0) * t + 300;
        points.push_back(Point(x, y));
        if (x > 0 && x < 800 && y > 0 && y < 600)
        {
            circle(image, Point(x, y), 2, Scalar(0, 0, 0), 3);
            // image.at<Vec3b>(x, y) = Vec3b(0, 0, 0);
        }
    }
    line(image, points[0], points[1], Scalar(0, 0, 0));
    line(image, points[0], points[2], Scalar(0, 0, 0));
    line(image, points[0], points[3], Scalar(0, 0, 0));
    line(image, points[1], points[2], Scalar(0, 0, 0));
    line(image, points[1], points[3], Scalar(0, 0, 0));
    line(image, points[2], points[3], Scalar(0, 0, 0));

    return image;
}
Mat to2dim(vector<Mat> a){
    Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));
    
    Mat transformtocenter = (Mat_<double>(4, 4) << 1, 0, 0, 400, 0, 1, 0, 300, 0, 0, 1, 0, 0, 0, 0, 1);
    vector<Point> points;
    for(int i = 0; i < a.size(); i++){
        Mat temp = matrixMultiply(transformtocenter, a[i]);

        int x = temp.at<double>(0, 0);
        int y = temp.at<double>(1, 0);
        points.push_back(Point(x, y));
        if (x > 0 && x < 800 && y > 0 && y < 600)
        {
            circle(image, Point(x, y), 2, Scalar(0, 0, 0), 3);
            //image.at<Vec3b>(x, y) = Vec3b(0, 0, 0);
        }
    }
    line(image, points[0], points[2], Scalar(0, 0, 0));
    line(image, points[0], points[4], Scalar(0, 0, 0));
    line(image, points[0], points[1], Scalar(0, 0, 0));
    line(image, points[1], points[3], Scalar(0, 0, 0));
    line(image, points[1], points[5], Scalar(0, 0, 0));
    line(image, points[2], points[6], Scalar(0, 0, 0));
    line(image, points[2], points[3], Scalar(0, 0, 0));
    line(image, points[4], points[5], Scalar(0, 0, 0));
    line(image, points[4], points[6], Scalar(0, 0, 0));
    line(image, points[7], points[5], Scalar(0, 0, 0));
    line(image, points[7], points[6], Scalar(0, 0, 0));
    line(image, points[7], points[3], Scalar(0, 0, 0));

    return image;
}
void coordsToText(vector<Mat> a){
    fstream f;
    f.open("coordinates.txt", std::ios_base::app);
    for(int i = 0; i < a.size(); i++){
        Mat temp = a[i];
        int x = temp.at<double>(0, 0);
        int y = temp.at<double>(1, 0);
        int z = temp.at<double>(2, 0);
        f <<"(" <<  x << " " << y << ", " << z <<  "), ";
    }
    f << "\n";
    f.close();
}
Mat to2dimtetra(vector<Mat> a)
{
    Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));

    Mat transformtocenter = (Mat_<double>(4, 4) << 1, 0, 0, 400, 0, 1, 0, 300, 0, 0, 1, 0, 0, 0, 0, 1);
    vector<Point> points;
    for (int i = 0; i < a.size(); i++)
    {
        Mat temp = matrixMultiply(transformtocenter, a[i]);

        int x = temp.at<double>(0, 0);
        int y = temp.at<double>(1, 0);
        points.push_back(Point(x, y));
        if (x > 0 && x < 800 && y > 0 && y < 600)
        {
            circle(image, Point(x, y), 2, Scalar(0, 0, 0), 3);
            // image.at<Vec3b>(x, y) = Vec3b(0, 0, 0);
        }
    }
    line(image, points[0], points[1], Scalar(0, 0, 0));
    line(image, points[0], points[2], Scalar(0, 0, 0));
    line(image, points[0], points[3], Scalar(0, 0, 0));
    line(image, points[1], points[2], Scalar(0, 0, 0));
    line(image, points[1], points[3], Scalar(0, 0, 0));
    line(image, points[2], points[3], Scalar(0, 0, 0));

    return image;
}
VideoWriter cubeTransform(VideoWriter video, bool perspective){
    ofstream myfile;
    myfile.open("coordinates.txt");
    myfile.close();
    Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));
    Mat scale50 = (Mat_<double>(4, 4) << 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 1);
    double radian = 30 * M_PI / 180
    //Mat rotatedown = (Mat_<double>(4, 4) << cos(radian), 0, sin(radian), 0, 0, 1, 0, 0, -sin(radian), 0, cos(radian), 0, 0, 0, 0, 1);
    Mat transformation = scale50;
    vector<Mat> points; // store a vector of the points +- 1 represented as homogenous matrices
    for (int i = 0; i < 8; i++)
    {
        double x = (i % 4 < 2) ? 1 : -1;
        double y = (i % 2 == 0) ? 1 : -1;
        double z = (i < 4) ? 1 : -1;
        Mat point = (Mat_<double>(4, 1) << x, y, z, 1);
        Mat newpoint = matrixMultiply(transformation, point);
        points.push_back(newpoint);
    }
    fstream myf;
    myf.open("coordinates.txt", std::ios_base::app);
    myf << "Cube Coordinates:\n";
    myf.close();
    vector<Mat> rotated = points;
    for (int i = 0; i <= 180; i++)
    {
        if(i > 0 && i < 5){
            coordsToText(rotated);
        }
        rotated = rotate(points, i);
        Mat image;
        if(perspective){
            image = perspective_projection_cube(rotated);
        }
        else{
            image = to2dim(rotated);
        }d

        
        video.write(image);
    }
    return video;
}
VideoWriter tetraTransform(VideoWriter video, bool perspective)
{
    Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));
    Mat transformation = (Mat_<double>(4, 4) << 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 1);
    double radian = 30 * M_PI / 180;
    vector<Mat> points; // store a vector of the points in an platonic tetrahedron
    points.push_back((Mat_<double>(4, 1) << 1/ sqrt(3), 0, 0, 1));
    points.push_back((Mat_<double>(4, 1) << 0, 0, 2 / sqrt(6), 1));
    points.push_back((Mat_<double>(4, 1) << -1 * sqrt(3) / 6, 0.5, 0, 1));
    points.push_back((Mat_<double>(4, 1) << -1 * sqrt(3) / 6, -0.5, 0, 1));
    for (int i = 0; i < points.size(); i++)
    {
        Mat newpoint = matrixMultiply(transformation, points[i]);
        points[i] = newpoint;
    }
    fstream myf;
    myf.open("coordinates.txt", std::ios_base::app);
    myf << "Tetrahedron Coordinates:\n";
    myf.close();
    vector<Mat> rotated = points;
    for (int i = 0; i <= 180; i++)
    {

        if (i > 0 && i < 5)
        {
            coordsToText(rotated);
        }
        rotated = rotate(points, i);
        if (perspective)
        {
            image = perspective_projection_tetra(rotated);
        }
        else
        {
            image = to2dimtetra(rotated);
        }
        video.write(image);
    }
    return video;
}
void part1(){
    VideoWriter video("rotation.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(800, 600), true);
    video = cubeTransform(video, false);
    video = tetraTransform(video, false);
}
void part2(){
    VideoWriter video("rotation.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(800, 600), true);
    video = cubeTransform(video, true);
    video = tetraTransform(video, true);
}
int main(){
    //part1();
    part2();
}