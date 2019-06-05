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
 * File         : GMIClient.h
 *
 * Description	: This singleton object provides the main way ofNSArray<NSString *> *algorithms
 * of communication between the parent app and the SDK's functionality.
 */

#import <UIKit/UIKit.h>

#import "GMIConstant.h"
#import "GMIMessage.h"
#import "GMIErrors.h"
#import "GMIPerson.h"
#import "GMIMessage.h"
#import "GMIResponse.h"

//#define DEFAULT_TENANT @"ImageWare"

@class GMIServer;
@class GMIBaseComm;
@class GMICredentials;
@class BiometricCaptureData;
@class PersonManager;

/**
 *  Protocol to listen for message callback events; this lets you capture events that will be sent to the server, or successful face/voice verification through the GMIReponse object.
 */
@protocol GMIResponseDelegate <NSObject>
- (void) onMessageResponse: (GMIResponse *) response;
@end

/**
 *  Interface that provides singleton-access to the GMI system.
 */
@interface GMIClient : NSObject

@property (retain) GMIPerson *person __deprecated_msg("The GMIClient's global person is deprecated, please use the person parameter of individual calls moving forward.");
@property (retain) GMIServer *server;

@property (nonatomic, retain) id<GMIResponseDelegate> responseDelegate;

/**
 *  Creates and returns a singleton instance of GMIClient. This singleton is the majority of the interface to the GMI SDK.
 *
 *  @discussion this must be done only once in your program's life cycle, before you call the sharedClient method.
 *
 *  @param serverURL the URL to the GMI server associated with this app.
 *  @param tenant the tenant provided to you by ImageWare. The default is ImageWare (you probably don't want the default)
 *  @param appCode the app code you created to interact with GMI.
 *  @param clientID the client ID portion of your OAUTH credentials
 *  @param clientSecret the client secret portion of your OAUTH credentials
 *
 *  @return a singleton instance of GMIClient
 */
+ (GMIClient *) startGMI: (NSString *) serverURL withTenant: (NSString *) tenant withAppCode: (NSString *) appCode withClientCredentials: (NSString*) clientId withSecret: (NSString*) clientSecret;


/*
 *  Creates and returns a singleton instance of GMIClient. This singleton is the majority of the interface to the GMI SDK.
 *
 *  @discussion this must be done only once in your program's life cycle, before you call the sharedClient method.
 *
 *  @param dict a dictionary with the rest call paths defined
 *  example dictionary content
 */
+ (GMIClient *) sharedClient;

/**
 * Sets the tenant for the GMIClient to the passed tenant.
 */
- (void) setTenant: (NSString*) tenant;

/**
 * Gets the set tenant. By default the tenant is "ImageWare".
 *
 * @return the set tenant, or "ImageWare" if the tenant has not been set
 */
- (NSString*) getTenant;

/**
 * Sets the app code used internally and by the server, specifically when registering a device with the server.
 */
- (void) setAppCode: (NSString*) appCode;

/**
 * Gets the set app. By default the app is "ImageWare", but the server will return an error when trying to register the device; you'll need to set the app using setAppCode before attempting to register.
 *
 * @return the set app, or "ImageWare" if the app has not been set
 */
- (NSString*) getAppCode;

/**
 * Sets your client ID and Secret for communicating with the server.
 * 
 * @param clientId
 * @param clientSecret
 */
- (void) setClientCredentials: (NSString*) clientId withSecret: (NSString*) clientSecret;

/**
 *  Renders the passed GMIMessage to the passed UINavigationController object, with a toggle for animation. A GMI-side UIWebView and its associated Controller object will be pushed onto the passed UINavigationController, with the toggled animation as appropriate.
 *
 *  @discussion calling this message more than once will cause the failure() block to be called with the error "A message is already being rendered."
 *
 *  @param message the GMIMessage object to render.
 *  @param navController the UINavigationController to render to. The GMI custom web view controller and any associated biometric event controllers will be pushed onto this controller and popped when rendering is completed.
 *  @param animated the animation toggle. If true, subsequent pushes and pops will be animated.
 *  @param success a block called when rendering successfully pushed the webview onto the navigation stack. Do <b>not</b> perform UI stuff in the success block.
 *  @param failure a block called when rendering failed for some reason. The error details why. Possible failure cases are when we're already rendering a message, if the device doesn't support one of the message's required CaptureTypes, or if the message is nil, for example.
 */
- (void) renderMessage:(GMIMessage *) message withNavController:(UINavigationController*) navController withAnimation:(BOOL) animated withSuccess: (void (^)(void)) success withFailure: (void (^)(GMIError *error)) failure;

