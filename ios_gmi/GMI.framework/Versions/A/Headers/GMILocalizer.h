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
 * Description	: Utility class for localization. Lets the app implementor override our default localization, should they choose to.
 */

#import <Foundation/Foundation.h>

#define LocStr(str) [GMILocalizer getLocalizedString:str]

@interface GMILocalizer : NSObject

/**
 *  Sets the default bundle for the SDK localization. Once this is called, the SDK will use the provided bundle for any localization resources, overriding our defaults. If no localization is provided for a string, it will fall back to our defaults.
 *
 *  @param bundle the NSBundle that will override our localization files.
 */
+ (void) useBundle: (NSBundle *) bundle;

+ (NSString *) getLocalizedString: (NSString *) str;

@end
