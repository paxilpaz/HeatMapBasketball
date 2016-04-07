/*
 * HeatMap.h
 *
 *  Created on: 4 Apr 2016
 *      Author: paxilpaz
 */
#ifndef HEATMAP_H_
#define HEATMAP_H_

#define MAX_VAL 255
#define THRESH 1

#define RADIUS 20

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

typedef struct S_MY_POINT {
	Point p;
	unsigned long new_val;
} MyPoint;

/* Computes the heat map from a grayscale image */
void heat_map_computation(Mat source, Mat* heat);

Mat create_heatmap(vector<Point> points,int image_width,int image_height);

void compute_matrices(Mat difference,Mat* a,unsigned long** b,unsigned long* max);
#endif