/**
 * Registers this device with the server for the current person using the passed APN push token. Calls the passed success or failure blocks as appropriate. This registration is pending, requiring a second factor of authentication determined by the server's configuration (typically, it's email).
 *
 *  @param pushToken the push token of the device, which is given when the device registers for APN, or nil
 *  @param success
 *  @param failure
 */
- (void) registerDeviceWithUserId: (NSString *) userId success: (void (^)(void))success failure:(void (^)(GMIError *error))failure;
- (void) registerDeviceWithUserId: (NSString *) userId withPushToken: (NSData *) pushToken success: (void (^)(void))success failure:(void (^)(GMIError *error))failure;

/**
 *  Returns the device list for the currently active person.
 *
 *  @param success   a block to be called on success, contains an array of GMIDevice objects.
 *  @param failure   a block to be called on failure
 */
- (void) getDeviceListForPersonSuccess: (void (^)(NSArray *deviceList)) success failure: (void (^)(GMIError *error)) failure __deprecated_msg("Use of the GMIClient's global person is deprecated, please use the version of this function that takes the person as a parameter instead.");
- (void) getDeviceListForPerson: (GMIPerson *) person success: (void (^)(NSArray *deviceList)) success failure: (void (^)(GMIError *error)) failure;

/**
 *  Gets array of GMIPerson objects where each person has this device registered to it. Calls the passed success or failure block as appropriate.
 *
 * @discussion when checking credentials, you can check that the userId for one of the people matches the credential userId you're checking for.
 *
 * @param success   a block to be called on success, with its argument being an array of GMIPerson objects.
 * @param failure   a block to be called on failure
 */
- (void) getPeopleForThisDeviceSuccess: (void (^)(NSArray *arrayOfPeople))success failure:(void (^)(GMIError *error))failure;

/**
 *  Gets the GMIPerson from the server for the passed user ID, performing one of the passed blocks as appropriate.
 *
 *  @param userId the User ID to check for
 *  @param success a block to call on success, with the retrieved GMIPerson instance as its argument.
 *  @param failure a block to call if this attempt fails
 */
- (void) getGMIPersonByUserId: (NSString*) userId success: (void (^)(GMIPerson *person))success failure:(void (^)(GMIError *error))failure;

/**
 *  Gets a person, if any, with the passed UUID and a previously set tenant (using setTenant:), or ImageWare (the default tenant) if the tenant has not been set.
 *
 *  @param uuid the UUID to check for
 *  @param success a block to call on success, with the retrieved GMIPerson instance as its argument.
 *  @param failure a block to call if this attempt fails
 *
 *  @discussion this method will fail with an error if a person with the passed UUID exists, but is not tied to the set tenant.
 */
- (void) getGMIPersonByUUID: (NSString*) uuid success: (void (^)(GMIPerson *person))success failure:(void (^)(GMIError *error))failure;

/**
 *  Unregisters the current device for this app on the server.
 *
 *  @param success a block to call on successful unregistration
 *  @param failure a block to call if the attempt fails for some reason
 */
- (void) unregisterApplicationForDeviceSuccess: (void (^)(void)) success failure: (void (^)(GMIError *error)) failure __deprecated_msg("Use of the GMIClient's global person is deprecated, please use the version of this function that takes the person as a parameter instead.");
- (void) unregisterApplicationForDeviceForPerson: (GMIPerson *) person success: (void (^)(void)) success failure: (void (^)(GMIError *error)) failure;

/**
 *  Checks whether or not the current current device is associated with the current app on the current server.
 *
 * @param success a block to call if the device is associated
 * @param failure a block to call if the attempt fails. This can fail because of a 404 (the device isn't registered), or because of an error (503, etc) - you should check the GMIError returned in the failure block to make sure which it is.
 */
- (void) getDeviceAppAssociationSuccess: (void (^)(void)) success failure: (void (^)(GMIError *error)) failure __deprecated_msg("Use of the GMIClient's global person is deprecated, please use the version of this function that takes the person as a parameter instead.");
- (void) getDeviceAppAssociationForPerson: (GMIPerson *) person success: (void (^)(void)) success failure: (void (^)(GMIError *error)) failure;

/**
 * Gets the list of pending enrollment messages from the server for the current person and application.
 *
 * @discussion this method calls GET app/{app_code}/person/{person_uuid}/enroll
 *
 * @param success a block called when the method succeeds. It is given an array of GMIMessage objects (note that the array can be empty, meaning the method succeeded but there were no objects).
 * @param failure a block called if the method fails for an unexpected reason.
 */
