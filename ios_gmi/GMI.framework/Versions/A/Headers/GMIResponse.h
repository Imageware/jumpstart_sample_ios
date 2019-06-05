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
 * Description	: GMIResponse provides a way for the GMI SDK to talk to the parent program via a Protocol. The GMIResponse class contains an NSDictionary object that acts as a dictionary of response data for message, as well as an NSArray of biometric event data that can be accessed and iterated.
 *
 *   This class and its associates make heavy use of the GMIConstants.h class, specifically the constants that start with "gmiresp_". It is highly recommended that implementors also use these constants.
 */

#import <Foundation/Foundation.h>

/**
 *  The GMIResponse is used to communicate from messages to the client app. Whenever the message moves to the next page, or a form is submitted or a verification/enrollment/identification happens, that event triggers an GMIResponse that is passed to the GMIClient's registered response listener.
 */
@interface GMIResponse : NSObject

/**
 *  A dictionary of data for the GMIResponse object.
 */
@property NSMutableDictionary *data;

/**
 *  An NSArray containing each biometric event in the order they occurred.
 */
@property NSMutableArray *biometricEvents;

/**
 *  Puts the passed key into this GMIResponse's dictionary.
 *
 *  @param key the key
 *  @param value the value
 */
- (void) setObject:(NSString*) value forKey: (NSString*) key;

/**
 *  Returns the value for the passed key, or nil if the key doesn't exist.
 *
 *  @param data the key
 *
 *  @return the data for the key, or nil
 */
- (NSString *) objectForKey:(NSString*)key;

/**
 *  Gets the NSMutableDictionary for this GMIResponse.
 *
 *  @return the dictionary for this response
 */
- (NSMutableDictionary *) getDataAsDictionary;

/**
 *  Returns an array of the Biometric Events stored in this GMIResponse. These events are stored as they occur, in the order that they occur; that is, if any of the events are successful it will usually be the last one (since, after that, the message will continue rendering).
 *
 *  The returned array is actually an array of NSMutableDictionaries (although it's recommended you don't edit these dictionaries). Each dictionary is guaranteed to contain the following key/value pairs (each constant is in GMIConstant.h):
 *
 *  - success (gmiresp_VeriWasSuccessful key), an object that is either gmiresp_Yes or gmiresp_No
 *  - type (gmiresp_event_BiometricEventType), an object that is either gmiresp_event_Enrollment or gmiresp_event_Verification
 *  - modality (gmiresp_event_BiometricModality), an object that is either gmiresp_BiometricVoice or gmiresp_BiometricFace
 *
 *  In addition, if there was an error (for example, a failed verification/enrollment error), the error is stored in that dictionary as follows:
 *
 *  - gmiresp_VeriFailError as the key to an object that is an NSString describing the error
 *
 *  @return the array of biometric event dictionaries for this GMIResponse
 */
- (NSMutableArray *) getBiometricEventArray;

@end

typedef GMIResponse IMSResponse __deprecated_msg("IMSResponse is deprecated. Use 'GMIResponse' instead.");
