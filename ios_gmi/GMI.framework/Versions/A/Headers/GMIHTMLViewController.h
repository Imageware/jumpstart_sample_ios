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
 * Description	: The GMIHTMLViewController provides a controller interface to the HTML renderer UIWebView. A user can call the GMIClient's render method and pass in a custom UIWebView, in which case this class is not used - however, if the user passes in a NavigationController, a new GMIHTMLViewController is created and pushed, in which case this class is used.
 */

#import <UIKit/UIKit.h>
#import "GMIClient.h"
#import "CaptureManager.h"
#import "RenderLock.h"

@class GMIMessage;
@class GMIPerson;
@class JSContext;

@interface GMIHTMLViewController : UIViewController <UIWebViewDelegate, UIAlertViewDelegate>

@property (nonatomic,weak) IBOutlet UIWebView *webView;
@property (nonatomic,weak) IBOutlet UIView *loadingView;
@property (nonatomic,strong) GMIMessage* message;
@property (nonatomic,assign) BOOL animated;
@property (nonatomic,strong) JSContext *context;
@property (nonatomic,strong) RenderLock *renderLock;
@property (nonatomic,strong) GMIPerson *person;

/**
 *  Displays an Enroll or Verify dialog with the passed metadata and GMIJSExport's callback UUID. The metadata determines the modality of the event, as well as the number of samples.
 *
 *  The callback UUID describes a UUID that's tied to a closure in the JS bridge, called by the GMIJSExport class after the event is complete, or if there is an error along the way.
 */
+ (void) showEnrollDialog: (NSDictionary*) metadata withCallbackUUID: (NSString*) uuid forGMIMessage: (GMIMessage*) message;
+ (void) showVerifyDialog: (NSDictionary*) metadata withCallbackUUID: (NSString*) uuid forGMIMessage: (GMIMessage*) message;

/**
 * Finishes rendering the message, performing some general cleanup as needed and popping the webview from the navigation stack.
 */
+ (void) endMessage: (NSString*) arguments;
+ (void) endMessageJS: (NSString*) arguments;
+ (void) abandonMessage: (NSString*) arguments;

/**
 * 
 */
- (instancetype)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil withMessage: (GMIMessage*) gmiMessage withRenderLock: (RenderLock *) lock;

/**
 *  Returns the current instance of the GMIHTMLViewController. This lets you do really fancy things like set up custom back buttons or modify your web view logic, stuff like that.
 *
 *  Please note that doing custom GUI things to this instance, like setting back buttons and doing custom webview things, could totally break this in unexpected ways and that you should thoroughly test your changes.
 */
+ (GMIHTMLViewController *) currentInstance;

+ (bool) isAnimated;

+ (void) cancelBiometricsAttempt;
+ (void) cancelBiometricsAttemptWithMessage: (NSString *) message;

- (void) popToWebView;

@end
