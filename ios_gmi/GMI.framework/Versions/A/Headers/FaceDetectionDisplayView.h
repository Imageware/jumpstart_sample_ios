//
//  FaceDetectionDisplayView.h
//  GMI
//
//  Created by iws03cbrooks1 on 7/20/16.
//  Copyright © 2016 ImageWare Systems Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface FaceDetectionDisplayView : UIView

// the rectangle where we'll draw the face
@property CGRect faceRect;

// whether or not we should draw a face outline at all
@property BOOL drawFace;

@end
