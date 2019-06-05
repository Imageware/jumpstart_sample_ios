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
 * Description	: A protocol that defines a set of communication methods that will be supported server-side. The protocol is designed to be comm-agnostic if possible (that is, it will support REST and should be able to support other methods).
 * 
 * These methods all block unless specified.
 */

#import "GMIPerson.h"
#import "GMIServer.h"
#import "GMIDevice.h"
#import "GMIBiometricProbe.h"
#import "GMIErrors.h"
#import "GMICredentials.h"
#import "GMIMessage.h"

@protocol GMICommunicator <NSObject>

/**
 *  Gets the person the passed device is registered to, if any. This will return nil in case of error, or if the device isn't registered to any person.
 *
 *  @param device the device to check
 *  @param server the server to check on
 *  @param error  an empty error
 *
 *  @return the person registered to the passed device, or nil if there is no person or in case of error. If there's an error, the passed error will be set.
 */
- (NSArray*) getPersonForDevice: (GMIDevice *) device onServer: (GMIServer *) server withCredentials: (GMICredentials *) creds withError: (GMIError **) error;

/**
 *  Gets the person the passed device is registered to, if any. This will return nil in case of error, or if the device isn't registered to any person.
 *
 *  @param userId the user ID to use for finding the person
 *  @param server the server to check on
 *  @param error an empty error
 *
 *  @return the person registered to the passed device, or nil if there is no person or in case of error. If there's an error, the passed error will be set.
 */
- (GMIPerson*) getPersonForUserId: (NSString*) userId onServer: (GMIServer *) server withCredentials: (GMICredentials *) creds withError: (GMIError **) error;

/**
 *  Gets the person for the passed UUID, for the tenant set in the GMIClient instance.
 *
 *  @param person the person UUID to look for
 *  @param server the server to check on
 *  @param error an empty error
 *
 *  @return the person with the passed ID, registered with the tenant set in the GMIClient instance, or nil if there is no person or in case of error. If there's an error, the passed error will be set.
 *
 *  @discussion this method will return nil in the case where a person with the passed UUID exists, but isn't tied to the tenant. Something to keep in mind.
 */
- (GMIPerson*) getPersonTenantInfoForPerson: (NSString *) person onServer: (GMIServer *) server withCredentials: (GMICredentials *) creds withError: (GMIError **) error;

/**
 *  Gets the list of devices registered to the passed person on the passed server. This will return an array of GMIDevice objects.
 *
 *  @param person the person object to find the list of devices for
 *  @param server the server to check
 *  @param error  an empty error object
 *
 *  @return an NSArray of GMIDevice objects if successful. If this person is registered to no devices, this will return an empty, but initialized array. If an error occurs, this will return a nil array and the passed error will be set.
 */
- (NSArray*) getDeviceListForPerson: (GMIPerson*) person onServer: (GMIServer *) server withCredentials: (GMICredentials *) creds withError: (GMIError **) error;

/**
 *  Updates the passed device's info for the passed person, on the passed server. Used primarily to update the device's push token.
 *
 *  @param device the device to register
 *  @param server the server to register on
 *  @param error  an empty error object
 *
 *  @return YES if successful; else NO and error will be set
 */
- (bool) updateDevice: (GMIDevice*) device onServer: (GMIServer *) server withCredentials: (GMICredentials *) creds withError: (GMIError **) error;

/**
 *  Registers the passed device for the passed person, on the passed server.
 *
 *  @param device the device to register
 *  @param person the person for which to register the device
 *  @param server the server to register on
 *  @param error  an empty error object
 *
 *  @return YES if successful; else NO and error will be set
 */
- (BOOL) registerDevice: (GMIDevice*) device forUserIdForPerson: (NSString *) userId onServer: (GMIServer *) server withCredentials: (GMICredentials *) creds withError: (GMIError **) error;

/**
 *  Unregisters the passed device for the passed person, on the passed server.
 *
 *  @param device the device to unregister
 *  @param person the person the device is linked to
 *  @param server the server to delete the device from
 *  @param error  an empty error object
 *
 *  @return YES if the deletion was successful (the server returned a 200 or a 204), else NO. If an error occurred, the passed error will be set and this will return NO.
 */
