#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/videoio.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <opencv2/calib3d.hpp>
using namespace cv;
using namespace std;
void part1(){
    //from a video of a chessboard, create a cube around the center of the chessboard
    //and save it as a new video
    VideoCapture cap("withChessBoard.MOV");
    if(!cap.isOpened()){
        cout << "Error opening video stream or file" << endl;
        return;
    }
    vector<Point3f> objp;
    vector<vector<Point2f>> imgpoints;
    vector<vector<Point3f>> objpoints;

    for(int i = -3; i  <= 3; i++){
        for(int j = -3; j <= 3; j++){
            objp.push_back(Point3f(j, i, 0));
        }
    }
    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    VideoWriter outputVideo;
    outputVideo.open("vr.avi", VideoWriter::fourcc('M','J','P','G'), 30, Size(frame_width, frame_height), true);
    Mat frame;
    Mat cameraMatrix, distCoeffs, R, T;
    //calculated once through calibrate Camera
    cameraMatrix = (Mat_<double>(3, 3) << 1830.435257868412, 0, 929.9735017338022, 0, 1890.874522884808, 576.6857677527559, 0, 0, 1);
    distCoeffs = (Mat_<double>(5, 1) << 0.4762869007348995, -5.41032192063459, -0.00626111840235899, -0.0005248333087438388, 23.61603627962149);
    //use find chessboard corners
    //use perspective transform to draw a cube on the center of the chessboard and save the video
    int count = 0;
    vector<Point3f> cubepoints;
    cubepoints.push_back(Point3f(-1, -1, 0));
    cubepoints.push_back(Point3f(-1, 1, 0));
    cubepoints.push_back(Point3f(1, 1, 0));
    cubepoints.push_back(Point3f(1, -1, 0));
    cubepoints.push_back(Point3f(-1, -1, -2));
    cubepoints.push_back(Point3f(-1, 1, -2));
    cubepoints.push_back(Point3f(1, 1, -2));
    cubepoints.push_back(Point3f(1, -1, -2));
    while(cap.read(frame)){
        vector<Point2f> corners;
        bool found = findChessboardCorners(frame, Size(7,7), corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);
        if(found){
            //void cornerSubPix(InputArray image, InputOutputArray corners, Size winSize, Size zeroZone, TermCriteria criteria);
            // double calibrateCamera(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints, 
            // Size imageSize, InputOutputArray cameraMatrix,
            // InputOutputArray distCoeffs, OutputArrayOfArrays rvecs, OutputArrayOfArrays tvecs);
            //drawChessboardCorners(frame, Size(7, 7), corners, found);
            solvePnP(objp, corners, cameraMatrix, distCoeffs, R, T, 0);
            vector<Point2f> newpoint;
            projectPoints(cubepoints, R, T, cameraMatrix, distCoeffs, newpoint);
            line(frame, newpoint[1], newpoint[2], Scalar(0, 0, 255), 2);
            line(frame, newpoint[2], newpoint[3], Scalar(0, 0, 255), 2);
            line(frame, newpoint[3], newpoint[0], Scalar(0, 0, 255), 2);
            line(frame, newpoint[0], newpoint[1], Scalar(0, 0, 255), 2);
            line(frame, newpoint[4], newpoint[5], Scalar(0, 0, 255), 2);
            line(frame, newpoint[5], newpoint[6], Scalar(0, 0, 255), 2);
            line(frame, newpoint[6], newpoint[7], Scalar(0, 0, 255), 2);
            line(frame, newpoint[7], newpoint[4], Scalar(0, 0, 255), 2);
            line(frame, newpoint[0], newpoint[4], Scalar(0, 0, 255), 2);
            line(frame, newpoint[1], newpoint[5], Scalar(0, 0, 255), 2);
            line(frame, newpoint[2], newpoint[6], Scalar(0, 0, 255), 2);
            line(frame, newpoint[3], newpoint[7], Scalar(0, 0, 255), 2);

            outputVideo.write(frame);
            objpoints.push_back(objp);
            imgpoints.push_back(corners);
        }
    }
    //Mat transformation = (Mat_<double>(3, 3) << 2321301151669015, 0, 447.4379278198764, 0, 486.8051200717729, -285.7601543256591, 0, 0, 1);
   
    //calibrateCamera(objpoints, imgpoints, Size(CAP_PROP_FRAME_WIDTH, CAP_PROP_FRAME_HEIGHT), cameraMatrix, distCoeffs, R, T);
    
    return;
    
}
int main(){
    part1();
}