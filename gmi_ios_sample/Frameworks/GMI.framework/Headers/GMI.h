//
//  GMI.h
//  GMI
//
//  Created by ImageWare Systems, Inc on 12/20/18.
//

#import <UIKit/UIKit.h>

FOUNDATION_EXPORT double GMIVersionNumber;

FOUNDATION_EXPORT const unsigned char GMIVersionString[];

#if !TARGET_IPHONE_SIMULATOR
#import <GMI/RRBPalmSDKModelInfo.h>
#import <GMI/RRBPalmSDKUser.h>
#import <GMI/RRBCameraOverlayView.h>
#import <GMI/RRBPalmSDKDecoder.h>
#import <GMI/RRBPalmSDKModelID.h>
#endif
