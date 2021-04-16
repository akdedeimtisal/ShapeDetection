#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
Mat imgBlur, imgCanny, imgGray, imgDil;

void getContours(Mat imgDil, Mat img) {

	
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int area;
	string objectType;
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++) {
		area = contourArea(contours[i]);
		cout << area << endl; 
		vector<vector<Point>> conPoly(contours.size());
		vector<Rect> boundRect(contours.size());
		if (area > 1000) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);
			int objCor = (int)conPoly[i].size();
			if (objCor == 3) {
				objectType = "triangle";
			}
			if (objCor == 4) {
				float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
				cout << aspRatio << endl;
				if (aspRatio > 0.95 && aspRatio < 1.05) {
					objectType = "square";
				}
				else {
					objectType = "rectangle";
				}
				
			}
			if (objCor > 4) {
				objectType = "circle";
			}
			drawContours(img, conPoly, i, Scalar(255, 0, 255), 1);

			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);

			putText(img, objectType, { boundRect[i].x, boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN, 1, Scalar(0, 69, 255),1);
		}
	}
	
}



void main() {

	string path = "C:/Users/ASUS/Desktop/shapes.jpg";
	Mat img = imread(path);	
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);
	getContours(imgDil, img);
	imshow("Image", img);

	waitKey(0);
}