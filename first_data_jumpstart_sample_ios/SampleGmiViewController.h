//
//  SampleGmiViewController.h
//  Sample
//
//  Created by Henry Chan on 1/4/19.
//  Copyright © 2019 ImageWare Systems, Inc. All rights reserved.
//
//  Calls via the GMI SDK.  Verfication and registartion are performed using the SDK view controller.
//

#import <UIKit/UIKit.h>
#import <GMI/GMI.h>

// Dictionary keys
#define TENANT_NAME   @"tenant"
#define CLIENT_ID     @"clientid"
#define CLIENT_SECRET @"clientsecret"
#define APP_CODE      @"appcode"
#define SERVER_NAME   @"servername"
#define CODED_64      @"client64"


// UserDefaults
#define VALID_USER    @"lastUserKey"
#define VALID_UUID    @"lastUUIDKey"
#define USERDEFAULTPUSH    @"PushToken"




@interface SampleGmiViewController : UIViewController <GMIResponseDelegate>

- (id) initWithCredentials:(NSDictionary *) dictData withUserName: (NSString *) userName;
-(void) sampleResetUserRegistered;
-(void) setMessageIdentifier : (NSString *) idString;
-(void) presentVerification:(void (^)(NSString *messageId)) completion;
-(void) setUserName: (NSString *) tName;
-(void) getEnrollsForPerson : (GMIPerson *)person success:(void (^)(BOOL success)) success;
-(void) noEnrollProcessing:(BOOL) disableEnrolls;
-(void) registerUser: (NSString *) userId completion:(void (^)(GMIPerson * gmiPerson)) completion;
-(void) setCurrentGmiPerson: (GMIPerson *) nowPerson;
-(void) findUserId : (NSString *) userId success: (void (^)(GMIPerson *))foundPerson;


+(NSDictionary *) clientCredentials;


+(NSString *) sampleGetUserRegistered;


+(NSString *) getUserName;

+(BOOL) fingerprintEnabled;  // Convenience check
//
// Enroll messages remain on server for each user until completed.
// Possible to track if the user skipped (in OnMessageResponse) and not show that particular
// biometric enrollment.  This gets more complex if multiple users are using the same app.
// The folllowing methods allow tracking and supressing particular types.
//
+(void) suppressEnrollCaptureType: (NSString *) captureType;
+(void) allowEnrollCaptureType: (NSString *) captureType;
+(BOOL) skipEnrollCaptureType: (NSString *) captureType;
+(void) clearEnrollCaptureType;

+(BOOL) enrolledCaptureType:(NSString *) captureType forPerson:(GMIPerson *)person;
+(void) setOrientationAll;
+(void) setOrientationPortrait;


@end
