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

#define GMI_DEVICE_UUID_KEY @"deviceId"
#define GMI_DEVICE_PUSHTOKEN_KEY @"pushToken"
#define GMI_DEVICE_PLATFORM_KEY @"platform"
#define GMI_DEVICE_PLATFORM_VERSION_KEY @"platformVersion"
#define GMI_DEVICE_CLIENT_VERSION_KEY @"clientVersion"
#define GMI_DEVICE_NAME_KEY @"name"

@interface GMIDevice : NSObject

@property NSString* uuid;
@property NSString* pushToken;
@property NSString* platform;
@property NSString* platformVersion;
@property NSString* clientVersion;
@property NSString* name;

/**
 *  Initializes a device with the passed uuid and push token. The platform is iOS by default.
 *
 *  @param uuid      the device's uuid
 *  @param pushToken the push token for this device
 */
- (instancetype) initWithUUID: (NSString*) devuuid withPushToken: (NSString*) devPushToken;

/**
 *  Initializes a device with the passed dictionary. As many fields as possible from the dictionary are populated in this Device.
 *
 *  Note that throwing an arbitrary dictionary at this method doesn't necessarily mean it'll work. The method will succeed but you could end up with an empty device, or a device with useless info.
 *
 *  @param dict the dictionary
 */
- (instancetype) initWithDictionary: (NSDictionary*) dict;

/**
 *  Returns whether the passed GMIDevice is equal to this device. Two GMIDevice objects are equal if they have the same UUID.
 *
 *  @param device the device to test against
 *
 *  @return true if the passed device is equal to (has the same UUID as) this device
 */
- (BOOL) isEqualToDevice: (GMIDevice*) device;

@end