- (BOOL) dissociateAppAndDevice: (GMIDevice*) device forPerson: (GMIPerson *) person onServer: (GMIServer *) server withCredentials: (GMICredentials *) creds withError: (GMIError **) error;

/**
 *  Returns whether or not the passed device is associated with the passed person and the app on the passed server.
 *
 *  @param device the device to unregister
 *  @param person the person the device is linked to
 *  @param server the server to delete the device from
 *  @param error  an empty error object
 *
 *  @return YES if the device is associated (the server returned a 200), else NO. If an error occurred, the passed error will be set and this will return NO.
 */
- (BOOL) isDeviceAssociated: (GMIDevice*) device forPerson: (GMIPerson *) person onServer: (GMIServer *) server withCredentials: (GMICredentials *) creds withError: (GMIError *__autoreleasing *) error;

/**
 * Gets a list of all of the enrollment messages pending for the passed user and the passed app code.
 *
 * @param personUuid the person
 * @param appCode the app code to check for
 * @param creds
 * @param error the error if we mess up with JSON or something
 */
- (void) getEnrollMessagesForPerson: (NSString *) personUuid onServer: (GMIServer *) server withCredentials: (GMICredentials *) creds withError: (GMIError *__autoreleasing *) error success: (void (^)(NSArray<GMIMessage *> *messages)) success failure: (void (^)(GMIError *error)) failure;

/**
 *  Biometrically verifies the passed person, using the passed probe on the passed server. The probe can contain any number of data items.
 *
 *  @param person the person to verify
 *  @param probe  the probe used to verify the person
 *  @param server the server to verify on
 *  @param error  an empty error object, or nil if you don't care about errors
 */
- (void) verifyPerson: (GMIPerson*) person withProbe: (GMIBiometricProbe*) probe onServer: (GMIServer *) server withMessageURI: (NSString*) messageURI withTenant: (NSString*) tenant withCredentials: (GMICredentials *) creds success: (void (^)(NSString *result)) success failure: (void (^)(GMIError *error)) failure;

/**
 *  Biometrically enrolls the passed person on the passed server, using the passed probe. The probe can contain any number of data items.
 *
 *  @param person the person to enroll
 *  @param probe  the probe used to enroll the person
 *  @param server the server to enroll on
 *  @param error  an empty error object
 */
- (void) enrollPerson: (GMIPerson*) person withProbe: (GMIBiometricProbe*) probe onServer: (GMIServer *) server withMessageURI: (NSString*) messageURI withTenant: (NSString*) tenant withCredentials: (GMICredentials *) creds success: (void (^)(NSString *result)) success failure: (void (^)(GMIError *error)) failure;

/**
 *  Pulls a template with the passed name from the passed server and prepares it as an GMIXMLTemplate using the passed metadata, returning that object.
 *
 *  @param templateID the string name of the template, for example "Dual_Enrollment". This should not be the template's fully-qualified URI.
 *  @param personUuid the UUID of the person you're pulling the message for
 *  @param metadata an NSDictionary of metadata to insert into the template. This should directly corrolate template variable keys to data objects (for example, "app_name"->"My Application Name"). You can also pass nil for this, if the template has no metadata.
 *  @param server the server to pull the template from
 *  @param error an empty error object
 *
 *  @return a GMIMessage that's ready to be rendered, if successful, or nil if there was an error (in which case, the passed error will be set).
 */
- (GMIMessage*) pullDirectMessage: (NSString *)templateID forPersonId: (NSString *) personUuid withMetadata: (NSDictionary*)metadata onServer: (GMIServer *) server withCredentials: (GMICredentials *) creds withError: (GMIError **) error;

/**
 *  Returns an NSArray of all the unread message objects on the passed server, for the passed person.
 *  Note - this does not mark the collected messages as read. For that, use the markMessageAsRead method.
 *
 *  @param person the person to check for
 *  @param server the server to retrieve messages from
 *  @param appCode the app code for this person
 *  @param success
 *  @param failure
 *
 *  @return an NSArray of GMIMessages on success. If successful but there are no messages, this array will be empty. If unsuccessful because of an error, the error block will be called.
 */
