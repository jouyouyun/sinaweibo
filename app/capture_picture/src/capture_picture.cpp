/*
 * There are also python implementation for camera capture and showing image.
 */

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#define IMG_PATH "/tmp/cap_pic.png"

int main( int argc, char **argv )
{
    CvCapture* capture = NULL;
    IplImage *frame = NULL;

    //0=default, -1=any camera, 1..99=your camera
    capture = cvCaptureFromCAM( CV_CAP_ANY );
    if(!capture) {
        cout << "No camera detected" << endl;
        return -1;
    }

    frame = cvQueryFrame( capture );
    if( !frame ) {
        cout << "ERROR: frame is null..." << endl;
        cvReleaseCapture( &capture );
        return -1;
    }
    
    //imwrite(argv[1], cvarrToMat(frame));
    cvSaveImage(IMG_PATH, frame);

    cvReleaseCapture( &capture );
    return 0;
}
