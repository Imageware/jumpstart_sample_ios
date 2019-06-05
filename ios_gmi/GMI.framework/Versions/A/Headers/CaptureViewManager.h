//
//  CaptureViewManager.h
//  GMI
//
//  Created by iws03cbrooks1 on 7/18/16.
//  Copyright © 2016 ImageWare Systems Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CaptureViewController.h"
#import "GMIConstant.h"

@interface CaptureViewManager : NSObject

// registers the passed controller for the given capture type. The capture type is unique - so replacing @"passphrase" will cause voice captures to use your custom controller class, instead.
+ (void) registerView:(Class)controller forCaptureType:(CaptureType)type algorithmType:(NSString *) algoType ;

// returns the currently-registered view controller for the passed capture type.
//+ (Class) controllerForCaptureType:(CaptureType)type algorithmType:(NSString *) algoType ;
+ (Class) controllerForCaptureType:(CaptureType)type algorithmType:(NSString *) algoType hasLiveness:(BOOL)liveness;

// returns a list of all of the modalities for which we've registered a view controller
+ (NSArray<NSString *> *) supportedModalities;

@end