- (void) getPendingEnrollmentMessagesSuccess: (void (^)(NSArray *messages)) success failure: (void (^)(GMIError *error)) failure __deprecated_msg("Use of the GMIClient's global person is deprecated, please use the version of this function that takes the person as a parameter instead.");
- (void) getPendingEnrollmentMessagesForPerson: (GMIPerson *) person success: (void (^)(NSArray *messages)) success failure: (void (^)(GMIError *error)) failure;

/**
 * Tells the server to perform an enrollment using the passed BiometricCaptureData object.
 *
 * Note that this version is now deprecated as of version >3.8.3.
 *
 *  @param data the BiometricCaptureData object to use for this call. It contains the file information as well as modality, liveness detection information, etc. An invalid BiometricCaptureData being passed in will call the failure() block with the appropriate error, and will also populate a field in the GMIResponse object you'll receive through the appropriate listener.
 *  @param success the success block takes a boolean (success or fail), and a string reason. The success block is called when the server is hit successfully, and the boolean it takes tells you whether or not the enrollment was actually successful. If the BOOL is NO, the string contains the reason why.
 *  @param failure a block called if the enrollment call fails to hit the server for some reason.
 */
- (void) enroll:(BiometricCaptureData *) data withTenant:(NSString *) tenant success: (void (^)(BOOL enrollmentSucceeded, NSString* reasonForFailure)) success failure: (void (^)(GMIError *error)) failure;

/**
 * Tells the server to perform a biometric verification using the passed BiometricCaptureData object.
 *
 *  @param data the BiometricCaptureData object to use for this call. It contains the file information as well as modality, liveness detection information, etc. An invalid BiometricCaptureData being passed in will call the failure() block with the appropriate error, and will also populate a field in the GMIResponse object you'll receive through the appropriate listener.
 *  @param success the success block takes a boolean (success or fail), and a string reason. The success block is called when the server is hit successfully, and the boolean it takes tells you whether or not the verification was actually successful. If the BOOL is NO, the string contains the reason why.
 *  @param failure a block called if the verification call fails to hit the server for some reason.
 */
- (void) verify:(BiometricCaptureData *) data withTenant:(NSString *) tenant success: (void (^)(BOOL verificationSucceeded, NSString* reasonForFailure)) success failure: (void (^)(GMIError *error)) failure;

/**
 *  Pulls a template from the server in the form of an GMIMessage. Calls the success or failure block as appropriate.
 *
 *  @discussion since direct messages are tied to the person UUID client-side, you'll need to set the person UUID for this GMIClient before calling this method - otherwise it will fail and return an error to the pullDelegate.
 *
 *  @param templateID   the template to pull
 *  @param success      a block to call on success
 *  @param failure      a block to call on failure
 */
- (void) pullDirectMessage: (NSString*) templateId success:(void (^)(GMIMessage *message)) success failure:(void (^)( GMIError *error))failure __deprecated_msg("Use of the GMIClient's global person is deprecated, please use the version of this function that takes the person as a parameter instead.");
- (void) pullDirectMessage: (NSString*) templateId forPerson: (GMIPerson *) person success:(void (^)(GMIMessage *message)) success failure:(void (^)( GMIError *error))failure;

/**
 *  Pulls a template from the server in the form of a GMIMessage compiled with the passed metadata. Calls the success or failure block as appropriate.
 *
 *  @param templateID   the template to pull
 *  @param metadata     the metadata to replace template variables. Cannot be nil, pass @{ } if you want no metadata.
 *  @param success      a block to call on success
 *  @param failure      a block to call on failure
 */
- (void) pullDirectMessage: (NSString*) templateId withMetadata: (NSDictionary*)metadata success:(void (^)(GMIMessage *message))success failure:(void (^)(GMIError *error)) failure __deprecated_msg("Use of the GMIClient's global person is deprecated, please use the version of this function that takes the person as a parameter instead.");
- (void) pullDirectMessage: (NSString*) templateId forPerson: (GMIPerson *) person withMetadata: (NSDictionary*)metadata success:(void (^)(GMIMessage *message))success failure:(void (^)(GMIError *error)) failure;

/**
 *  Retrieves an array of NSDictionary objects in order of the messages. The returned dictionaries contain all of the message data for each message, but are not themselves renderable messages.
 *
 *  @discussion this call is intended for use in things like inbox apps, where you want to display messages without rendering them. To get a renderable message, you can use this method to get message UUIDs and then call getMessage: with that ID. For most use cases, you should just call getAllMessages: or getFirstMessage: instead.
 *
 *  @param success a block called when this method succeeds. It takes the returned array of dictionaries of message info.
 *  @param failure the failure block, passed an error pointing out why this call failed.
 */
