#include <iostream>
#include <string>
#include <iomanip> // controlling float print precision
#include <sstream> // string to number conversion

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp> // Gaussian Blur
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp> // OpenCV window I/O

using namespace std;
using namespace cv;

// PSNR: peak-signal-to-noise ratio
// SSIM: structural similarity index measure

double getPSNR(const Mat& I1, const Mat& I2);
Scalar getMSSIM(const Mat& I1, const Mat& I2);

static void help()
{
  cout
    << "------------------------------------------------------------------------------" << endl
    << "This program shows how to read a video file with OpenCV. In addition, it "
    << "tests the similarity of two input videos first with PSNR, and for the frames "
    << "below a PSNR trigger value, also with MSSIM."                                   << endl
    << "Usage:"                                                                         << endl
    << "./video-input-psnr-ssim <referenceVideo> <useCaseTestVideo> <PSNR_Trigger_Value> <Wait_Between_Frames> " << endl
    << "--------------------------------------------------------------------------"     << endl
    << endl;
}

int main(int argc, char* argv[]) {
  help();

  if (argc != 5) {
    cout << "Not enough parameters" << endl;
    return -1;
  }

  stringstream conv;
  const string sourceReference = argv[1];
  const string sourceCompareWith = argv[2];
  int psnrTriggerValue, delay;
  conv << argv[3] << endl << argv[4];
  conv >> psnrTriggerValue >> delay;

  int frameNum = -1;

  VideoCapture captRefrnc(sourceReference), captUndTst(sourceCompareWith);
  if(!captRefrnc.isOpened()) {
    cout << "Could not open reference " << sourceReference << endl;
    return -1;
  }
  if(!captUndTst.isOpened()) {
    cout << "Could not open case test " << sourceCompareWith << endl;
    return -1;
  }

  Size refS = Size((int) captRefrnc.get(CAP_PROP_FRAME_WIDTH),
                   (int) captRefrnc.get(CAP_PROP_FRAME_HEIGHT));
  Size uTSi = Size((int) captUndTst.get(CAP_PROP_FRAME_WIDTH),
                   (int) captUndTst.get(CAP_PROP_FRAME_HEIGHT));

  if(refS != uTSi) {
    cout << "Inputs have different size!!! Closing." << endl;
    return -1;
  }

  const char* WIN_UT = "Under Test";
  const char* WIN_RF = "Reference";

  namedWindow(WIN_RF, WINDOW_AUTOSIZE);
  namedWindow(WIN_UT, WINDOW_AUTOSIZE);
  moveWindow(WIN_RF, 0, 0);
  moveWindow(WIN_UT, refS.width, 0);

  cout << "Reference frame resolution: Width=" << refS.width << " Height=" << refS.height
       << " of nr#: " << captRefrnc.get(CAP_PROP_FRAME_COUNT) << endl;
  cout << "PSNR trigger value " << setiosflags(ios::fixed) << setprecision(3)
       << psnrTriggerValue << endl;

  Mat frameReference, frameUnderTest;
  double psnrV;
  Scalar mssimV;

  for(;;) {
    captRefrnc >> frameReference;
    captUndTst >> frameUnderTest;

    if(frameReference.empty() || frameUnderTest.empty()) {
      cout << " < < < Game over! > > > ";
      break;
    }

    ++frameNum;
    cout << "Frame: " << frameNum << "# ";
    psnrV = getPSNR(frameReference, frameUnderTest);
    cout << setiosflags(ios::fixed) << setprecision(3) << psnrV << "dB";

    if(psnrV < psnrTriggerValue && psnrV) {
      mssimV = getMSSIM(frameReference, frameUnderTest);
      cout << " MSSIM: "
           << " R " << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[2] * 100 << "%"
           << " G " << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[1] * 100 << "%"
           << " B " << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[0] * 100 << "%";
    }
    cout << endl;
    imshow(WIN_RF, frameReference);
    imshow(WIN_UT, frameUnderTest);

    char c = (char)waitKey(delay);
    if (c == 27) break;
  }

  return 0;
}

double getPSNR(const Mat& I1, const Mat& I2) {
  Mat s1;
  absdiff(I1, I2, s1);
  s1.convertTo(s1, CV_32F);
  s1 = s1.mul(s1);

  Scalar s = sum(s1);
  double sse = s.val[0] + s.val[1] + s.val[2];
  if(sse <= 1e-10)
    return 0.0;
  else {
    double mse = sse / (double)(I1.channels() * I1.total());
    double psnr = 10.0 * log10((255 * 255) / mse);
    return psnr;
  }
}

Scalar getMSSIM(const Mat& i1, const Mat& i2) {
  const double C1 = 6.5025, C2 = 58.5225;
  int d = CV_32F;
  Mat I1, I2;
  i1.convertTo(I1, d);
  i2.convertTo(I2, d);
  Mat I2_2 = I2.mul(I2);
  Mat I1_2 = I1.mul(I1);
  Mat I1_I2 = I1.mul(I2);

  Mat mu1, mu2;
  GaussianBlur(I1, mu1, Size(11, 11), 1.5);
  GaussianBlur(I2, mu2, Size(11, 11), 1.5);

  Mat mu1_2 = mu1.mul(mu1);
  Mat mu2_2 = mu2.mul(mu2);
  Mat mu1_mu2 = mu1.mul(mu2);

  Mat sigma1_2, sigma2_2, sigma12;
  GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
  sigma1_2 -= mu1_2;
  GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
  sigma2_2 -= mu2_2;
  GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
  sigma12 -= mu1_mu2;

  Mat t1, t2, t3;

  t1 = 2 * mu1_mu2 + C1;
  t2 = 2 * sigma12 + C2;
  t3 = t1.mul(t2);

  t1 = mu1_2 + mu2_2 + C1;
  t2 = sigma1_2 + sigma2_2 + C2;
  t1 = t1.mul(t2);

  Mat ssim_map;
  divide(t3, t1, ssim_map);

  Scalar mssim = mean(ssim_map);
  return mssim;
}