- (void) getAllMessagesForPerson: (GMIPerson *) person onServer: (GMIServer *) server withAppCode: (NSString*) appCode withCredentials: (GMICredentials *) creds success: (void (^)(NSArray *messages)) success failure: (void (^)(GMIError *error)) failure;

/**
 *  Gets a message object with the passed message UUID, on the passed server with the passed person.
 *
 *  @param messageId the message to grab
 *  @param person the person to check for
 *  @param server the server to retrieve messages from
 *  @param appCode the app code for this person
 *  @param success
 *  @param failure
 *
 *  @return a GMIMessages on success. If unsuccessful because of an error, the error block will be called.
 */
- (void) getMessage: (NSString*) messageId forPerson: (GMIPerson *) person onServer: (GMIServer *) server withAppCode: (NSString*) appCode withCredentials: (GMICredentials *) creds success: (void (^)(GMIMessage *message)) success failure: (void (^)(GMIError *error)) failure;

/**
 *  Returns an NSArray of all the unread message objects on the passed server, for the passed person.
 *
 *  @param person the person to check for
 *  @param server the server to retrieve messages from
 *  @param appCode the app code for this person
 *  @param success
 *  @param failure
 *
  *  @return an NSArray of NSDictionaries on success,  If successful but there are no messages, this array will be empty. If unsuccessful because of an error, the error block will be called.
 */
- (void) getAllMessageDataForPerson: (GMIPerson *) person onServer: (GMIServer *) server withAppCode: (NSString*) appCode withCredentials: (GMICredentials *) creds success: (void (^)(NSArray *messageData)) success failure: (void (^)(GMIError *error)) failure;

/**
 *  Returns an NSArray of all the unread message objects on the passed server, for the passed person.
 *  Note - this does not mark the collected messages as read. For that, use the markMessageAsRead method.
 *
 *  @param person the person to check for
 *  @param server the server to retrieve messages from
 *  @param success
 *  @param failure
 *
 *  @return a GMIMessage on success. If successful but there are no messages, this will return nil. If unsuccessful because of an error, the error block will be called.
 */
- (void) getFirstMessageForPerson: (GMIPerson *) person onServer: (GMIServer *) server withAppCode: (NSString*) appCode withCredentials: (GMICredentials *) creds success: (void (^)(GMIMessage *message)) success failure: (void (^)(GMIError *error)) failure;

/**
 *  Marks the passed message as read on the passed server.
 *
 *  @param message the message to mark as read
 *  @param person  the person to delete the message for
 *  @param server  the server
 *  @param error   a blank error object
 *
 *  @return YES if successful, else NO. If an error occurs, this will return NO and the passed error will be set.
 */
- (BOOL) markMessageAsRead: (GMIMessage *) message withPerson: (GMIPerson*) person onServer: (GMIServer *) server withCredentials: (GMICredentials *) creds withError: (GMIError **) error;

/**
 *  Marks the passed message as rejected on the passed server.
 *
 *  @param message the message to mark as read
 *  @param person  the person to delete the message for
 *  @param server  the server
 *  @param error   a blank error object
 *
 *  @return YES if successful, else NO. If an error occurs, this will return NO and the passed error will be set.
 */
- (BOOL) markMessageAsRejected: (GMIMessage *) message forReason: (NSString*) rejectionString withPerson: (GMIPerson*) person onServer: (GMIServer *) server withCredentials: (GMICredentials *) creds withError: (GMIError **) error;

//
- (void) attemptAbandonCallback: (NSString*) rejectionString withMessage: (GMIMessage *) message withPerson: (GMIPerson*) person onServer: (GMIServer *) server withCredentials: (GMICredentials *) creds withError: (GMIError **) error;

/**
 * 
 */
- (NSArray *) getPersonAlgorithms: (NSString *) uuid onServer: (GMIServer *) server withCredentials: (GMICredentials *) creds withError: (GMIError * __autoreleasing *) error;

/*
 */
- (NSArray *) getAlgorithmsForCapture:(NSString *)type person:(NSString *)uuid onServer:(GMIServer *)server withCredentials:(GMICredentials *)creds withError:(GMIError  *__autoreleasing *)error;

@end
