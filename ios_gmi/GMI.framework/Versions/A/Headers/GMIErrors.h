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
 */

#import <Foundation/Foundation.h>

#define GMI_DOMAIN @"com.iwsinc.gmi"

#define GMIERROR_IS_NSERROR 3131
#define MESSAGE_METADATA_ISSUE -2020
#define RENDERING_ERROR -187
#define GMIERROR_NIL_TEMPLATE -188
#define AUTHORIZATION_ERROR -1012

#define GMIERROR_DESCRIPTION_KEY @"description"
#define GMIERROR_ERRORID_KEY @"errorId"
#define GMIERROR_CODE_KEY @"code"
#define GMIERROR_ID_KEY @"id"
#define GMIERROR_ERROR_LIST @"errors"

// a list of errors we report
#define GMI_ERROR_EMPTY_CAPTURE @"Modality count of 0, unsupported enroll or verify."
#define GMI_ERROR_EMPTY_CAPTURETYPE_IN_MESSAGE @"Empty capture type %@ not allowed."
#define GMI_ERROR_MISENROLL @"Enroll calls must be unique and the message must include enrollment metadata. For more information view the server documentation."

//When we download a message with a bad capture type (ie “passphrase”:[“passphrase”] or “passphrase”:[“voice”,”passphrase”] or “passphrase:[“mispelled”]):
#define GMI_ERROR_UNSUPPORTED_CAPTURE_TYPE_IN_MESSAGE @"Unsupported capture type (%@) in message %@. Cannot continue."

@interface GMIError : NSError

@property (retain) NSString *gmiErrorCode; // the GMI server error code, for example gmi.user.not.found
@property (retain) NSString *uniqueErrorId; // the error ID from the server, unique to each error instance
@property (retain) NSString *errorDesc; // a string description of the error, primarily used for debugging issues
@property int responseCode; // the response code given by the error. For server calls this corresponds to the HTTP error code (404, 500, etc), or one of the iOS system codes (-1009 for example)

- (instancetype) initWithResponseCode: (int) responseCode withUserInfo: (NSDictionary*) userInfo ;

+ (GMIError *) errorFromNSError: (NSError *) error;

@end
