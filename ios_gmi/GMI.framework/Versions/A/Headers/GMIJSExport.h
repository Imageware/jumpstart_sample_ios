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
 * Description	: The GMIJSExport class is a Singleton-pattern class that contains the Javascript bridge for our HTMLRendering framework. It provides some data that the Javascript in a rendered "template" has access to, as well as providing some simple methods, such as getMetadata and getPersonUUID, that allow the Javascript to interface with the SDK.
 */

#import "GMIHTMLViewController.h"
#import "GMIConstant.h"
#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import <JavaScriptCore/JSValue.h>

@protocol GMIJSExport <JSExport>

/**
 *
 */
+ (NSString*) enroll: (JSValue*) arguments;

/**
 *
 */
+ (NSString*) verify: (JSValue*) arguments;

/**
 *
 */
+ (NSString*) getPerson;

/**
 *
 */
+ (NSString*) getDevice;

/**
 *
 */
+ (NSString*) getMetadata;

/**
 *
 */
+ (void) endMessage: (NSString*) arguments;

/**
 *
 */
+ (void) abandonMessage: (NSString*) arguments;

/**
 *  @return a list of the supported modalities for this device. 
 */
+ (NSString *) getSupportedModalities;
+ (NSString *) getSupportedCaptureTypes;
+ (NSString*) getEnrolledCaptureTypes;

/**
 *
 */
+ (NSString *) getDeviceInfo;

+ (NSString *) getBearerToken;

@end

@interface GMIJSBridge : NSObject <GMIJSExport>

/**
 *  Sets the current GMIMessage for this Bridge, giving the JavaScript access to the message's metadata.
 *
 * @param message the GMIMessage to provide access to
 */
+ (void) setCurrentMessage: (GMIMessage*) message;

/**
 *  Performs a callback in the Javascript Bridge for the passed callback UUID.
 *
 *  @discussion The UUID is generated when the webview presents a biometric capture view and is stored internally to the JS bridge mapped to a function pointer. This callback method will call the function pointer for the passed UUID, using the passed metadata as an argument.
 *
 *  Something to note - the function pointer with this UUID will be destroyed on this callback, so it's important not to call this method more than once for a given UUID. It probably wouldn't break the system to do so, but it definitely could turn out poorly and at best won't work.
 */
+ (void) doCallbackForUUID: (NSString*) uuid withMetadata: (NSDictionary*) metadata forUIWebView: (UIWebView*) view;

+ (void) clearBiometricsFlag;

/**
 *“success”: true, // or false
 “status”: “Success”, // or a message defining failure, such as Cancelled, Failed, Error, etc
 “modality”: [passed modality],
 “samples”: [passed samples],
 “message”: [error message, etc],
 “event-type”: [enrollment, verification, identification]
 */
+ (NSDictionary*) buildEventMetadataWithSuccess: (bool) success withStatus: (NSString*) status withModality: (NSString *) modality withSamples: (int) samples withMessage: (NSString*) message withErrorCode: (NSString *) code withBioCapEventType: (BioEventType) eventType;

@end
