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
 *
 * Description	: Class used to manage a single biometric capture request. This request can contain any number of modality/sample count pairs.
 */

#import <Foundation/Foundation.h>
#import "GMIConstant.h"
#import "GMIErrors.h"
#import "BiometricCaptureData.h"

#define PROMPT_DATA @"captureSequence"
#define PROMPT_SEPARATOR @","

@class CaptureSample;
@class CaptureTypeList;

@interface BiometricCaptureRequest : NSObject

// whether this is an enrollment, verify or identify
@property BioEventType bioEventType;
@property (readonly) NSMutableArray *livenessArray;
@property NSString *enrollId;

// tenant we'll use for this capture request's response
@property NSString *internalTenant;

// the response data for this request. When this request is satisfied, we'll pass this object into the enroll or verify call in the GMIClient
@property (readonly) BiometricCaptureData *data;

/**
 *  Returns whether this SampleRequest is satisfied - that is, whether it has collected the required number of samples.
 *
 *  @return YES if this SampleRequest has collected the specified number of samples; else NO
 */
- (BOOL) isSatisfied;

/**
 *  @return the number of samples left until this sample request is satisfied. This speficially returns required_samples - sampleData.count
 *  Note that this can be negative, although in practice it won't be since you should never be able to load required_samples + 1 samples.
 */
- (int) remainingSamples;

/**
 *  @return the number of samples required by this request.
 */
- (int) requiredSamples;
- (int) remainingSamplesForModality: (BioType) modality;
- (int) requiredSamplesForModality: (BioType) modality;

/**
 *  Appends the passed file data to this Sample Request.
 *
 *  @param data a CaptureSample object containing a file and some file information. Use the appendDataToSample:forModality: method if you don't want to build a CaptureSample object.
 */
- (void) appendCaptureSample: (CaptureSample *) sample;

/**
 * Removes all of the current capture samples from this capture request
 */
- (void) removeAllCaptureSamples;

/**
 *  Makes a request to the server to enroll or verify, depending on this request. This will call the appropriate methods to verify or enroll, filling out the GMIResponse object as appropriate and performing the passed callback blocks if they're not nil.
 *
 * @discussion this method does an extra step, checking that all of the gateway modalities are satisfied before sending the data to the server - if the gateway modalities are completed and this is only a request for gateways (ie fingerprint-only requests), this will return a success without hitting the server at all.
 *
 * @param success a success block that will be called if this server request succeeds
 * @param failure a failure block that will be called if this server request fails
 */
- (void) performServerRequestSuccess: (void (^)(BOOL, NSString*)) success failure: (void (^)(GMIError *error)) failure;

/**
 * @return the next required sample modality, or nil if there aren't any.
 */
- (BioType) nextSampleModality;

// returns the next required sample CaptureType, if any.
- (CaptureType) nextSampleCaptureType;

/**
 * @return a convenient prompt for the next required sample
 */
- (NSString*) nextSamplePrompt;

/**
 * @return the overall modality of this request. If all of the samples are face, returns FACE_PIC, if all of the samples are voice, returns VOICE_PASS_REC, etc. If it contains a number of different modalities, returns MULTI.
 */
- (BioType) overallModality;

/**
 *  Creates an Enrollment Sample Request for the passed modality and requiring the passed number of samples.
 *
 *  @param modArray the set of modalities for the enrollment (face or voice)
 *  @param order the order of the modalities to ask for
 *  @param tenant the tenant for this request, if different than the one stored in the GMIClient object. If not set, defaults to the GMIClient's current tenant object.
 */
+ (BiometricCaptureRequest *) enrollmentRequestForModalities: (NSArray *) modArray forPerson: (GMIPerson *) person withCaptureTypeTranslator: (CaptureTypeList *) captureTypeList withMessageMetadata: (NSDictionary *) metadata withEnrollId: (NSString *) enrollId;
+ (BiometricCaptureRequest *) enrollmentRequestForModalities: (NSArray *) modArray forPerson: (GMIPerson *) person withCaptureTypeTranslator: (CaptureTypeList *) captureTypeList withMessageMetadata: (NSDictionary *) metadata forTenant: (NSString *) tenant withEnrollId: (NSString *) enrollId;

/**
 *  Creates a Verification Sample Request for the passed modality and requiring the passed number of samples.
 *
 *  @param modArray the set of modalities for the verification (face or voice)
 *  @param order the order of the modalities to ask for
 *  @param tenant the tenant for this request, based on the current message
 */
+ (BiometricCaptureRequest *) verificationRequestForModalities: (NSArray *) modArray forPerson: (GMIPerson *) person withCaptureTypeTranslator: (CaptureTypeList *) captureTypeList withMessageMetadata: (NSDictionary *) metadata;
+ (BiometricCaptureRequest *) verificationRequestForModalities: (NSArray *) modArray forPerson: (GMIPerson *) person withCaptureTypeTranslator: (CaptureTypeList *) captureTypeList withMessageMetadata: (NSDictionary *) metadata forTenant: (NSString *) tenant;

@end

// used by the Request and Data objects to keep our enroll IDs and capture sequences in sync
@interface CompositeCaptureRequest : NSObject

@property NSMutableArray *requiredCaptureSamples;
@property NSMutableArray *capturedSamples;
@property CaptureType captureType;
@property NSUUID *uuid;

- (instancetype) initWithRequiredCaptureSamples: (NSMutableArray *) samples;

- (BioType) nextSampleModality;
- (BOOL) isSatisfied;

@end

