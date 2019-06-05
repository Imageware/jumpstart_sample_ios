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
 * Description  : The GMIServer object provides a simple interface for keeping track of the server URI, the tenant ID and the API secret key.
 */

#import <Foundation/Foundation.h>

@interface GMIServer : NSObject

@property (strong,nonatomic) NSString *serverURI;
@property (strong,nonatomic) NSString *attServerURI;
@property (strong,nonatomic) NSString *tenantID;
@property (strong,nonatomic) NSString *appCode;
@property (strong,nonatomic) NSString *serverPath;
@property (strong,nonatomic) NSString *absolutePath;


/**
 * Creates a server around the passed URL.
 */
- (instancetype) initWithURL: (NSURL*) url;

/**
 * Returns the description of this GMIServer object, taking this form:
 *
 * URI: <server URI>
 * Tenant: <tenant>
 * App: <app ID>
 *
 * @return the description of this server
 */
- (NSString*) description;

@end
