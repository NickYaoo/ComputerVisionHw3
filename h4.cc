//
// Nicklaus Yao
// Computational Vision Homework 3
// Program 4 
//

#include "image.h"
#include "DisjSets.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cmath>

using namespace std;
using namespace ComputerVisionProjects;

/**
 * @brief determines if a pixel is in bound during Raster Scan
 * 
 * @param row 
 * @param col 
 * @return true 
 * @return false 
 */
bool inBounds( int row, int col){
  // we do not need to worry about out of bounds to the right (greater than max)
  if (row < 0)
    return false;
  if (col < 0)
    return false;
  return true;
}

// @brief Computes and draws lines from Hough transform
// @param input_filename the filename of the input original image
// @param input_voting_array_filename the filename of the input hough voting array (from h3)
// @param threshold is the threshold for detection of peaks in hough voting array
// @param output_gray_level_line_filename the filename of the output line image
void ComputeAndDrawLinesFromHough(const string &input_filename, const string &input_voting_array_filename, int threshold, const string &output_gray_level_line_filename) {
  cout << "Compute Lines From Hough" << endl;
  cout << "Input original image filename: " << input_filename << endl;
  cout << "input voting array filename : " << input_voting_array_filename << endl;
  cout << "Input threshold: " << threshold << endl;
  cout << "Output image line filename: " << output_gray_level_line_filename << endl;
  
  Image image;
  if (!ReadImage(input_filename, &image)) {
    cout <<"Can't open file " << input_filename << endl;
  }

  // Declare for reuse later
  int rows = image.num_rows();
  int cols = image.num_columns();

  // reading in voting array from text file
  std::ifstream file(input_voting_array_filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << input_voting_array_filename << std::endl;
    }

    // Stores the hough array from h3
    std::vector<std::vector<int>> voting_array;

    // typical sStream structure for reading in lines and each value
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::istringstream iss(line);
        int value;
        while (iss >> value) {
            row.push_back(value);
        }
        voting_array.push_back(row);
    }


    // Based on the voting array we threshold: 0 make into binary image
    for (int x = 0; x < voting_array.size(); x++) {
        for (int y = 0; y < voting_array[0].size(); y++) {
            // we ignore those that are less than the threshold
            if (voting_array[x][y] < threshold) {
                voting_array[x][y] = 0;
            }
            // we can keep the voting values if they are above the threshold
        }
    }

  // After Binary Image: Run Sequential Labeling to make into multiple objects
  // Use the code from the Previous Assignment
  int acc_rows = voting_array.size();
  int acc_cols = voting_array[0].size();

  // create a copy of the accumulator array - we need to keep voting counts for later
  vector<vector<int>> scan_array(voting_array);

  DisjSets dSets(acc_rows * acc_cols);

  // Code structure from p2 of Assignment 2
  int label_counter = 0; //running count
  // First Pass
  for(int i = 0; i < acc_rows; i++){
    for(int j = 0; j < acc_cols; j++){
      // check if this image is part of an bright area, all bright areas above threshold are nonzero
      if (scan_array[i][j] > 0){
        // initalize early because of reuse
        // if out of bounds consider it to be 0
        int b=0 , c=0, d = 0;
        if (inBounds(i, j - 1)){
          b = scan_array[i][j];}
        if (inBounds(i-1, j)){
          c = scan_array[i][j];}
        if (inBounds(i-1, j - 1)){
          d = scan_array[i][j];}
        
        // Check for "d"
        if(d != 0){
          scan_array[i][j]=d;
        }
        // check if [0 0]
        //          [c a]
        else if(b == 0 && c != 0){
          scan_array[i][j]=c;
        }
         // check if [0 b]
        //           [0 a]
        else if(c == 0 && b!=0){
          scan_array[i][j]=b;
        }
        // check if c and b are not equal case - we take the minimum label of the two - solve equivalence class later
        else if(b != 0 && c!= 0){
          if(dSets.find(c) != dSets.find(b)){
            // equivalence case
            dSets.unionSets(dSets.find(c), dSets.find(b));
          }
          scan_array[i][j] = c;
        }
        else{
          // case of all d == c == b == 0 ==> New Object
          label_counter += 1;
          scan_array[i][j] = label_counter;
        }
      }
    }
  }


  // 2 by 2 Raster Scan Pass 2 - Used from Assignment 2
  for(int i = 0; i < acc_rows; i++){
    for(int j = 0; j < acc_cols; j++){
      // Store the current label
      int temp = dSets.find(scan_array[i][j]);
      if(temp != 0){
        scan_array[i][j] = temp;
      }
    }
  }

    // use this dictionary label -> [weightedX, weightedY, totalWeight, centroidX, centroidY ]
    unordered_map<int, vector<double>> dict;
    // Find the center of brightness - weighted average of each object
    for(int i = 0; i < acc_rows; i++){
      for(int j = 0; j < acc_cols; j++){
        int voting = voting_array[i][j];
        int label = scan_array[i][j];
        if(label != 0){
          // check if this object has been seen in the dictionary, if not add to it
         if(dict.count(label) == 0){
            dict[label] = vector<double>(5);
          }
          // calculate weightedX
          dict[label][0] += (i * voting);
          // calculate weightedY
          dict[label][1] += (j * voting);
          // calculate sum of weights
          dict[label][2] += (voting);
          }
      }
    }

    // UPDATE STEP: COMPUTE THE CENTROID using weighted Sum/ total weights
    // Use iterators to loop through all keys of the dictionary
    for(auto& f : dict){
      const int& key = f.first;
      // Update x-center, y-center
      dict[key][3] = dict[key][0] / dict[key][2];
      dict[key][4] = dict[key][1] / dict[key][2];
      // cout << key << " " << dict[key][3] << " " << dict[key][4] << endl;
    }

    // Center: pho, theta ==> use this to find the points and draw a line
    // Given rho and theta, find the points and draw the line
    // Go through all 4 possibilites to find the 2 points - MAKE SURE THETA IS IN RADIANS
    for(auto& f : dict){
      const int& key = f.first;
      vector<double> linePoints;
      double rho = dict[key][4];
      double theta = dict[key][3] * M_PI/180.0;

      // case 1:
      double x1 = 0.0;
      double y1 = rho/ sin(theta);
    if(y1 >= 0 && y1 < cols){
      // we can use x1 and y1 as a point
      linePoints.push_back(x1);
      linePoints.push_back(y1);
    }

      // case 2:
    double y2 = 0;
    double x2 = rho/cos(theta);
    if(x2 >= 0 && x2 < rows){
      linePoints.push_back(x2);
      linePoints.push_back(y2);
    }

    // case 3:
    double x3 = rows - 1;
    double y3 = (rho - x3*cos(theta)) / sin(theta);
    if(y3 >= 0 && y3 < cols){
      // we can use x1 and y1 as a point
      linePoints.push_back(x3);
      linePoints.push_back(y3);
    }

    // case 4:
    int y4 = cols -1;
    int x4 = (rho-y4*sin(theta)) / cos(theta);
    if(x4 >= 0 && x4 < rows){
      linePoints.push_back(x4);
      linePoints.push_back(y4);
    }
    // Draw white line for each bright area
    DrawLine(int(linePoints[0]), int(linePoints[1]), int(linePoints[2]), int(linePoints[3]), 255, &image);
}

  if (!WriteImage(output_gray_level_line_filename, image)){
    cout << "Can't write to file " << output_gray_level_line_filename << endl;
  }

}

int main(int argc, char **argv){
  
  if (argc!=5) {
    printf("Usage: %s {input original gray-level image} {input hough voting array} {input hough threshold value} {output gray-level line image}\n", argv[0]);
    return 0;
  }
  
  const string input_filename(argv[1]);
  const string input_voting_array_filename(argv[2]);
  const int threshold = atoi(argv[3]);
  const string output_gray_level_line_filename(argv[4]);

  ComputeAndDrawLinesFromHough(input_filename, input_voting_array_filename, threshold, output_gray_level_line_filename);
  
  return 0;
}
