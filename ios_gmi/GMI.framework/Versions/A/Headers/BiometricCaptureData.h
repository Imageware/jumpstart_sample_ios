/*************************************************************************
 *
 * IMAGEWARE SYSTEMS, INC. CONFIDENTIAL
 * __________________
 *
 * 2000 - 2015 ImageWare Systems, Inc.
 * All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains the
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
#import "GMIBiometricProbe.h"
#import "GMIPerson.h"

@class CaptureSample;

#define PERSON_NOT_SET -1
#define FILE_DATA_COUNT_MISMATCH -2
#define FILE_OR_MODALITY_IS_NIL -3
#define UNDEFINED_OR_UNSUPPORTED_MODALITY -4

@interface BiometricCaptureData : NSObject

//
@property GMIPerson *person;
@property NSMutableArray *captureSamples;

@property (nonatomic, strong) CaptureType captureType;
/**
 * Creates and returns a new BiometricCaptureData object for the passed person, with the passed data array and modality array.
 *
 * @discussion the data array and modality array go one-for-one, but there's no file data validation, since those are intended to be NSData objects and there would be no way to tell if it's a face or an audio file or whatever.
 *
 * @param person the GMIPerson object to create this data for
 * @param captureSamples a list of CaptureSample objects
 * @return a BiometricCaptureData object or nil if you pass a nil person
 */
+ (BiometricCaptureData *) captureDataForPerson: (GMIPerson *) person withCaptureSampleArray: (NSArray *) captureSamples;

/**
 * Appends the passed CaptureSample object to this response object.
 */
- (void) appendData: (CaptureSample *) data;

//
- (BioType) modality;

//- (CaptureType) captureType;
- (void) cleanUpType:(CaptureType)capType; // use this to clean up any temp capturetype created


@end
