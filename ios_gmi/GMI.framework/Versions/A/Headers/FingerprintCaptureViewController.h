/*************************************************************************
 *
 * IMAGEWARE SYSTEMS, INC. CONFIDENTIAL
 * __________________
 *
 * 2000 - 2015 ImageWare Systems, Inc.
 * All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains the
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
 * File : FingerprintCapturePreview.h
 *
 * Description	: Simple fingerprint capture preview, used to prompt the user for a sample fingerprint. Because of limitations of the fingerprint API, we use the LocalAuthentication framework for this.
 */

#import <UIKit/UIKit.h>
#import "CaptureViewController.h"
#import "SkinManager.h"

@interface FingerprintCaptureViewController : CaptureViewController <Skinnable>

@property (weak, nonatomic) IBOutlet UILabel *scanYourFingerprintLabel;

+ (BOOL) hasFaceScanner;

@end
