#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
  Mat C = (Mat_<double>(3,3) << 0,-1,0,-1,5,-1,0,-1,0);

  cout << "C = " << endl << " " << C << endl << endl;

  Mat RowClone = C.row(1).clone();
  cout << "RowClone = " << endl << " " << RowClone << endl << endl;

  Mat R = Mat(3,2, CV_8UC3);
  randu(R, Scalar::all(0), Scalar::all(255));
  cout << "R = " << endl << " " << R << endl << endl;

  Point2f P(5, 1);
  cout << "Point (2D) = " << P << endl << endl;

  Point3f P3f(2,6,7);
  cout << "Point (3D) = " << P3f << endl << endl;

  vector<float> v;
  v.push_back( (float)CV_PI );
  v.push_back(2);
  v.push_back(3.01f);
  cout << "Vector of floats via Mat = " << Mat(v) << endl << endl;

  vector<Point2f> vPoints(20);
  for (size_t i = 0; i < vPoints.size(); i++)
    vPoints[i] = Point2f((float)(i * 5), (float)(i%7));
  cout << "A vector of 2D Points = " << vPoints << endl << endl;

}
