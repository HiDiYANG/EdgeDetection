README FILE

Author:   DI YANG
Contact:  di.yang@anu.edu.au
Date:     20/11/2012
================================================================================

Edges_Detection consists of C/C++ code developed for the ICAR research work. 
It contains a C/C++ code library as well as a separate executable for demonstration
purposes.

Edge Detection is based on "viscous force watershed" [1]

This document explains the contents of this directory and provides installation instructions as well as user and developer manuals.

DIRECTORY CONTENTS
==================

This directory contains the following subdirectories.

#1 images
    This directory contains the sample image to test this program.
    P.S. output images would be stored in this directory as well.

#2 src
    This directory contains the source code.
    - main.cpp (Demonstration of active contour)
    - watertest.cpp (Library contains dependent functions for viscous force watershed)
	
#3 tutorial
    This directory contains a video tutorial "Tutotial.mp4"

#4 test_images
    There are five types of images in this folder: 
    "*_res.png", "*_seg.png", "*_ini.png", "*_mask.bmp" and others
     (1) "*_res.png" output image with low visibility contains the individual index of each segmented label.
     (2) "*_seg.png" output image that shows the visualisation of segmentation result. 
     (3) "*_ini.png" output image that records initialization of manual masks.
     (4) "*_mask.bmp" input image with low visibility contains the individual index of each mask.
     (5) other images are input image.

#5 Makefile: to compile demonstration.

#6 The demonstration program needs to be compiled and installed in order to run the examples.


INSTALLATION INSTRUCTIONS FOR UBUNTU
====================================

This was tested on Linux using Ubuntu 12.04 (the latest release as of writing
this document) after installing all updates on 20/11/2012. Ubuntu was obtained
from www.ubuntu.com which provides a free download.

To install, the edge_detection directory needs to be copied over from the FTP to your
hard drive. The rest of this document assumes that the directory
ANU_2_CE/Software/edge_detection along with the subdirectories inside it have been
copied over to your hard drive. We refer to the location on your hard drive as
<edge_dtection path> throughout the remainder of this document. 

A working internet connection is required to install some of the dependent
libraries mentioned below. 

Lines beginning with the $ sign indicate commands that need to be typed and
entered in the command line in a terminal. The $ sign should not be typed. These
commands can be directly copy pasted. A terminal can be opened in Ubuntu by
pressing 'CTRL+ALT+t'.

Proceed as follows to install the dependent libraries and to install losslab.

  1) Install the C/C++ build system.
    $ sudo apt-get install build-essential
  
  2) Install the OpenCV library.
    $ sudo apt-get install libcv2.3 libcv-dev libcvaux-dev libcvaux2.3
    $ sudo apt-get install libhighgui-dev libhighgui2.3 opencv-doc

  3) Now that all dependant libraries have been installed we can proceed to
     build the losslab source code as follows.
    $ cd <edge_detection path>
    $ make



USER MANUAL
===========
Quick run:
#1 run "viscous watershed" in the colour image without predefined masks:
   ./edges test_images/test.png

#2 run "viscous watershed" in the colour image with predefined masks:
   ./edges test_images/photo2.png test_images/photo2_mask.bmp

Tutorial:
A detailed user manual is provided in doc/tutorial.avi which explains the
usage of the edge detection executable along with reference to examples in the examples
directory. Please refer this document first to understand the usage and
functionality of the code.


GUIDE USER INTERFACE
====================

#1 GUI for "viscous watershed" without pre-defined mask.
     *-------------------------------------------------------------------*
     | Hot keys:                                                         |
     | Press 'w' or 'ENTER' - run watershed with viscous force algorithm |
     | Press 'ESC' - exit the program                                    |
     *-------------------------------------------------------------------*
     |  WARNING: No Mask Image Found                                     |
     |  (before running it, roughly mark the areas on the image)         |
     | Press 'r' - resort the original image                             |
     *-------------------------------------------------------------------*

#2 GUI for "viscous watershed" with pre-defined mask.
     *-------------------------------------------------------------------*
     | Hot keys:                                                         |
     | Press 'w' or 'ENTER' - run watershed with viscous force algorithm |
     | Press 'ESC' - exit the program                                    |
     *-------------------------------------------------------------------*


DEVELOPER MANUAL
================

Functionality of "viscous watershed" is addressed as follows:

conduct "viscous watershed" to detect object outline

void VisWatershed( const CvArr* image,
                         CvArr* markers,
                   const int sz,
                   const double C );

Parameters: 
	image   - The input 8-bit 3-channel image.
	markers - The input/output 32-bit single-channel image (map) of markers. It should have the same size as image
	sz      - size of local window (Explanation see [1])
	C       - Parameter C (Explanation see [1]), the value of this parameter is among [0, 10].


REFERENCE 
=========
[1] D. Yang and S. Gould and M. Hutter, "A Noise Tolerant Watershed Transformation with Viscous Force for Seeded Image Segmentation", In Proceeding of Asian Conference of Computer Vision ACCV 2012, Vol.7724, pp. 775-789, 2013.
