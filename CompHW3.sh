#!/bin/bash

make all
./h1 InputImagesHomework3/hough_simple_1.pgm output.pgm
./h2 output.pgm 40 output2.pgm
./h3 output2.pgm output3.pgm a.txt
./h4 InputImagesHomework3/hough_simple_1.pgm a.txt 150 output4.pgm


./h1 InputImagesHomework3/hough_simple_2.pgm 2output.pgm
./h2 2output.pgm 40 2output2.pgm
./h3 2output2.pgm 2output3.pgm b.txt
./h4 InputImagesHomework3/hough_simple_2.pgm b.txt 120 2output4.pgm