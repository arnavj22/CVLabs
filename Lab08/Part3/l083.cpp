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
ofstream logger("log.txt");
double magnitude(Mat a){
    double sum = 0;
    for(int i = 0; i < a.rows; i++){
        for(int j = 0; j < a.cols - 1; j++){
            sum += pow(a.at<double>(i,j), 2);
        }
    }
    return sqrt(sum);
}
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
    Mat rotate2 = (Mat_<double>(4, 4) << cos(radian), 0, sin(radian), 0, 0, 1, 0, 0, -1 * sin(radian), 0, cos(radian), 0, 0, 0, 0, 1);
    for(int i = 0; i < a.size(); i++){
        Mat temp = matrixMultiply(rotate1, a[i]);
        temp = matrixMultiply(rotate2, temp);
        output.push_back(temp);
    }
    return output;

}
Mat get_projection(Mat coord, Mat e, Mat alpha, Mat n){
    double t = (alpha - e).dot(n) / (coord - e).dot(n);
    return t * (coord - e) + e;
}
string MatToString(Mat a)
{
    return "(" + to_string(a.at<double>(0, 0)) + ", " + to_string(a.at<double>(1, 0)) + ", " + to_string(a.at<double>(2, 0)) + ")";
}
void write_edge_2d(vector<Point> a, bool cube = true){
    if (cube)
    {
        logger << "\n\t" << a[0] << ", " << a[1];
        logger << "\n\t" << a[0] << ", " << a[2];
        logger << "\n\t" << a[0] << ", " << a[4];
        logger << "\n\t" << a[1] << ", " << a[3];
        logger << "\n\t" << a[1] << ", " << a[5];
        logger << "\n\t" << a[2] << ", " << a[3];
        logger << "\n\t" << a[2] << ", " << a[6];
        logger << "\n\t" << a[3] << ", " << a[7];
        logger << "\n\t" << a[4] << ", " << a[5];
        logger << "\n\t" << a[4] << ", " << a[6];
        logger << "\n\t" << a[5] << ", " << a[7];
        logger << "\n\t" << a[6] << ", " << a[7];
    }
    else{
        logger << "\n\t" << a[0] << ", " << a[1];
        logger << "\n\t" << a[0] << ", " << a[2];
        logger << "\n\t" << a[0] << ", " << a[3];
        logger << "\n\t" << a[1] << ", " << a[2];
        logger << "\n\t" << a[1] << ", " << a[3];
        logger << "\n\t" << a[2] << ", " << a[3];
    }
}

