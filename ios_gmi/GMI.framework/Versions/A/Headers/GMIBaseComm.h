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
 * Description	: The GMIBaseComm class acts as a default implementation of the GMICommunicator protocol. It provides a default implementation of all of the methods made available by the protocol.
 */

#import "GMICommunicator.h"
#import "GMISerializer.h"

#define REJECTION_HEADER @"rejectionInfo"
#define REJECTION_JSON @"rejectionString"

@interface GMIBaseComm : NSObject <GMICommunicator>

- (bool) pingTemplateURL: (NSString *) url withError: (GMIError **) error;

// get an authorization token for OAUTH
- (NSString*) getAuthorizationToken: (GMICredentials*) creds onServer: (GMIServer *) server withError: (GMIError **) error;

- (void) downloadUpdateJSONwith: (void (^)(NSDictionary *json)) success;

@end
