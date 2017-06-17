#include <opencv2/core/core.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>


int main( int argc, char** argv )
{
    if( argc != 2)
    {
     std::cout <<" Usage: graphcut ImageToCut" << std::endl;
     return -1;
    }

    cv::Mat image;
    image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file with colors (3 channels)

    if(! image.data )                              // Check for invalid input
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
    cv::imshow( "Display window the image waiting to show", image );                   // Show our image inside it.
    // cv::waitKey(0);                                          // Wait for a keystroke in the window
    std::cout << "display okay now start processing" << '\n';

    cv::destroyAllWindows();
    // sleep(1);
    // Open another image

    cv::Mat image2 = image.clone();
    std::cout << image.cols<<image.rows << '\n';

    // define bounding rectangle
    cv::Rect rectangle(40,20,image.cols-80,image.rows-190);

    cv::Mat result; // segmentation result (4 possible values)
    cv::Mat bgModel,fgModel; // the models (internally used)

    // GrabCut segmentation
    cv::grabCut(image,    // input image
                result,   // segmentation result
                rectangle,// rectangle containing foreground
                bgModel,fgModel, // models
                1,        // number of iterations
                cv::GC_INIT_WITH_RECT); // use rectangle
    std::cout << "oks cut the graph okay" <<std::endl;
    // Get the pixels marked as likely foreground
    cv::compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);
    // Generate output image
    cv::Mat foreground(image.size(),CV_8UC3,cv::Scalar(255,255,255));
    cv::Mat background(image.size(),CV_8UC3,cv::Scalar(255,255,255));
    image.copyTo(foreground,result); // bg pixels not copied
    image.copyTo(background,~result); // bg pixels not copied

    // draw rectangle on original image
    cv::rectangle(image, rectangle, cv::Scalar(255,255,255),1);

    imwrite("exe/illustraion.jpg",image);
    imwrite("exe/Foreground.jpg",foreground);
    // cv::Mat background = image2 - foreground;
    imwrite("exe/Background.jpg",background);
    return 0;
}