- (void) getAllMessageDataSuccess: (void (^)(NSArray *messageData)) success withFailure: (void (^)(GMIError *error)) failure __deprecated_msg("Use of the GMIClient's global person is deprecated, please use the version of this function that takes the person as a parameter instead.");
- (void) getAllMessageDataForPerson: (GMIPerson *) person success: (void (^)(NSArray *messageData)) success failure: (void (^)(GMIError *error)) failure;

/**
 *  Retrieves a message with the passed ID, for the stored person, if any exists.
 *
 *  @param messageId the ID for the message
 *  @param success the block called with the successful message, if everything works
 *  @param failure a failure block called if something bad happens.
 */
- (void) getMessage: (NSString*) messageId success: (void (^)(GMIMessage *message)) success failure: (void (^)(GMIError *error)) failure __deprecated_msg("Use of the GMIClient's global person is deprecated, please use the version of this function that takes the person as a parameter instead.");
- (void) getMessage: (NSString*) messageId forPerson: (GMIPerson *) person success: (void (^)(GMIMessage *message)) success failure: (void (^)(GMIError *error)) failure;

/**
 *  Retrieves any unread messages from the server. For each message retreived this way, this method will call onGetMessagesRequestSuccess. If there's a failure, will call the getMessagesDelegate getMessagesRequestFail.
 *
 *  @param success a block called when this message succeeds. It gets an array of GMIMessage objects containing all of the messages we retrieved for the current person.
 *  @param failure a failure block called if an error occurred trying to get the messages.
 */
- (void) getAllMessagesSuccess: (void (^)(NSArray *messageList)) success failure: (void (^)(GMIError *errors)) failure __deprecated_msg("Use of the GMIClient's global person is deprecated, please use the version of this function that takes the person as a parameter instead.");
- (void) getAllMessagesForPerson: (GMIPerson *) person success: (void (^)(NSArray *messageList)) success failure: (void (^)(GMIError *errors)) failure;

/**
 *  Retrieves the first, if any, message for the currently set person (use setPerson for this).
 *
 *  @param success a block called when this method succeeds. The argument to the block is the successfully grabbed GMIMessage object.
 *  @param failure the failure block, passed an error pointing out why this call failed.
 */
- (void) getFirstMessageSuccess: (void (^)(GMIMessage *message)) success withFailure: (void (^)(GMIError *error)) failure __deprecated_msg("Use of the GMIClient's global person is deprecated, please use the version of this function that takes the person as a parameter instead.");
- (void) getFirstMessageForPerson: (GMIPerson *) person success: (void (^)(GMIMessage *message)) success failure: (void (^)(GMIError *error)) failure;

/**
 *  Marks the passed message as read on the server, deleting it from the server's pool of messages. This method returns immediately, performing one of the passed blocks as appropriate once the server call returns.
 *
 *  @param message the message to delete
 *  @param success a success block to call if the operation is successful. Can be nil.
 *  @param failure a failure block to call if the operation fails. Can be nil.
 */
- (void) markMessageAsRead: (GMIMessage*) message success:(void (^)(void))success failure:(void (^)(GMIError *error)) failure;

/**
 *  Marks the passed message as rejected on the server. This method returns immediately, performing one of the passed blocks as appropriate once the server call returns.
 *
 *  @param message the message to mark as rejected
 *  @param success a success block to call if the operation is successful. Can be nil.
 *  @param failure a failure block to call if the operation fails. Can be nil.
 */
- (void) markMessageAsRejected: (GMIMessage *) message withReason: (NSString *) reason success:(void (^)(void))success failure:(void (^)(GMIError *error))failure;

/**
 *  Returns whether or not the GMI SDK is (probably) currently rendering a message. This is equivalent to polling the passed controller's top UIViewController's class, and testing if it's equivalent to any of the SDK's controller classes or the UIWebView.
 */
- (bool) isRenderingMessage: (UINavigationController*) controller;

/**
 * Updates the current device/app association on the server to have the passed push token. This is useful for when the user turns notifications on after having them off, to reset the push token and let the user start recieving push notifications.
 *  @param pushToken an NSData (or nil, which will set the push token to blank on the server). You can get this from the IMSUtility's helper method for the push token passed in your AppDelegate's registration method.
 *  @param success a success block called when the call completes successfully
 *  @param failure a failure block called if the server gives us an error, or there was a problem connecting.
 */
- (void) updateApplicationForDeviceWithPushToken: (NSData*) pushToken
                                         success: (void (^)(void))success
                                         failure: (void (^)(GMIError *error))failure;


@end

typedef GMIClient IMSClient __deprecated_msg("IMSClient is deprecated. Use 'GMIClient' instead.");