void write_edge(vector<Mat> a, bool cube = true)
{
    if (cube)
    {
        logger << "\n\t" << MatToString(a[0]) << ", " << MatToString(a[1]);
        logger << "\n\t" << MatToString(a[0]) << ", " << MatToString(a[2]);
        logger << "\n\t" << MatToString(a[0]) << ", " << MatToString(a[4]);
        logger << "\n\t" << MatToString(a[1]) << ", " << MatToString(a[3]);
        logger << "\n\t" << MatToString(a[1]) << ", " << MatToString(a[5]);
        logger << "\n\t" << MatToString(a[2]) << ", " << MatToString(a[3]);
        logger << "\n\t" << MatToString(a[2]) << ", " << MatToString(a[6]);
        logger << "\n\t" << MatToString(a[3]) << ", " << MatToString(a[7]);
        logger << "\n\t" << MatToString(a[4]) << ", " << MatToString(a[5]);
        logger << "\n\t" << MatToString(a[4]) << ", " << MatToString(a[6]);
        logger << "\n\t" << MatToString(a[5]) << ", " << MatToString(a[7]);
        logger << "\n\t" << MatToString(a[6]) << ", " << MatToString(a[7]);
    }
    else
    {
        logger << "\t" << MatToString(a[0]) << ", " << MatToString(a[1]);
        logger << "\n\t" << MatToString(a[0]) << ", " << MatToString(a[2]);
        logger << "\n\t" << MatToString(a[0]) << ", " << MatToString(a[3]);
        logger << "\n\t" << MatToString(a[1]) << ", " << MatToString(a[2]);
        logger << "\n\t" << MatToString(a[1]) << ", " << MatToString(a[3]);
        logger << "\n\t" << MatToString(a[2]) << ", " << MatToString(a[3]);
    }
}
Mat perspective_cube_full(vector<Mat> a, Mat alpha, Mat n, Mat e, Mat w1, Mat w2, bool should_write)
{
    Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));
    // for(int i = 0; i < a.size(); i++){
    //     a[i].at<double>(0, 0) = a[i].at<double>(1, 0) * 400;
    //     a[i].at<double>(1, 0) = a[i].at<double>(1, 1) * 300;
    // }

    // Mat transformtocenter = (Mat_<double>(4, 4) << 1, 0, 0, 400, 0, 1, 0, 300, 0, 0, 1, 0, 0, 0, 0, 1);
    vector<Point> points;
    for (int i = 0; i < a.size(); i++)
    {

        Mat temp = a[i];
        Mat zero = (Mat_<double>(4, 1) << 0, 0, 0, 0);
        Mat p0 = get_projection(zero, e, alpha, n);
        double u = (get_projection(temp, e, alpha, n) - p0).dot(w1) / (w1.dot(w1)) + 400;
        double v = (get_projection(temp, e, alpha, n) - p0).dot(w2) / (w2.dot(w2)) + 300;
        // int x = temp.at<double>(0, 0) * t + 400;
        // int y = temp.at<double>(1, 0) * t + 300;
        points.push_back(Point(u, v));
        //cout << Point(u, v) << endl;
        if (u > 0 && u < 800 && v > 0 && v < 600)
        {

            circle(image, Point(u, v), 2, Scalar(0, 0, 0), 3);
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
    //cout << "hi" << endl;
    if (should_write)
    {
        write_edge_2d(points, true);
    }
    return image;
}
Mat perspective_tetra_full(vector<Mat> a, Mat alpha, Mat n, Mat e, Mat w1, Mat w2, bool should_write = false)
{
    Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));

    // Mat transformtocenter = (Mat_<double>(4, 4) << 1, 0, 0, 400, 0, 1, 0, 300, 0, 0, 1, 0, 0, 0, 0, 1);
    vector<Point> points;
    for (int i = 0; i < a.size(); i++)
    {

        Mat temp = a[i];
        Mat zero = (Mat_<double>(4, 1) << 0, 0, 0, 0);
        Mat p0 = get_projection(zero, e, alpha, n);
        double u = (get_projection(temp, e, alpha, n) - p0).dot(w1) / (w1.dot(w1)) + 400;
        double v = (get_projection(temp, e, alpha, n) - p0).dot(w2) / (w2.dot(w2)) + 300;
        // int x = temp.at<double>(0, 0) * t + 400;
        // int y = temp.at<double>(1, 0) * t + 300;
        points.push_back(Point(u, v));
        // cout << Point(u, v) << endl;
        if (u > 0 && u < 800 && v > 0 && v < 600)
        {

            circle(image, Point(u, v), 2, Scalar(0, 0, 0), 3);
            // image.at<Vec3b>(x, y) = Vec3b(0, 0, 0);
        }
    }
    line(image, points[0], points[1], Scalar(0, 0, 0));
    line(image, points[0], points[2], Scalar(0, 0, 0));
    line(image, points[0], points[3], Scalar(0, 0, 0));
    line(image, points[1], points[2], Scalar(0, 0, 0));
    line(image, points[1], points[3], Scalar(0, 0, 0));
    line(image, points[2], points[3], Scalar(0, 0, 0));
    if(should_write){
        write_edge_2d(points, false);
    }
    return image;
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
VideoWriter cubeTransform(VideoWriter video, int perspective){
    Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));
    Mat scale50 = (Mat_<double>(4, 4) << 200, 0, 0, 0, 0, 200, 0, 0, 0, 0, 200, 0, 0, 0, 0, 1);
    double radian = 30 * M_PI / 180;
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
    // fstream myf;
    // myf.open("coordinates.txt", std::ios_base::app);
    // myf << "Cube Coordinates:\n";
    // myf.close();
    vector<Mat> rotated = points;
    // for(int i = 0; i < rotated.size(); i++){
    //     cout << rotated[i] << endl;
    // }
    logger << "CUBE:\nThe plane defined by (x-a)*n=0 is:";
    logger << setprecision(2);

    Mat alpha = (Mat_<double>(4, 1) << 400, 300, 500, 0);
    Mat n = (Mat_<double>(4, 1) << 1, 1, 2, 0);
    Mat e = (Mat_<double>(4, 1) << 600, 600, 500, 0);
    Mat vec = (Mat_<double>(4, 1) << 0, 0, 0, 0);

    double t = ((alpha - e).dot(n) / (vec - e).dot(n));
    Mat a1 = get_projection(points[1], e, alpha, n) - get_projection(points[0], e, alpha, n);
    Mat a2 = get_projection(points[2], e, alpha, n) - get_projection(points[0], e, alpha, n);
    //cout << a1<< a2 << endl;
    Mat w1 = 1 / (sqrt(a1.dot(a1))) * a1;
    Mat c = a2 - (((a1.dot(a2))) / (a1.dot(a1)) * a1);

    Mat w2 = 1 / (sqrt(c.dot(c))) * c;
    Mat p0 = get_projection(vec, e, alpha, n);
    logger << "\n\ta = " << MatToString(alpha);
    logger << "\n\tn = " << MatToString(n);
    logger << "\nThe eye e is :\n\te = " << MatToString(e);
    logger << "\nThe coordinates in the plane x = p0 + u*w1 + v*w2 is:\n";
    logger << "\n\tp0 = " << MatToString(p0);
    logger << "\n\tw1 = " << MatToString(w1);
    logger << "\n\tw2 = " << MatToString(w2);
    

    for (int i = 0; i <= 360; i++)
    {
        bool should_write = false;
        if(i > 0 && i < 5){
            logger << "\n\nThe frame" << i + 1 <<  " in 3d has the following edges:";
            write_edge(rotated, true);
            logger << "\n\nThe frame" << i + 1 << " in 2d has the following edges:";
            should_write = true;
        }
        rotated = rotate(points, i);
        Mat image;
        if (perspective == 1)
        {
            image = perspective_projection_cube(rotated);
        }
        else if (perspective == 0)
        {
            image = to2dim(rotated);
        }
        else
        {
            
            image = perspective_cube_full(rotated, alpha, n, e, w1, w2, should_write);
        }

        video.write(image);
    }
    return video;
    logger.close();
}
VideoWriter tetraTransform(VideoWriter video, int perspective)
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
    // fstream myf;
    // myf.open("coordinates.txt", std::ios_base::app);
    // myf << "Tetrahedron Coordinates:\n";
    // myf.close();
    vector<Mat> rotated = points;
    Mat alpha = (Mat_<double>(4, 1) << 500, 300, 200, 0);
    Mat n = (Mat_<double>(4, 1) << 1, 2, 3, 0);
    Mat e = (Mat_<double>(4, 1) << 800, 50, 123, 0);
    Mat vec = (Mat_<double>(4, 1) << 0, 0, 0, 0);
    double t = (alpha - e).dot(n) / (vec - e).dot(n);
    Mat a1 = get_projection(points[0], e, alpha, n) - get_projection(points[1], e, alpha, n);
    Mat a2 = get_projection(points[2], e, alpha, n) - get_projection(points[1], e, alpha, n);
    // cout << a1<< a2 << endl;
    Mat w1 = 1 / (sqrt(a1.dot(a1))) * a1;
    Mat c = a2 - (((a1.dot(a2))) / (a1.dot(a1)) * a1);

    Mat w2 = 1 / (sqrt(c.dot(c))) * c;
    Mat p0 = get_projection(vec, e, alpha, n);
    logger << "\nTETRAHEDRON:\nThe plane defined by (x-a)*n=0 is:";
    logger << "\n\ta = " << MatToString(alpha);
    logger << "\n\tn = " << MatToString(n);
    logger << "\nThe eye e is :\n\te = " << MatToString(e);
    logger << "\nThe coordinates in the plane x = p0 + u*w1 + v*w2 is:\n";
    logger << "\n\tp0 = " << MatToString(p0);
    logger << "\n\tw1 = " << MatToString(w1);
    logger << "\n\tw2 = " << MatToString(w2);
    for (int i = 0; i <= 360; i++)
    {
        bool should_write = false;
        if (i > 0 && i < 5)
        {
            logger << "\n\nThe frame" << i + 1 << " in 3d has the following edges:";
            write_edge(rotated, false);
            logger << "\n\nThe frame" << i + 1 << " in 2d has the following edges:";
            should_write = true;
        }
         rotated = rotate(points, i);
        if (perspective == 1)
        {
            image = perspective_projection_tetra(rotated);
        }
        else if(perspective == 0)
        {
            image = to2dimtetra(rotated);
        }
        else{
            image = perspective_tetra_full(rotated, alpha, n, e, w1, w2, should_write);
        }
        video.write(image);
    }
    return video;
}
void part1(){
    VideoWriter video("rotation.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(800, 600), true);
    video = cubeTransform(video, 0);
    video = tetraTransform(video, 0);
}
void part2(){
    VideoWriter video("rotation.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(800, 600), true);
    video = cubeTransform(video, 1);
    video = tetraTransform(video, 1);
}
void part3(){

    VideoWriter video("rotation.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 60, Size(800, 600), true);
    video = cubeTransform(video, 2);
    video = tetraTransform(video, 2);
}
int main(){
    //part1();
    //part2();
    part3();
}