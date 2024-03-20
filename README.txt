Nicklaus Yao
Computational Vision Homework 3

**Thresholds should also be at the file "thresholds.txt"
1. All four parts of the assignment have been completed.
2. Have some problems when set very low thresholds in part 1. 
3. All programs runs as directed in assignment details.
4. Input files same as the ones provided.


----------------------
To compile in Linux:
----------
 
   make all

----------

Program_1 Edge image
    - Filter used is: 3 by 3 Sobel
    - Input: One of the original images like hough_simple_1.pgm
    - Output: An Edge Image created by applying Sobel X and Sobel Y filter 
    - Each pixel is set to the gradient magnitude of the pixel after applying the filters

To run:
---------
./h1 hough_simple_1.pgm edge_image_output.pgm

Program_2 Edge thresholding.
    - The threshold used is: 90
    - The threshold value is stored as the first integer in thresholds.txt
    - Used the same code structure as the previous p1.cc

To run:
---------
./h2 edge_image_output {input int threshold} edge_threshold_output.pgm


Program_3 Computation of the Hough image
    - Input: The Edge Threshold output image from h2
    - Output: Hough Gray Level Image - Bright spots are highlighted as white while background is white
    - Output: Hough Voting Array is a text file consistenting on rho by theta size used


To run:
---------
./h3 edge_threshold_output.pgm hough_gray_level.pgm hough_voting_array.txt
    
Program_4 Line detection
    - The threshold used is: 200
    - Input: Original Image - similar to one used in h1 (ex. hough_simple_1.pgm)
    - Input: hough_voting_array.txt - text file from before in h3
    - Input: input threshold from thresholds.txt - second line integer
    - Output: line_detection.pgm - the final output of drawing lines corresponding to edges on the original image
    - Algo: 
        1. Read in hough_voting_array textfile
        2. Using the hough array, we threshold the space so that anything under the threshold, we will ignore (aka not enough votes)
        3. Sequential Labeling(similar to the last assignment) using 2 by 2 Raster Scan
            - Note: Last Assignment was done with image extension, this time, out of bounds calculations were encoded and accounted for.
        4. After labeling separate objects (aka bright areas), we calculate the weightedX, weightedY, and total Weight in order to determine
            the weighted center of the area. 
        5. Using the center of the bright areas, we will determine two points based on the 4 cases of points 
            that may lie in the M by N image to draw a line on the original image. 

To run:
---------
./h4 hough_simple_1.pgm hough_voting_array.txt {input int threshold} line_detection.pgm