#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <chrono>
using namespace std;
using namespace cv;

Mat rotate(Mat src, double angle)   //rotate function returning mat object with parametres imagefile and angle
{
    Mat dst;      //Mat object for output image file
    Point2f pt(src.cols/2., src.rows/2.);          //point from where to rotate
    Mat r = getRotationMatrix2D(pt, angle, 1.0);      //Mat object for storing after rotation
    warpAffine(src, dst, r, Size(src.cols, src.rows));  // apply an affine transformation to image.
    return dst;         //returning Mat object for output image file
}
int main( int argc, char** argv )
{
    Mat img; Mat templ; Mat result; Mat img_display;
    if (argc < 4)
    {
        cout << "Not enough parameters" << endl;
        cout << "Usage:\n" << argv[0] << " <image_name> <template_name> <threshold>" << endl;
        return -1;
    }

    const clock_t begin_time = clock();
    img = imread( argv[1], IMREAD_COLOR );//read Reference Image
    templ = imread( argv[2], IMREAD_COLOR ); //read Template Image
    float threshold=stof(argv[3]); // matching threshold
    int rotate_angle;
    int h,w;
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    result.create( result_rows, result_cols, CV_32FC1 );
    Point matchLoc;
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    img.copyTo( img_display );
    for (rotate_angle=0;rotate_angle<360;rotate_angle+=30) //try rotation angles
    {
        templ = rotate(templ,rotate_angle);
        //get h and w value from template for draw rectangle
        h=templ.cols;
        w=templ.rows;
        matchTemplate( img, templ, result, TM_CCOEFF_NORMED); // calculate  Correlation coefficient (TM_CCOEFF)
        
        // get best matching coord
        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
        matchLoc = maxLoc;
        if (maxVal>=threshold) {
            rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
                      CV_RGB(36, 255, 12), 2, 8, 0); //draw matching coord rectangle
            //Write positions
            putText(img_display,format("[%d,%d]",matchLoc.x,matchLoc.y),Point( matchLoc.x-150, matchLoc.y ),FONT_HERSHEY_SIMPLEX, 0.9, CV_RGB(36,255,12), 1);
            putText(img_display,format("[%d,%d]",matchLoc.x+templ.cols,matchLoc.y),Point( (matchLoc.x+templ.cols)+15, matchLoc.y ),FONT_HERSHEY_SIMPLEX, 0.9, CV_RGB(36,255,12), 1);
            putText(img_display,format("[%d,%d]",matchLoc.x,matchLoc.y+templ.rows),Point( matchLoc.x-150, matchLoc.y+ templ.rows),FONT_HERSHEY_SIMPLEX, 0.9, CV_RGB(36,255,12), 1);
            putText(img_display,format("[%d,%d]",matchLoc.x+templ.cols,matchLoc.y+templ.rows),Point( (matchLoc.x+templ.cols)+15, matchLoc.y+templ.rows),FONT_HERSHEY_SIMPLEX, 0.9, CV_RGB(36,255,12), 1);

        }
    }


    cout <<"Total Process Time : " <<float( clock () - begin_time ) /  CLOCKS_PER_SEC<<" seconds"<<endl;
    imwrite("results/template_match_result.png",img_display); //show position of template on reference image
    imshow("result",img_display);
    waitKey(0);

    return EXIT_SUCCESS;
}