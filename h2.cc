//
// <YOUR NAME>
// Computational Vision Homework 3
// Program 2
//

#include "image.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;



 // @brief Implementation of Task Program 2 to convert to binary image given a threshold.
 //        Same as p1 in Homework 2 -- you can copy the code
 // @param input_filename the name of the input image
 // @param threshold the threshold value
 // @param output_filename the name of the output image
void ConvertToBinary(const string &input_filename, int threshold, const string &output_filename) {
  cout << "Converting to Binary Image" << endl;
  cout << "Input filename: " << input_filename << endl;
  cout << "Threshold: " << threshold << endl;
  cout << "Output filename: " << output_filename << endl;
}

int main(int argc, char **argv){  
  if (argc!=4) {
    printf("Usage: %s input_gray_edge_filename threshold output_binary_edge_filename\n", argv[0]);
    return 0;
  }
  
  const string input_filename(argv[1]);
  const int threshold = atoi(argv[2]);
  const string output_filename(argv[3]);

  ConvertToBinary(input_filename, threshold, output_filename);

  return 0;
}
