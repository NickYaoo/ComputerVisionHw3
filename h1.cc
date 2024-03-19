//
// Nicklaus Yao
// Computational Vision Homework 3
// Program 1
//

#include "image.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;
using namespace ComputerVisionProjects;

 // @brief Implementation of Task Program 1 to produce an edge gray-level image
 //  
 // @param input_filename the name of the input image
 // @param output_filename the name of the gray-level output edge image
void ComputeEdgeImage(const string &input_filename, const string &output_filename) {
  Image image;
  if (!ReadImage(input_filename, &image)) {
    cout <<"Can't open file " << input_filename << endl;
    return;
  }
  // We will change it later
  Image edgeImage = image;

  // get the size of the image
  int rows = image.num_rows();
  int cols = image.num_columns();

  // Create Sobel filters/kernels

  // X_gradient Sobel Array
  int XSobel[3][3]= {{-1, 0, 1},
                     {-2, 0, 2},
                     {-1, 0, 1}};

  int YSobel[3][3]= {{1, 2, 1},
                     {0, 0, 0},
                     {-1, -2, -1}};

  // NOTE:
  // [kernelX-1, kernelY-1]  | [kernelX-1, kernelY] | [kernelX-1, kernelY+1]
  // [kernelX, kernelY-1]    | [kernelX, kernelY]   | [kernelX, kernelY+1]
  // [kernelX+1, kernelY-1]  | [kernelX+1, kernelY] | [kernelX+1, kernelY+1]


  // Look to Apply the Filter in X and Y direction
  // We can combine to save some space as every pixel needs to be done with X and Y filter anyways
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      int gradientX = 0;
      int gradientY= 0;

      // Apply Filter
      for(int kernelX= -1; kernelX < 2; kernelX++){
        for(int kernelY = -1; kernelY < 2; kernelY++){
          // check for inbounds
          int pixelI = i + kernelX;
          int pixelJ = j + kernelY;
          if((pixelI >= 0 && pixelJ >= 0) && (pixelI < rows && pixelJ < cols)){
            // we use kernelX+1, kernelY+1 to fix indexing issue as kernelX and kernelY start at -1
            gradientX += image.GetPixel(pixelI, pixelJ) * XSobel[kernelX+1][kernelY+1];
            gradientY += image.GetPixel(pixelI, pixelJ) * YSobel[kernelX+1][kernelY+1];
          }
        }
      }

      // Compute the Gradient Magnitude
      double gMagnitude = sqrt(pow(gradientX,2) + pow(gradientY,2));
      // Set the Pixel to the new color
      edgeImage.SetPixel(i,j,gMagnitude);
    }
  }

  if (!WriteImage(output_filename, edgeImage)){
    cout << "Can't write to file " << output_filename << endl;
  }

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
