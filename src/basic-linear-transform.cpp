#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using namespace cv;

int main(int argc, char** argv)
{
  CommandLineParser parser(argc, argv, "{@input | lena.jpg | input image}");
  Mat image = imread(samples::findFile(parser.get<String>("@input")));
  if(image.empty())
  {
    cout << "Could not open or find the image!" << endl;
    cout << "Usage: " << argv[0] << " <Input image>" << endl;
    return -1;
  }

  Mat new_image = Mat::zeros(image.size(), image.type());
  Mat new_image2 = Mat::zeros(image.size(), image.type());

  double alpha = 1.0;
  int beta = 0;
  double t;

  cout << " Basic Linear Transforms " << endl;
  cout << "_________________________" << endl;
  cout << "* Enter the alpha value [1.0-3.0]: ";
  cin >> alpha;
  cout << "* Enter the beta value [0-100]: ";
  cin >> beta;

  t = (double)getTickCount();
  for(int y = 0; y < image.rows; y++) {
    for(int x = 0; x < image.cols; x++) {
      for(int c = 0; c < image.channels(); c++) {
        new_image.at<Vec3b>(y,x)[c] =
          saturate_cast<uchar>(alpha * image.at<Vec3b>(y,x)[c] + beta);
      }
    }
  }
  t = 1000 * ((double)getTickCount() - t) / getTickFrequency();
  cout << "Time of transform by accessing each pixel: "
       << t << " ms." << endl;

  t = (double)getTickCount();
  image.convertTo(new_image2, -1, alpha, beta);
  t = 1000 * ((double)getTickCount() - t) / getTickFrequency();
  cout << "Time of transform by `convertTo` function: "
       << t << " ms." << endl;


  imshow("Original Image", image);
  imshow("New Image by Accessing Each Pixel", new_image);
  imshow("New Image by convertTo Function", new_image2);

  waitKey();
  return 0;
}
