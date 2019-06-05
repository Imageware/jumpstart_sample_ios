/************************************************************************
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
 *
 * Description	: The GMIMessage class represents a single message from the GMI servers, composed of a dictionary of metadata and a single URL that begins message flow when visited.
 */

#import <Foundation/Foundation.h>
#import "GMIConstant.h"

#define GMI_MESSAGEID @"messageId"
#define GMI_METADATA @"metadata"
#define GMI_EXTRAS @"extras"

#define GMI_APPLICATION_CODE @"applicationCode"
#define GMI_TENANT_CODE @"tenantCode"
#define GMI_POSTBACK_URL @"postbackUrl"
#define GMI_TEMPLATE @"template"
#define GMI_HTML @"html"
#define GMI_TEMPLATE_TYPE @"type"
#define GMI_URL @"url"
#define GMI_BODY @"body"
#define GMI_PERSON @"personId"
#define GMI_OPTIONAL @"optional"
#define GMI_REQUIRED @"required"
#define GMI_LIVENESS_REQS @"liveness"
#define GMI_CAPTURE_TYPE_LIST @"captureTypeList"

// special enrollment keys that only exist in the metadata in an enrollment message, inside the enrollData dictionary
#define GMI_ENROLL_DATA @"enrollData"
#define GMI_ENROLL_CAPTURE_TYPE @"captureType"
#define GMI_ENROLL_SAMPLES @"enrollSamples"
#define GMI_ENROLL_ID @"enrollId"

// special key in the metadata of a non-enrollment message
#define CAPTURE_TYPE_LIST @"captureTypeList"
@class GMIPerson;

// an internal interface for translating capture types, like "passphrase", into biometrics like "[voice, voice, voice]"
@interface CaptureTypeList : NSObject

@property NSMutableDictionary *sampleDict;

- (instancetype) initWithSampleDict: (NSDictionary *) captureList;

- (instancetype) copyWithZone:(NSZone *)zone;
- (instancetype) mutableCopyWithZone:(NSZone *)zone;

- (void) addCompositeModality: (NSString *) mod withModList: (NSArray *) list;

- (NSArray *) modalitiesForCaptureType: (NSString *) mod;

- (BOOL) containsCaptureType:(NSString *)captureType;

@end

// 
@interface GMIMessage : NSObject

/**
 * The name of this Message's template.
 */
@property NSString *templateName;

/**
 * An NSDictionary containing all of this message's metadata.
 */
@property NSDictionary *metadata;

/**
 * The URL to this message's first page, if it's an HTML message.
 */
@property NSString *url;

/**
 * The message's UUID
 */
@property(nonatomic,strong) NSString *messageId;

/**
 * The GMIPerson this message is targeted to. Note that this object is only guaranteed to have the person UUID; the person object may not contain any enrollment or verification data for the user, or the user's userId.
 */
@property(nonatomic,strong) GMIPerson *person;

/**
 * The tenant for this message.
 */
@property(nonatomic,strong) NSString *tenant;


/**
 * Additional key-value pairs in the message but that aren't necessarily hard-coded and aren't metadata. Things like templateWithoutApp, tenantCode, etc.
 */
@property NSMutableDictionary *extras;

//
@property NSString *templateType;

//
@property NSString *html;

// whether or not this is a direct message
@property BOOL isDirect;

// the metadata fields for
@property (nonatomic, getter=getOptionalCaptureTypes) NSArray *optional;
@property (nonatomic, getter=getRequiredCaptureTypes) NSArray *required;
@property (nonatomic, getter=getLivenessRequirements) NSArray *livenessRequirements;

@property (nonatomic, strong) NSArray *palmMessages;

// capture type definitions for this message
@property CaptureTypeList *captureTypeList;

- (instancetype) copyWithZone:(NSZone *)zone;
- (instancetype) mutableCopyWithZone:(NSZone *)zone;

/**
 * Initializes a GMIMessage with the passed dictionary. All appropriate fields are populated and, if appropriate, the message XML is parsed.
 *
 *  It's entirely possible for the method to populate the given error when it's parsing XML - should never happen, but could. In this case this call will return all fine and dandy, except the error will be set and more than likely the parsing will have failed, so the xmlTemplate member will not be populated. This is an XML issue and you'll have to fix your XML templates to fix this problem.
 */
- (instancetype) initWithDictionary: (NSDictionary*) dict withError: (NSError**) error;

/**
 * Convenience method for storing in your own database or whatever. Essentially packs this GMIMessage into a nice little JSON object for quick transmission.
 */
- (NSString*) toJSONString: (NSError**) error;

// special enrollment calls
- (bool) isEnrollmentMessage;
- (NSDictionary *) getEnrollmentMetadata;

// Indicates if the currently set modality is valid
- (bool) isValidModality;

// returns whether or not we support all of the modalities the message is asking for
// for iOS this basically just checks that we support fingerprint if the message asks for fingerprint
- (bool) canCaptureAllRequiredTypes;

//
- (void) setOptional:(NSArray *)optional;
- (void) setRequired:(NSArray *)required;

- (NSInteger) getBioType;
- (NSString *)captureType;
- (GMIMessage *)clone;
- (NSInteger) enrollID;

@end
