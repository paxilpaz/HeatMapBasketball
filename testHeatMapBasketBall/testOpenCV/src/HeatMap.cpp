#include "HeatMap.h"


void heat_map_computation(Mat source, Mat* heat) {
    double min=-1.0;
    double max=-1.0;
    minMaxIdx(source, &min, &max);

    /*
    for(int i = 0; i < source.cols; i++) {
        for(int j = 0; j < source.rows; j++) {
            double intensity = source.at<double>(j,i);
            if(min==-1.0 && intensity > 0) {
                min = intensity;
            }
            else {
                if(intensity < min && intensity > 0) {
                    min = intensity;
                }
            }
            if(max==-1.0) {
                max = intensity;
            }
            else {
                if(intensity > max) {
                    max = intensity;
                }
            }
        }
    }*/

    Mat adjMap;
    source.convertTo(adjMap,CV_8UC1,MAX_VAL/(max-min),min);
    applyColorMap(adjMap, *heat, cv::COLORMAP_JET);

    adjMap.release();
}

Mat create_heatmap(vector<Point> points,int image_width,int image_height) {

    /* Creates two matrices, one to store the number of changes occurred to each pixel,
     the other to store the 8-bit value associated to each pixel (grayscale image) */
    Mat a = Mat::zeros(Size(image_width,image_height), CV_64FC1);

    /*Riuso spudorato del codice*/
    Mat difference = Mat::zeros(Size(image_width,image_height), CV_8UC3);

    //Inizializzazione di B

    unsigned long** b;
    b = new unsigned long*[image_width];
    for (int i = 0; i<image_width; i++) {
        b[i] = new unsigned long[image_height];
        for (int j = 0; j<image_height; j++) {
            b[i][j]=0;
        }
    }
    unsigned long max = 0;

    //Scorrere traiettorie

    for (int i = 0; i < points.size() ;++i) {
    	cout << "Computing point " << (i+1) << " of " << points.size() << endl;
    	for (int j = points.at(i).x - RADIUS; j <= points.at(i).x + RADIUS; ++j ) {
    		for (int k = points.at(i).y - RADIUS; k <= points.at(i).y + RADIUS; ++k) {
    			if ((j >= 0 && j <= image_width) && (k >= 0 && k <= image_height)) {
    				int distance_from_center = (int) (sqrt((j - points.at(i).x)*(j - points.at(i).x) + (k - points.at(i).y)*(k - points.at(i).y)));
    				if (distance_from_center <= RADIUS) {
    					Vec3b &color = difference.at<Vec3b>(Point(j,k));
    					color.val[0] = (RADIUS - distance_from_center) + 2;
    					color.val[1] = (RADIUS - distance_from_center) + 2;
    					color.val[2] = (RADIUS - distance_from_center) + 2;
    					//difference.data[difference.step[0] * k + difference.step[1] * j + 0] = (RADIUS - distance_from_center) + 2;
    					//difference.data[difference.step[0] * k + difference.step[1] * j + 1] = (RADIUS - distance_from_center) + 2;
    					//difference.data[difference.step[0] * k + difference.step[1] * j + 2] = (RADIUS - distance_from_center) + 2;
    				}
    			}
    		}

    	}
    	//imshow("partial",difference);
    	//waitKey(0);
    	compute_matrices(difference, &a, b, &max);
    	difference = Mat::zeros(Size(image_width,image_height), CV_8UC3);
    }

    Mat heat;
    heat_map_computation(a,&heat);
    return heat;
}

void compute_matrices(Mat difference,Mat* a,unsigned long** b,unsigned long* max) {
    unsigned long previous_max = *max;
    unsigned long new_max = previous_max;
    vector<MyPoint> changed;
    vector<MyPoint> unchanged;
    bool max_changed = false;
    int threshold = THRESH;

    //cout << "cols: " << difference.cols << " rows: " << difference.rows << endl;

    //Compute changed pixels and store them in b
    for (int k = 0; k < difference.cols; k++) {
        for (int j = 0; j < difference.rows; j++) {
        	MyPoint myP;
        	myP.p = Point(j,k);
            Vec3b pixel = difference.at<Vec3b>(j,k);
            unsigned short int abs_diff=0;
            //... check difference ...
            for(int l = 0; l < difference.channels(); l++) {
                uchar col = pixel.val[l];
                int converted_value = col;
                if(converted_value>threshold) {
                    //cout << "Found changed pixel: " << j << "," << k <<endl;
                    abs_diff++;
                    break;
                }
            }
            //... if the value of a pixel is above threshold for one channel then it's changed...
            if(abs_diff!=0) {
                //cout << "Ok, found different pixels: changing value in b" << endl;

                //cout << "Changed size to 1: " << changed.size() << endl;
                //cout << "b[i][j] " << b[j][k] << endl;
                //unsigned long intensity = b[j][k]+1;
                unsigned long intensity = b[j][k]+pixel.val[0];
                cout << "New intensity = " << intensity << endl;
                b[j][k]=intensity;
                if(intensity>new_max) {
                //if(!max_changed && intensity>new_max) {
                    new_max = intensity;
                    max_changed = true;
                    //cout << "Max changed: new max = " << new_max << endl;
                }
                myP.new_val = pixel.val[0];
                changed.push_back(myP);
            }
            //... otherwise it hasn't
            else {
                unchanged.push_back(myP);
            }
        }
    }

    //New value for changed pixels
    for (unsigned int i = 0; i<changed.size(); i++) {
        cv::Point p = changed.at(i).p;
        //cout << "Changed pixel: " << p.x << "," << p.y << endl;
        double new_value = double(a->at<double>(p.x,p.y)*((double)previous_max/(double)new_max)+changed.at(i).new_val/((double)new_max));
        a->at<double>(p.x,p.y)=new_value;
    }

    //New value for unchanged pixels
    for (unsigned int i = 0; i<unchanged.size(); i++) {
        cv::Point p = unchanged.at(i).p;
        double new_value = double(a->at<double>(p.x,p.y)*((double)previous_max/(double)new_max));
        a->at<double>(p.x,p.y)=new_value;
    }

    //Update new max value of b
    *max = new_max;
}
