//
//  watertest.h
//  ImpWshed
//
//  Created by Di Yang on 1/03/12.
//  Copyright (c) 2012 The Australian National University. All rights reserved.
//

#ifndef ImpWshed_watertest_h
#define ImpWshed_watertest_h

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

typedef struct CvWSNode
{
	struct CvWSNode* next;
	int mask_ofs;
	int img_ofs;
}
CvWSNode;

typedef struct CvWSQueue
{
	CvWSNode* first;
	CvWSNode* last;
}
CvWSQueue;

static CvWSNode* icvAllocWSNodes( CvMemStorage* storage );

void VisWatershed( const CvArr* srcarr,
                         CvArr* dstarr,
                   const int sz,
                   const double C );

#endif
