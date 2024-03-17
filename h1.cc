//
// <YOUR NAME>
// Computational Vision Homework 3
// Program 1
//

#include "image.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;



 // @brief Implementation of Task Program 1 to produce an edge gray-level image
 //  
 // @param input_filename the name of the input image
 // @param output_filename the name of the gray-level output edge image
void ComputeEdgeImage(const string &input_filename, const string &output_filename) {
  cout << "Producing Edge Image" << endl;
  cout << "Input filename: " << input_filename << endl;
  cout << "Output filename: " << output_filename << endl;
}

int main(int argc, char **argv){  
  if (argc!=3) {
    printf("Usage: %s input_gray_level_file_name output_edge_image_file_name\n", argv[0]);
    return 0;
  }
  
  const string input_filename(argv[1]);
  const string output_filename(argv[2]);

  ComputeEdgeImage(input_filename, output_filename);

  return 0;
}
