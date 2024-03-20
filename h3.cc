//
// Nicklaus Yao
// Computational Vision Homework 3
// Program 3
//

#include "image.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;
using namespace ComputerVisionProjects;

// @brief Computes Hough Transform
// @param input_filename the filename of the input binary edge image
// @param output_hough_image_filename the name of the output hough image (gray scale pgm)
// @param output_voting_array_filename the name of the output hough voting array
void ComputeHoughTransform(const string &input_filename, const string & output_hough_image_filename, const string &output_voting_array_filename) {
  cout << "Compute Hough Transform" << endl;
  cout << "Input labeled image filename: " << input_filename << endl;
  cout << "Output hough image filename: " <<  output_hough_image_filename << endl;
  cout << "Output hough voting array filename: " << output_voting_array_filename << endl;

  Image image;
  if (!ReadImage(input_filename, &image)) {
    cout <<"Can't open file " << input_filename << endl;
  }

  // Initalize Parameters
  int rows = image.num_rows();
  int cols= image.num_columns();

  // the sigmas/stepping function
  double theta_res = 1.0; // in degrees ==> M_PI / 180
  double rho_res = 0.5;
  double max_rho = sqrt(pow(rows,2) + pow(cols,2)); // sqrt( M^2 + N^2)

  // Accumulator Array [1...R, 1... T]
  int rho_bins = max_rho / rho_res;
  int theta_bins = 180/ theta_res;
  vector<vector<int>> hough_acc(theta_bins, vector<int>(rho_bins,0));

  // Hough Transform
  int max_voting = 0;
  for(int i = 0; i< rows; i++){
    for(int j = 0; j < cols; j++){
      // check if this is labeled an Edge aka 255
      if(image.GetPixel(i,j) == 255){
        for(int h = 0; h < theta_bins; h++){
          // similar to formula on slides, but our h starts at 0, not 1
          double theta_h = (h) * M_PI / theta_bins;
          double rho = i * cos(theta_h) + j * sin(theta_h);

          // if rho = 5.3, k = 5
          // initially wanted to use just rho, but try to "normalize" based on factors that influence image size
          int k = static_cast<int> (rho*rho_res / max_rho * rho_bins);
          if(k >= 0 && k < rho_bins){
            hough_acc[h][k]++;
            max_voting = max(hough_acc[h][k], max_voting);
          }
        }
      }
    }
  }

  // Write to textfile
  std::ofstream voting_array_file(output_voting_array_filename);
  for (int i = 0; i < theta_bins; i++) {
      for (int j = 0; j < rho_bins; j++) {
          voting_array_file << hough_acc[i][j] << " ";
      }
      voting_array_file << std::endl;
   }
  voting_array_file.close();

  // Create output image
  Image output;
  output.AllocateSpaceAndSetSize(theta_bins, rho_bins);
  output.SetNumberGrayLevels(255);

  for (int i = 0; i < theta_bins; i++) {
    for (int j = 0; j < rho_bins; j++) {
      // make sure we do not have any undefined behavior when voting count is above 255
      if(hough_acc[i][j] >= 255){
        output.SetPixel(i, j, 255);
      }
      else{
        output.SetPixel(i, j, hough_acc[i][j]);
      }
    }
  }

  if (!WriteImage(output_hough_image_filename, output)){
    cout << "Can't write to file " << output_hough_image_filename << endl;
  }
}


int main(int argc, char **argv){
  
  if (argc!=4) {
    printf("Usage: %s input_binary_edge_filename output_gray_legel_hough_image_filename output_hough_voting_array(txt)\n", argv[0]);
    return 0;
  }
  
  const string input_filename(argv[1]);
  const string output_hough_image_filename(argv[2]);
  const string output_voting_array_filename(argv[3]);

  ComputeHoughTransform(input_filename, output_hough_image_filename, output_voting_array_filename);
  
  return 0;
}
