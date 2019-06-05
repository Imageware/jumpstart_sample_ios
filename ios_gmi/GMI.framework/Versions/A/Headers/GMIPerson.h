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
 *
 * Description	:
 */

#import "GMIConstant.h"
#import "GMIAlgorithm.h"

#define GMI_PERSON_DATA @"data"
#define GMI_PERSON_TENANTDATA @"tenantData"
#define GMI_PERSON_ID @"id"
#define GMI_PERSON_USERID @"userId"
#define GMI_PERSON_VOICE_DIGITS @"voiceDigits"

@interface GMIPerson : NSObject

@property (readonly) NSString* uuid;
@property (readonly) NSString *userId;
@property (readonly) NSDictionary *biometricData;
@property (readonly) NSDictionary *tenantData;
@property (readonly) NSString *voiceDigits;
@property (nonatomic,strong) NSArray<NSString *> *algorithmList ;
@property (nonatomic,strong) NSArray<GMIAlgorithm *> *algorithms;

- (instancetype) copyWithZone:(NSZone *)zone;
- (instancetype) mutableCopyWithZone:(NSZone *)zone;

/**
 *  Initializes a GMIPerson with the passed person UUID.
 *
 *  @param peruuid the UUID of this person
 *
 *  @return init
 */
- (instancetype) initWithUUID: (NSString*) peruuid;

/**
 *  Initializes a GMIPerson with the passed dictionary.
 *
 *  @param peruuid the UUID of this person
 *
 *  @return the person
 */
- (instancetype) initWithDictionary: (NSDictionary*) dict;

/**
 *  Returns whether the passed GMIPerson is equal to this person. Two GMIPerson objects are equal if they contain the same data (uuid).
 *
 *  @param person the person to test against
 *
 *  @return true if the passed person is equal to (has the same data as) this Person
 */
- (bool) isEqualToPerson: (GMIPerson*) person;

/**
 *  Returns a JSON version of the GMIPerson's description method. This is a convenience method for the JSBridge but can be handy for other weird things.
 *
 *  Note that this method takes no NSError param, so make sure you check the returned value isn't nil or blank.
 *  Also note that you get back a data object. You'll need to use the NSJSONSerialization classes to get something human-reabable from this.
 */
- (NSData*) getJSONData;

/**
 * Returns a dictionary of this person's data, or nil if there's a problem retrieving the data.
 */
- (NSDictionary*) getJSONDictionary;

/**
 *  Returns whether or not this person is currently enrolled for the ImageWare (shared) tenant, under any of the current modalities.
 */
- (bool) isEnrolled;

/**
 *  Returns whether or not this person is currently enrolled with the passed modality. Use the #define'd PERSON_MODALITY_x constants for this method.
 */
- (bool) isEnrolledForModality: (BioType) modality;

/**
 * Returns all of the biotypes avalaible to this user
 */
- (NSArray<NSDictionary *> *)bioTypes;

/**
 * Returns the latest biotype for palm
 */
- (BioType) palmBioType;

/**
 This method loads the algorithm for this particular person with UUID
 The functions below will only work correctly after this method has been called successfully.
 @params
 completion: A completion block which is called with a boolean indicating success or failure.
            upon success the member array of Algorithims will contain an array of GMIAlgorithm
 */
- (void) loadAlgorithms:( void (^)(BOOL)) completion ;

/**
 * Returns the array of liveness strings for a given capture type
 * @params
 * captureType: The captureType value used to compare to the internal list
 * return: Returns a list of liveness strigns, this method will return nil if loadAlgorthms has not been called before this call
 */
- (NSArray<NSString *>*) getLiveness:(NSString *)captureType ;

/*
 * Returns the algorithm code for a particular capturetype as specified by the server
 * @params
 * captureType: The capture type string such as "face", "pin", etc
 * return: The cooresponding Algorithm String, this method will return nil if loadAlgorthms has not been called before this call
 */
- (NSString *) algorithmFor:(NSString *)captureType;

/*
 * Returns the capturetype string from the bioType as an INT
 * @params
 * bioType: The biotype value to be translated, the translation only occure if loadAlgorithm was called and the server provided the values
 * return: The captureType string,  this method will return nil if loadAlgorthms has not been called before this call
 */
- (NSString *) captureTypeFor:(int)bioType;


@end
