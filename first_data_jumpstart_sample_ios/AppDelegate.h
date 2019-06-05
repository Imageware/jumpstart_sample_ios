//
//  AppDelegate.h
//  first_data_jumpstart_sample_ios
//
//  Created by Henry Chan on 1/4/19.
//  Copyright © 2019 ImageWare Systems, Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SampleGmiViewController.h"
#import <UserNotifications/UserNotifications.h>

//
//  Tablet devices should always support rotation (per Apple guidelines)
//  If your application only supports Phone devices in Portrait, then disable the ROTATE_PHONE definition.
//  The camera capture routines are designed to work only in Portrait, so there is code that forces Portrait
//  before accessing the camera.
//
//#define ROTATE_PHONE  // Define if all device types rotate.


@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) GMIPerson *thisPerson;  // Need to track the current user info
@property (strong, nonatomic) NSArray *bioForPerson;  // of type GMIAlgorithm
@property (strong, nonatomic) GMIMessage *currentMessage;

-(void) checkGmiMessages;  // Sets currentMessage if presenting
-(void) presentGmiMessageId: (NSString *) messageId;  // Presents a specific message 


//
//  === GMI Image capture requires portrait mode display.
//
-(void) fixedOrientation: (UIInterfaceOrientationMask) orientationMask;

@end
