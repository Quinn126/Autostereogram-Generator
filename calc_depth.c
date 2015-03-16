/*
 * PROJ1-1: YOUR TASK A CODE HERE
 *
 * Feel free to define additional helper functions.
 */

#include "calc_depth.h"
#include "utils.h"
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* Implements the normalized displacement function */
unsigned char normalized_displacement(int dx, int dy, int maximum_displacement) {
    double squared_displacement = dx * dx + dy * dy;
    double normalized_displacement = round(255 * sqrt(squared_displacement) / sqrt(2 * maximum_displacement * maximum_displacement));
    return (unsigned char) normalized_displacement;

}

/* This function tells us the index when we input coordinate and size.*/
int index(int w, int h, int width){
	int index;
	index = h * width + w;
	return index;
}

 /* when we are given the center pixel of a feature, this function tells us what the indexies of the feature's pixels are.*/
int * featureindex(int x, int y, int feature_width, int feature_height, int width){
    int *featureindex = NULL;
    int indexcounter = 0;
    for(int line = 0; line < (2 * feature_height + 1); line++){
        for(int row = 0; row < ( 2 * feature_width + 1); row++){
        featureindex[indexcounter] = index(x - feature_width + line, y - feature_height + row, width);
        indexcounter++;
        }
    }
    return featureindex;
}

void calc_depth(unsigned char *depth_map, unsigned char *left,
        unsigned char *right, int image_width, int image_height,
        int feature_width, int feature_height, int maximum_displacement) {
 
    /* YOUR CODE HERE */
    /* When maximum_displacement is 0, the whole image would have a normalized displacement of 0. */ 
   if(maximum_displacement == 0){
        int size = image_width * image_height;
        for(int i = 1; i < size; i ++){
            *depth_map = 0;
            i++;
            depth_map++;
        }
            return;
    }
  
    for(int h = 0; h<image_height; h++){
    	for(int w = 0; w<image_width; w++){
    		int currentindex = index(w, h, image_width);
            int size = (2 * feature_height + 1) * ( 2 * feature_width + 1);
            /* Pixels on the edge of the image, whose left-image features don't fit inside the image, should have a distance of 0. */
    		if(((h - feature_height) < 0) || ((h + feature_height) > (image_height - 1)) || ((w - feature_width)< 0) || ((w + feature_width)> (image_width - 1))){
    			depth_map[currentindex] = 0;
                continue;
    		}
            /* Else the left feature exists.*/
            int minimum_squaredeuclidean = 255 * 255 * size;
            int minimum_x;
            int minimum_y;
            /* For those pixels whose features lies inside the searching space. So: We are searching over the center of features.*/
            for(int spaceh = (h - maximum_displacement + feature_height); spaceh <= (h + maximum_displacement - feature_height); spaceh++){
                for(int spacew = (w - maximum_displacement + feature_width); spacew <= (w + maximum_displacement - feature_width); spacew++){
                /* We first need to make sure thier features are not partially outside image boundary.*/
                if(((spaceh - feature_height) < 0) || ((spaceh + feature_height) > (image_height - 1)) || ((spacew - feature_width)< 0) || ((spacew + feature_width)> (image_width - 1))){
                    continue;
                    }
                /* Else the right feature should be counted.*/
                else{
                /* Now we have the center of two pixels.*/                    
                    int *leftfeature = featureindex(w, h, feature_width, feature_height, image_width);
                    int *rightfeature = featureindex(spacew, spaceh, feature_width, feature_height, image_width);
                    int temp_squaredeuclidean = 0;                    
                    for(int i = 0; i < size; i++){
                     temp_squaredeuclidean = temp_squaredeuclidean + (leftfeature[i] - rightfeature[i]) * (leftfeature[i] - rightfeature[i]);
                    }
                    if (temp_squaredeuclidean < minimum_squaredeuclidean){
                        minimum_squaredeuclidean = temp_squaredeuclidean;
                        minimum_x = spacew;
                        minimum_y = spaceh;
                        }
                    }
                }
            }
            int dx = abs(w - minimum_x);
            int dy = abs(h - minimum_y);
            depth_map[currentindex] = normalized_displacement(dx, dy, maximum_displacement);
    	}
    }
    return;

}













