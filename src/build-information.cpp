#include <opencv2/core/utility.hpp>
#include <fstream>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
  ofstream outfile("build_info.txt");

  if(outfile.is_open()) {
    outfile << getBuildInformation();
    outfile.close();
  } else {
    cerr << "Error opening file for writing.";
    return 1;
  }
  return 0;
}
