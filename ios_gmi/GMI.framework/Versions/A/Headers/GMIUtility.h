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
 * Description	: Utility class for getting the UUID of the device.
 */

#import <Foundation/Foundation.h>
#import "GMIConstant.h"
#import "GMIServer.h"
#import "GMIErrors.h"

@interface GMIUtility : NSObject

/**
 *  Gets this device's UUID
 *
 *  @return The UUID for this Device
 */
+ (NSString *) getUUID;
+ (NSString *) platformString;

/**
 * Installs the passed UUID in this app's keychain. Subsequent calls to getUUID will return this keychain's UUID, even after the app is killed and uninstalled/reinstalled.
    Parameter lists including the identifier will let you store the keychain UUID with an arbitrary identifier.
 */
+ (void) installKeychainUUID: (NSString *) uuid;
+ (void) installKeychainUUID: (NSString *) uuid withIdentifier: (NSString *) identifier;
+ (void) uninstallKeychainUUID;
+ (void) uninstallKeychainUUIDForIdentifier: (NSString *) identifier;

/**
 *  Returns the String form of the passed device token.
 * 
 *  @param deviceToken an APN token created on a successful registration for push
 *
 *  @return a String form of the passed device token
 */
+ (NSString*) stringFromDeviceToken: (NSData*) deviceToken;

/**
 *  Generates and returns a random UUID. 
 *
 *  @return a random UUID using the iOS 6+ NSUUID class.
 */
+ (NSString*) generateRandomUUID;

/**
 * 
 */
+ (NSString*) jsonStringFromDictionary: (NSDictionary*) dict withError: (NSError**) error;

/**
 *  Returns whether the passed String is a valid email address.
 *
 *  @param checkString the address to check
 *
 *  @return YES if the email is a valid email (of the form ***@***.***), else NO
 */
+ (BOOL) NSStringIsValidEmail:(NSString *)checkString;

/**
 *  Creates an error for an HTTP request with the passed status code (for example 404) and the status from the response object (for example "File Not Found").
 *
 *  @param statusCode the code for the error
 *  @param status     the description string for the error
 *
 *  @return a GMIError with the passed status code and description set
 */
+ (GMIError*) makeErrorForCode: (NSInteger)statusCode withDescription: (NSString*) status;
+ (GMIError*) makeErrorForCode: (NSInteger)statusCode withDomain: (NSString*) domain withDescription: (NSString*) status;
+ (int) biotypeForTenant: (NSString *) ten withCapture: (NSString *) cap ;
/**
 *  Returns an extension for the passed bioType.
 *
 *  @param mod the modality to check
 *  @return the file extension for files of that modality
 */
+ (NSString*) extensionForBioType: (BioType) mod;

/**
 *  Returns the mime type for the passed BioType.
 *
 *  @param the BioType to check
 *  @return the mime type for the passed modality
 */
+ (NSString*) mimeForBioType: (BioType) mod;

/**
 *  Returns the human-readable String version of the passed BioType.
 */
+ (CaptureType) captureTypeForBioType: (BioType) arg;

/**
 *  Returns the BioType that is the integer version of the passed modality, or -1 if none exist.
 */
+ (BioType) bioTypeForCaptureType: (CaptureType) arg;

/**
 *  Returns the main bundle of the SDK, where all of the xibs and SDK resources live.
 */
+ (NSBundle *) getSDKResourceBundle;

@end

typedef GMIUtility IMSUtility __deprecated_msg("IMSUtility is deprecated. Please use GMIUtility instead.");
