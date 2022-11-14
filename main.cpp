#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int main( int argc, char** argv )
{
    cv::Mat frame,ori,hasil;
    cv::VideoCapture video("no4.mp4");
    
    /*
    cv::Mat element = cv::getStructuringElement
    (
        cv::MORPH_RECT,cv::Size(5,5),cv::Point(2,2)
    );
    */

    int minimumH = 0;
    int maksimumH = 119; //105
    int minimumS = 70; 
    int maksimumS = 255;
    int minimumV = 0;
    int maksimumV = 255;

    cv::namedWindow("Trackbar HSV", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Minimum Hue", "Trackbar HSV", &minimumH, 255);
    cv::createTrackbar("Maksimum Hue", "Trackbar HSV", &maksimumH, 255);
    cv::createTrackbar("Minimum Saturation", "Trackbar HSV", &minimumS, 255);
    cv::createTrackbar("Maksimum Saturation", "Trackbar HSV", &maksimumS, 255);
    cv::createTrackbar("Minimum Value", "Trackbar HSV", &minimumV, 255);
    cv::createTrackbar("Maksimum Value", "Trackbar HSV", &maksimumV, 255);

    std::vector<std::vector<cv::Point>> contours;
    std::vector <cv::Vec4i> hierarchy;
    cv::Mat drawing;

    while(true)
    {   
        cv::Scalar Mini(minimumH, minimumS, minimumV);
        cv::Scalar Maxi(maksimumH, maksimumS, maksimumV);

        video.read(frame);
        video.read(ori);
        cv::cvtColor(frame,frame,cv::COLOR_BGR2HSV);
        inRange(frame, Mini, Maxi, frame);
        cv::imshow("webcam",frame);
        cv::imshow("Original", ori);

        drawing = cv::Mat::zeros(frame.size(), CV_8UC3); 
        cv::findContours(frame, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        for(int i=0; i<contours.size(); i++)
        {
            int area = cv::contourArea(contours[i]);
            if(area < 1000)
            {
                contours.erase(contours.begin()+i);
            }
        }
        std::vector<std::vector<cv::Point>> hull( contours.size() );
        for(int i=0; i<contours.size(); i++)
        {
            cv::convexHull(contours[i], hull[i]);
            cv::Scalar color = cv::Scalar(255,255,255);
            cv::drawContours(drawing, contours, i, color, -1);
        }
        //cv::imshow("Hull",drawing);
        //cv::morphologyEx(drawing,drawing,cv::MORPH_OPEN,element,cv::Point(-1,-1),2);
        cv::bitwise_and(ori,drawing,hasil);
        cv::imshow("Hasil",hasil);

        cv::waitKey(30);

    }
    return 0;
}