/*************************************************************************
 *
 * IMAGEWARE SYSTEMS, INC. CONFIDENTIAL
 * __________________
 *
 *  2000 - 2015 ImageWare Systems, Inc.
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains the
 * property of ImageWare Systems Inc. and its suppliers, if any.
 * The intellectual and technical concepts contained herein are
 * proprietary to ImageWare Systems Inc. and its suppliers and may
 * be covered by U.S. and Foreign Patents, patents in process, and
 * are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from ImageWare Systems Inc.
 *
 * Author(s)	: cbrooks
 */

#import <Foundation/Foundation.h>
#import "GMIConstant.h"

@class CaptureViewController;
@class CaptureSample;

@interface CaptureDevice : NSObject 

- (void) initializeDevice;
- (void) uninitialize;
- (NSString *) getId;
- (CaptureDeviceType) deviceType;
- (NSString *) version;
- (NSString *) vendor;

@end

// for devices like cameras and palm scanners, which prep for a single capture and then perform that capture
@protocol SingleCapturer <NSObject>

//
- (int) prepForCapture;

//
- (void) performCapture: (void (^)(CaptureSample *sample)) callback;

@end

@protocol StreamCapturer <NSObject>

// the returned value should be a status
- (int) startCaptureStream;

// the returned value should be a status
- (int) stopCaptureStream;

//
- (void) getCompletedCaptureSample: (void (^)(CaptureSample *sample)) callback;

@end
