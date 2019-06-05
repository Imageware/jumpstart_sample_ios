//
//  CaptureViewController.h
//  GMI
//
//  Created by iws03cbrooks1 on 5/2/16.
//  Copyright © 2016 ImageWare Systems Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GMIConstant.h"

@class CaptureDevice;

@interface CaptureViewController : UIViewController

// the device currently being used by this capture preview
@property CaptureDevice *currentDevice;

// the metadata for our current capture
@property NSDictionary *metadata;

// returns whether or not we have permission to capture with this controller - for example, if microphone permission is denied, or if we have no fingerprints.
// overriders should note that the returned GMIPermissionStatus must be GMIPermissionStatusOK for success, or else a different status that will be the exact status returned to the template.
- (GMIPermissionStatus) getCapturePermissionStatus;

//
// Performs a back by poping all of the CaptureViewController from the navigational stack.
//
- (void) leave;

//
//
//
- (void) playBeep;

@end
