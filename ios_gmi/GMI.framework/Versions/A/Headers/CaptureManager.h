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

#import <UIKit/UIKit.h>
#import "GMIConstant.h"
#import "CaptureDevice.h"
#import "GMIErrors.h"
#import "BiometricCaptureRequest.h"

// the CaptureSample class, used to contain a single file object and its info
@interface CaptureSample: NSObject

// the content of this CaptureSample. Should be an NSData object.
@property NSData *content;
// now this one is for things like Facevoice, where a single capture sample is actually 2+ capture sample data objects (in the face of Facevoice, a face and tehn a voice)
@property NSMutableArray *comboContent;
// the biotype of this content object
@property BioType biotype;
// the mimetype for the capture content object.
@property NSString *mimeType;
@property NSUUID *captureSequenceUUID;
@property NSString *enrollId;
@property NSArray *captureSequence;
@property CaptureType captureType;

@end

// the actual CaptureManager class header
@interface CaptureManager : NSObject

// the currently active device
//@property CaptureDevice *activeDevice;

// our current biometric capture request, retained across calls to startCapture
@property BiometricCaptureRequest *captureRequest;

// retrieves the current singleton instance of this capture manager.
+ (CaptureManager*) sharedInstance;

// initializes the capture manager. Note that the first time you call sharedInstance, this method will be called with nil device info (that's usually what you want anyway).
- (void) initialize: (NSDictionary *) deviceInfo;
- (void) uninitialize;

// methods to activate capture devices by name or biotype.
/*- (void) activateCaptureDeviceWithName: (NSString*) deviceId;
- (void) activateCaptureDeviceForBiotype: (BioType) biotype;*/

/**
 * Starts a capture for the passed BiometricCaptureRequest object. The object contains all of the information the CaptureManager needs to start capturing: the modality to capture, and how many samples to take.
 * 
 * Note that you'll only ever need to use anything in this class if you can't do everything you want to do through our normal messaging structure. Otherwise, it is highly recommended that you simply use our messaging API to perform capture and enrollment/verification.
 *
 * @discussion this is a stand-alone method, so you can call this with a request to capture biometrics. It's recommended that when you call this, you then listen for your view becoming active again (meaning the capture is completed and in a good state to begin another one), then check that the request is satisfied. If the request still has biometrics to capture, calling this method again with the same request will capture the next set of biometrics.
 *
 * @param request the request this capture should fulfill, at least in-part. Each call to this method captures the next required biometric in this request.
 * @param metadata message metadata, not currently used.
 * @param controller the UINavigationController you would like this method to push/pop to.
 */
- (void) startCapture: (BiometricCaptureRequest *) request withNavigationController: (UINavigationController *) controller;
- (void) startCapture: (BiometricCaptureRequest *) request withMetadata: (NSDictionary *) metadata withNavigationController: (UINavigationController *) controller animated: (bool) animated;

@end
