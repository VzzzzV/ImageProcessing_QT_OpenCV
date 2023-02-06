# Readme
__Author : Zhan Zhan__
__Date: 2018 May__

## Image Processing Software
This is a project to implement general image processing operations and two advanced algorithm : Noise Adaptive Fuzzy Switching Median Filter and Unseeded region growing segmentation .

## Dependence
This project is based on Qt5 and OpenCV.

## Implemented Functions
+ Rotation
+ Cropping
+ Color to gray
+ Histogramm
	- Histogramm computation
	- Histogram equalization
+ Binarization
	- Otsu
	- threshold
+ Flitering
	- Mean value filtering
	- Median filtering
	- Gaussian filtering
+ Adding noise
	- Gaussian noise
	- Pepper noise
	- Random noise
+ Edge Detection
	- Roberts operator
	- Prewitt operator
	- Sobel operator
	- Canny operator
+  Quality Assessment
	- PSNR
	- SNR

+ Advanced Algorithm
	- NAFSM filter
	original paper: [Noise Adaptive Fuzzy Switching Median Filter for Salt-and-Pepper Noise Reduction](https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=5356178)
	- Unseeded region growing segmentation
	original paper: [Unseeded region growing for 3D image segmentation](https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=1122a278171a28483ec7b4107acbcb6a5c8d1dae)

## Results
There are some processed images in the ./result fold.