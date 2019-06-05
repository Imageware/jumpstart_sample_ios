//
//  PalmImageViewController.h
//  CameraTest
//
//  Created by Dana on 7/20/17.
//  Copyright © 2017 com.danadevoe. All rights reserved.
//

#import "CameraViewController.h"
#import "CaptureViewController.h"
#import "SkinManager.h"
#import "LivenessDetection.h"

/*
This object implementeds Palm image caputre for use with the HT technology on the backend server
 */

//SI-269
//typedef NS_ENUM(NSUInteger, HandImageType) {
//    HandImageLeft=PALM_LEFT,HandImageRight=PALM_RIGHT
//};

@class GMIMessage;
@interface PalmImageViewController : CameraViewController<UIAlertViewDelegate, Skinnable, LivenessCaptor>

@end
