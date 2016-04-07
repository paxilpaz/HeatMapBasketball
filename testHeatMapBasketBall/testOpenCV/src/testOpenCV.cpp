//============================================================================
// Name        : testOpenCV.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "HeatMap.h"

using namespace std;
using namespace cv;


int main() {

	Mat test = imread("/home/paxilpaz/Desktop/halfcourt.png");

	cout << test.cols << ", " << test.rows << endl;
	imshow("test", test);
	waitKey(0);
	cvDestroyAllWindows();

	vector<Point> points;

	points.push_back(Point(278,61));
	points.push_back(Point(282,77));
	points.push_back(Point(305,81));
	points.push_back(Point(329,67));
	points.push_back(Point(92,311));
	points.push_back(Point(306,370));
	points.push_back(Point(330,265));
	points.push_back(Point(422,199));
	points.push_back(Point(408,167));
	points.push_back(Point(417,126));
	points.push_back(Point(389,106));
	points.push_back(Point(365,64));
	points.push_back(Point(344,79));
	points.push_back(Point(330,103));
	points.push_back(Point(298,107));
	points.push_back(Point(259,84));
	points.push_back(Point(230,122));
	points.push_back(Point(203,66));

	Mat mask(test.rows, test.cols, CV_8UC3);
	Mat heat = create_heatmap(points, test.cols, test.rows);

	imshow("tst", heat);
	waitKey(0);

	double alpha = 0.75;
	double beta = 1.0 - alpha;
	Mat dest;
	addWeighted(heat, alpha, test, beta, 0.0, dest);
	imshow("Res", dest);
	waitKey(0);

	imwrite("/home/paxilpaz/heatmap.jpg", dest);

	return 0;
}
