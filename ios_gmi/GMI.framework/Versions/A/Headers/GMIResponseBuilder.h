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
 * Description	: GMIResponseBuilder is used as a shared utility to create GMIResponse objects.
 */

#import "GMIMessage.h"
#import "GMIConstant.h"
#import "GMIResponse.h"

@interface GMIResponseBuilder : NSObject

/**
 *  Returns a shared instance of the GMIResponseBuilder.
 *
 *  @return a shared instance of the GMIResponseBuilder.
 */
+ (instancetype) sharedInstance;

/**
 *  Delivers the current, built GMIResponse object (or nil if none was built) to the GMIResponseListener assigned in GMIClient.
 */
- (void) deliver;

/**
 *  Appends the passed (key, value) pair to the existing GMIResponse. If none exists, creates one before appending.
 *
 *  @param value
 *  @param key   
 */
- (void) appendToMessage: (NSString*)value forKey: (NSString*)key;

/**
 *  Returns whether or not this builder has started a message yet.
 *
 *  When a message is started, it appends its ID to this instance. In that way we can use this method to check whether a verification/enrollment is stand-alone or inside of a message.
 *
 *  @return
 */
- (BOOL) hasMessagePrepped;

/**
 *  Abandons this message, setting the GMIResponse event type as MESSAGE_ABANDONED.
 */
- (void) setAsAbandonedMessage;

/**
 *  Creates an GMIResponse prepped for the rendering of a message.
 */
- (void) prepMessageRenderResponse: (GMIMessage*) message;

/**
 *  Appends the passed biometric event to this GMIResponse.
 *
 *  @param type either "Enrollment" or "Verification" (use gmiresp constants)
 *  @param modality the modality for this event
 *  @param error the error, if any occurred. This can also be a blank string.
 */
- (void) appendFailedBiometricEvent: (NSString*) type withModality: (BioType) modality withError: (NSString*) error;

/**
 *  Appends the passed biometric event to this GMIResponse.
 *
 *  @param type either "Enrollment" or "Verification" (use gmiresp constants)
 *  @param modality the modality for this event
 */
- (void) appendSuccessfulBiometricEvent: (NSString*) type withModality: (BioType) modality;

/**
 *  Peek at the builder's GMIResponse object without delivering it. This object should not be modified and the result of this method should never be used to perform a GUI change (for example, because of a successful verification). 
 *
 *  If you want to receive an GMIResponse object when it's delivered (and then be able to use its info to perform program logic), create a class that adheres to the GMIResponseDelegate protocol and set the GMIClient's responseDelegate to an instance of that class.
 *
 *  @return the GMIResponse object.
 */
- (GMIResponse*) peek;

@end
