//
//  SampleGmiViewController.m
//  Sample
//
//  This project is an example of solution for biometric verification.
//  Before using this application, you will need to have the following set up:
//
//  1) Tenant ID
//  2) Client ID
//  3) Client Secret
//
//  The registration, enrollments, and verification are processed through the GMI SDK.
//  Enroll and verification message templates are displayed via a GMI SDK method. Responses from
//  the templates are handed in the callback OnMessageResponse.
//
//  Created by Henry Chan on 1/4/19.
//  Copyright © 2019 ImageWare Systems, Inc. All rights reserved.
//

#import "AppDelegate.h"
#import "GmiServerAccess.h"
#import "SampleGmiViewController.h"
#import <GMI/GMIHTMLViewController.h>

#import <AudioToolbox/AudioToolbox.h>
#import <LocalAuthentication/LocalAuthentication.h>  // For TouchID


@interface SampleGmiViewController ()
{
    NSString *userName;
    NSDictionary *serverCredentials;
    BOOL onlyOnce;
    UIView *waitingNotice;
    NSMutableArray *gmiMessages;  // Array of GMIMessage
    NSInteger enrollCount;  // Track valid enrolls, so we can restrict certain combinations.
    GMIMessage *gmi_message;
    NSString *bearerToken;
    NSString *messageId;
    void (^successBlock)(NSString *meId);
    AppDelegate *appDelegate;
    BOOL disabledEnrollProcessing;
    GMIPerson *thisGMIPerson;
    BOOL verifyAfterEnroll;  // default false
    void (^verifyComplete)(NSString *returnMessId);
}
@end

@implementation SampleGmiViewController


#define USER_KEY @"KEY_USER_VALUE"         // UserDefaults key User UUID to use for verification.

#define SAMPLEUSER @"sampleUserRegistered"   // UserDefaults key for last valid username
#define SKIPLIST  @"sampleSkipList"         // UserDefaults key for skipped enrollments


//
// Client credentials may differ from server credentials.
//
+(NSDictionary *) clientCredentials
{
    NSDictionary *clientDict = @{
                                 TENANT_NAME : @"fdata",
                                 CLIENT_ID : @"fduser",
                                 CLIENT_SECRET : @"K311er$77",
                                 APP_CODE : @"authenticare",
                                 SERVER_NAME : @"https://deployment-gmi.iwsinc.com",  //"https://deployment-gmi.iwsinc.com"
                                 CODED_64 :  @"ZmR1c2VyOkszMTFlciQ3Nw==="  // clientID:ClientSecret ===> 64baseEncodedValue
                                 };

    return clientDict;
}




+(BOOL) fingerprintEnabled
{
    BOOL hasTouchID = NO;
    // if the LAContext class is available
    if ([LAContext class]) {
        LAContext *context = [LAContext new];
        NSError *error = nil;
        hasTouchID = [context canEvaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics error:&error];
    }
    return hasTouchID;
}



+(NSString *) getUserName
{
    NSString *uName = [[NSUserDefaults standardUserDefaults] objectForKey:USER_KEY];
    
    return uName;
}

// -- Enrollments are processed on app startup.  To skip a particular type, a local value is saved.
//  The settings view allows the type to be enabled again, allowing that enroll type to be processed.
//  The saved values are used regardless of what username is current.
//

/*
   suppressEnrollCaptureType - skip this type
 */
+(void) suppressEnrollCaptureType: (NSString *) captureType
{
    NSMutableArray *skipItems = [[NSMutableArray alloc] initWithArray:[self captureTypeList]];
    NSString *updatedList = nil;
    
    if (skipItems) // Check to see if already on list (don't add twice)
    {
        if ([skipItems containsObject:captureType] == NO)
        {
            [skipItems addObject:captureType];
        }
    }
    else
    {
        [skipItems addObject:captureType];
    }
    
    updatedList = [skipItems componentsJoinedByString:@","];
    [[NSUserDefaults standardUserDefaults] setObject:updatedList forKey:SKIPLIST];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

/*
    allowEnrollCaptureType - process this type
 */
+(void) allowEnrollCaptureType: (NSString *) captureType
{
    NSMutableArray *skipItems = [[NSMutableArray alloc] initWithArray:[self captureTypeList]];
    NSString *updatedList = nil;
    
    if (skipItems) // Check to see if on list, then remove if found.
    {
        [skipItems removeObject:captureType];
    }
    
    updatedList = [skipItems componentsJoinedByString:@","];
    [[NSUserDefaults standardUserDefaults] setObject:updatedList forKey:SKIPLIST];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

//
// skipEnrollCaptureType
//
// Check to see if captureType is suppressed, and return status.
//
+(BOOL) skipEnrollCaptureType: (NSString *) captureType
{
    NSMutableArray *skipItems = [[NSMutableArray alloc] initWithArray:[self captureTypeList]];
    
    if (skipItems)
    {
        if ([skipItems containsObject:captureType] == YES)
        {
            return YES;
        }
    }
    return NO;
}


+(void) clearEnrollCaptureType
{
    [[NSUserDefaults standardUserDefaults] removeObjectForKey:SKIPLIST];
}
/*
    captureTypeList - returns all types skipped, if not in list not skipping
       Usually not used directly.
 */
+(NSArray *) captureTypeList
{
    NSArray *typeList = nil;
    NSString *skipItems = [[NSUserDefaults standardUserDefaults] objectForKey:SKIPLIST];
    if (skipItems)
    {
        typeList = [skipItems componentsSeparatedByString:@","];
    }
    
    return typeList;
}

//
// Check the person info to determine if a particular capture type is valid.
// Mainly used in the SettingsViewController.
//
+(BOOL) enrolledCaptureType:(NSString *) captureType forPerson:(GMIPerson *)person
{
    if (person)
    {
        if (person.biometricData.count > 0)
        {
            NSDictionary *enrollData = [person.biometricData objectForKey:@"biometricMetadata"];
            NSArray *bioTypes = [enrollData objectForKey:@"enrollmentData"];
            for (NSDictionary *bioData in bioTypes)
            {
                NSNumber *bType = [bioData objectForKey:@"bioType"];
                
                // TODO: Instead of check known values, can get a list of types.
                
                if ( [bType integerValue] == FACE_PIC && [captureType caseInsensitiveCompare:MODALITY_STRING_FACE] == NSOrderedSame)
                {
                    return YES;
                }
                if ( [bType integerValue] == VOICE_PASS_REC && [captureType caseInsensitiveCompare:MODALITY_STRING_PASSPHRASE] == NSOrderedSame)
                {
                    return YES;
                }
                // ======= Add other biometrics types here  ========
                // =============================
            }
        }
    }
    return NO;
}


+(void) setOrientationAll
{
#ifndef ROTATE_PHONE
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        AppDelegate *appDel = (AppDelegate *)[[UIApplication sharedApplication] delegate];
        [appDel fixedOrientation:UIInterfaceOrientationMaskAll];
    }
#else
    AppDelegate *appDel = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    [appDel fixedOrientation:UIInterfaceOrientationMaskAll];
#endif
}

+(void) setOrientationPortrait
{
#ifndef ROTATE_PHONE
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        AppDelegate *appDel = (AppDelegate *)[[UIApplication sharedApplication] delegate];
        [appDel fixedOrientation:UIInterfaceOrientationMaskPortrait];
        [[UIDevice currentDevice] setValue:@(UIInterfaceOrientationPortrait) forKey:@"orientation"];
        [UIViewController attemptRotationToDeviceOrientation];
    }
#else
    AppDelegate *appDel = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    [appDel fixedOrientation:UIInterfaceOrientationMaskPortrait];
    [[UIDevice currentDevice] setValue:@(UIInterfaceOrientationPortrait) forKey:@"orientation"];
    [UIViewController attemptRotationToDeviceOrientation];
#endif
}

#pragma mark - init


-(void) setUserName: (NSString *) tName
{
    [[NSUserDefaults standardUserDefaults] setObject:tName forKey:USER_KEY];
    [[NSUserDefaults standardUserDefaults] synchronize];
    userName = tName;
    if (messageId)
    {
        messageId = nil;   /// Clear from previous display
    }
    onlyOnce = YES; // Allow registering user again.
}

-(void) setCurrentGmiPerson: (GMIPerson *) nowPerson
{
    thisGMIPerson = nowPerson;
}


- (id) initWithCredentials:(NSDictionary *) dictData withUserName: (NSString *) uName{
    if (self = [super init]) {
        serverCredentials = dictData;
        userName = uName;
    }
    return self;
}

-(void) setMessageIdentifier : (NSString *) idString
{
    messageId = idString;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Do any additional setup after loading the view.
    //
    
    //serverCredentials
    NSString *tenant = serverCredentials[TENANT_NAME];          // Tenant name on server
    NSString *clientId = serverCredentials[CLIENT_ID];          // Unique ID
    NSString *clientSecret = serverCredentials[CLIENT_SECRET];  // Secret used for oauth verification
    NSString *appCode = serverCredentials[APP_CODE];            // Set in the admin portal. Default "GoVerifyID"
    NSString *serverName = serverCredentials[SERVER_NAME];  // This normally won't change unless you are using your own server.
    
    NSString *serverGMI = [NSString stringWithFormat:@"%@/gmiserver/", serverName];
    
    //
    // ===== INITALIZE the main library ===== //
    // No network calls are made. Sets up internal data only
    //
    
    [GMIClient startGMI: serverGMI withTenant:tenant withAppCode:appCode withClientCredentials:clientId withSecret:clientSecret];
    [[GMIClient sharedClient] setResponseDelegate:self]; // necessary to process OnMessageResponse.

    onlyOnce = YES;
    userName = nil;
    appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
}

/*
    Enrolls processed on this view presented.  For verifys, disable so that the verify message can be
    added to the end of th4 queue.
 */
- (void)viewDidAppear:(BOOL)animated
{    
    self.navigationController.navigationBarHidden = FALSE;
    if (onlyOnce)
    {
        onlyOnce = NO;
    }
    else  // After template is complete, this view will appear again. OK if processing multiple enrolls.
    {
        return;
    }
    
    if (userName == nil)
    {
        userName = [[NSUserDefaults standardUserDefaults] stringForKey:VALID_USER];
    }
    [self showWaiting];
    
#ifdef AUTOREGISTER
    [self registerUser: userName completion:^(GMIPerson *person) {
        
        NSLog(@"Person: %@", person);
        
        [self hideWaiting];
        
        if (person)
        {
            if (self->disabledEnrollProcessing==FALSE)
            {
                [self checkEnrollsForPerson:person];
            }
        }
        else  // Unable to create user name (bad characters?)
        {
            [[NSUserDefaults standardUserDefaults] removeObjectForKey:VALID_USER];
            [[NSUserDefaults standardUserDefaults] removeObjectForKey:SAMPLEUSER];
            self->onlyOnce = YES;  // Trigger to check enrollment due to user change.

            NSString *uMessage = [NSString stringWithFormat:@"Unable to create '%@'", self->userName];
            UIAlertController *alertController = [UIAlertController
                                                  alertControllerWithTitle:@"Error"
                                                  message: uMessage
                                                  preferredStyle:UIAlertControllerStyleAlert];
            UIAlertAction *okAction = [UIAlertAction
                                       actionWithTitle:@"OK"
                                       style:UIAlertActionStyleDefault
                                       handler:^(UIAlertAction *action)
                                       {
                                           [self dismissViewControllerAnimated:NO completion:nil];
                                       }];
            [alertController addAction:okAction];
            
            [self presentViewController:alertController animated:YES completion:nil];
        }
    }];  // Only need to do once, unless re-enrolling.
#else
    // Check for messages.
    
    if  (thisGMIPerson == nil)  // This should be set by the parent.
    {
        [self dismissViewControllerAnimated:YES completion:nil];
        return;
    }
    

    if (disabledEnrollProcessing==NO)
    {
        // View will appear again after popview controller. SHould only do once.
        [self checkEnrollsForPerson:thisGMIPerson];
    }
    else // Must be verify.
    {
        // Don't do anything after view appears.  The caller will trigger the verificaiton generation.
//        [self dismissViewControllerAnimated:YES completion:nil];
    }

#endif
}

-(void) viewWillDisappear:(BOOL)animated
{
    // Disable navigation when presenting the GMI template.  Will restore when view appears again after template is dismissed
//    self.navigationController.navigationBarHidden = TRUE;
    NSLog(@"view gone");
}

//
// Using performSegueWithIdentifier to navigate out of this view will trigger this method.
// After processing one message, we'll check again for more messages. No polling for messages
// is necessary, since we check each time we exit this view.  Incoming messages when the app is
// active will be processed via the notification routines.  A check for messages is always performed
// when the app becomes active the first time.
//
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    id vc = segue.destinationViewController;
    if ([vc isKindOfClass:[UINavigationController class]])
    {
        // This view is the main operation view after successful signon.
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 2 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
            [self->appDelegate checkGmiMessages];
        });
    }
}



-(void) noEnrollProcessing:(BOOL) disableEnrolls
{
    disabledEnrollProcessing = disableEnrolls;
}

//
// Keep calling this method until all messages processed. Only call after message is completed (onMessageResponse)
// Process in order, since the last object may be a verification message
//
-(BOOL) processGMIMessages
{
    BOOL isMessage = TRUE;
    GMIMessage *gmessage = [gmiMessages firstObject];
    
    if (gmessage)
    {
        
        NSString *templateName = gmessage.templateName;
        
        NSDictionary *enrolldata = [gmessage.metadata objectForKey:GMI_ENROLL_DATA];
        NSString *captureType = [enrolldata objectForKey:GMI_ENROLL_CAPTURE_TYPE];

        if (templateName)  // We can examine the templateName to determine what type of message is processed.
        {
            if ([templateName containsString:@"VERIFY"])
            {
                NSLog(@"Verify message");
            }
        }
        //
        // For debugging or tracking purposes, we can examine the capture type
        //
        if ([captureType isEqualToString:MODALITY_STRING_PALM])
        {
            NSLog(@"Enrolling palm");
        }

        if ([self renderMessageOnThisView:gmessage]) // After template processed, call this method again.
        {
            gmi_message = gmessage;
            //
            // Messages also set the currentMessage, so we can determine the message type when complete
            // when evaluating the message in OnMessageResponse.
            //
            appDelegate.currentMessage = gmessage;
           [gmiMessages removeObjectAtIndex:0];  // Only remove if we are presenting.
        }
    }
    else
    {
        isMessage = FALSE;
    }
    return isMessage;
}

//
//  Registation is performed by examining the current pending messages.
//  The message contain the template name used to register the new user.
//
//  There can be multiple messages (different biotypes), but only one should be processed at a time.
//  Because of the asyncronous template presentation, a copy of the messages are
//  saved to allow sequential processing.
//
//  Users are not required to register all (or any) biometric.  Enroll messages
//  are only removed from the server after successful capture of biometrics. In order to allow skipping a
//  particular biometric type, a local list must be maintained to bypass a particular enrollment.
//  A re-enroll of an existing biometric generates a new enroll message.
//
-(void) getEnrollsForPerson : (GMIPerson *)person success:(void (^)(BOOL enrollsFound)) success
{
    gmiMessages = [[NSMutableArray alloc] initWithCapacity:3];
    enrollCount = 0;
    [[GMIClient sharedClient] getPendingEnrollmentMessagesForPerson:person success:^(NSArray *messages){
        if (messages == nil || messages.count==0)
        {
            // No messages available. No warning necessary
            success(FALSE);
            return;
        }
        for (GMIMessage *gmessage in messages)
        {
            if (gmessage)  // Do all in array
            {
                NSDictionary *enrolldata = [gmessage.metadata objectForKey:GMI_ENROLL_DATA];
                
                NSString *captureType = [enrolldata objectForKey:GMI_ENROLL_CAPTURE_TYPE];
                NSNumber *enrollId = [enrolldata objectForKey:GMI_ENROLL_ID];
                //
                // If we need to restrict/allow certain type of biometrics, we can filter here.
                //
 //               if ([SampleGmiViewController skipEnrollCaptureType:captureType])
 //               {
 //                   self->enrollCount++;  // Fake enroll
 //                   continue;  // Skip this type.
//              }
                if ([captureType isEqualToString:MODALITY_STRING_PASSPHRASE])
                {
                    NSLog(@"Enrolling Voice");
                }
                if ([captureType isEqualToString:MODALITY_STRING_FACE])
                {
                    NSLog(@"Enrolling face");
                }
                if ([captureType isEqualToString:MODALITY_STRING_PALM])
                {
                    NSLog(@"Enrolling palm");
                }

                
                NSLog(@"Enroll URL: %@", gmessage.url);
                NSLog(@"%ld", (long)[enrollId integerValue]);
                
                [self->gmiMessages addObject:gmessage];
            }
        }
        
        if (self->gmiMessages.count)
        {
            // Let caller know to process messages.
            success(TRUE);
        }
        else
        {
            success(FALSE);
        }
        
    } failure:^(GMIError *error){
        if (error)  // Access Error. This can occur if user already created in different tenant.
        {
            NSLog (@"Error: %@", error.description);
        }
        success(FALSE);
    }];
}




//
// Need to track if the device/user has already registered. The gmi method "registerDeviceWithUserId"
// will trigger an email verification if that option is used.  We only need to send on verification.
// If the device is deleted, must register again.
//
-(void) sampleResetUserRegistered
{
    [[NSUserDefaults standardUserDefaults] removeObjectForKey:SAMPLEUSER];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

+(NSString *) sampleGetUserRegistered
{
    NSString *lastUser = [[NSUserDefaults standardUserDefaults] stringForKey:SAMPLEUSER];
    return lastUser;
}

//
// Registration of the device-user is required and should only be done once.  Email vetting is the normal
// process used to verify.  Once registered, we should not register again, otherwise another email will
// be produced.  We track if the user is already registered by checking the value saved in the userDefaults.
// For multiple users, a different approach should be used, possibly using the username as the key.
//
// Alternate registration could be done on the server.
// Verify user, /gmiserver/tenant/delivery/person/validate?userId=hchan&deviceId=xxx&appCode=delivery
// POST /tenant/{tenant_code}/person/validate?userId={userId={user_id}&deviceId={device_id}&appCode={application_code}
//
-(BOOL) sampleUserRegistered
{
    BOOL alreadyRegistered = NO;
    NSString *lastUser = [SampleGmiViewController sampleGetUserRegistered];
    
    if (lastUser)
    {
        if ([lastUser caseInsensitiveCompare:userName] == NSOrderedSame)
        {
            alreadyRegistered = YES;
        }
    }
    
    return alreadyRegistered;
}

-(void) sampleRegisterUser : (NSString *) uName
{
    [[NSUserDefaults standardUserDefaults] setObject:uName forKey:SAMPLEUSER];
    [[NSUserDefaults standardUserDefaults] synchronize];
}



-(void) showWaiting
{
    if (waitingNotice)
    {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{

        CGRect centeredFrame = CGRectMake((self.view.frame.size.width - 100)/2,(self.view.frame.size.height - 50)/2, 100, 50);
        self->waitingNotice = [[UIView alloc] initWithFrame:centeredFrame];
        self->waitingNotice.backgroundColor = [UIColor whiteColor];
        self->waitingNotice.layer.cornerRadius = 10.0;
        
        UILabel *textLabel = [[UILabel alloc] initWithFrame:CGRectMake( 10, 5, 90, 40) ];
        textLabel.text = @"Waiting...";
        [self->waitingNotice addSubview:textLabel];
        
        [self.view addSubview:self->waitingNotice];
    });
}

-(void) hideWaiting
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [self->waitingNotice removeFromSuperview];
        self->waitingNotice = nil;
    });
}


-(void) findUserId : (NSString *) userId success: (void (^)(GMIPerson *))foundPerson
{
    [self setUserName:userId];

    [self view];   // Force viewDidLoad, if not already loaded.
    
    [self noEnrollProcessing:TRUE];

        [[GMIClient sharedClient] getPeopleForThisDeviceSuccess:^(NSArray *arrayOfPeople) {
            GMIPerson *foundMe = nil;
            for (GMIPerson *peep in arrayOfPeople) {
                if ([peep.userId isEqualToString: userId]) {
                    foundMe = peep;
                }
            }
            // if the person isn't tied, we'll create an empty GMIError (but one that isn't nil)
            foundPerson(foundMe);
            
        } failure:^(GMIError *error) {
            foundPerson(nil);
        }];
    
    return;
}

//
// GMI server calls are asyncronous.  Success block called only when user is valid.
//
- (void) checkUserAndDeviceStatus: (NSString *) userId success: (void (^)(void))success
{
    static BOOL waitingForVerify = NO;
    
    if (waitingForVerify)  // Since this method may be a delayed call, keep from getting called multiple times.
    {
        return;
    }
    [self showWaiting];
    waitingForVerify = YES;

    [[GMIClient sharedClient] getGMIPersonByUserId:userId success:^(GMIPerson *person) {
        DBG_Log(@"Person get: %@", [person description]);
        DBG_Log(@"Device belongs to app...");
//        [self setUserName:userId];  // Should already be set.
        
        [[GMIClient sharedClient] getPeopleForThisDeviceSuccess:^(NSArray *arrayOfPeople) {
            bool personIsTied = NO;
            NSLog(@"Number on this device: %lu", (unsigned long)arrayOfPeople.count);
            for (GMIPerson *peep in arrayOfPeople) {
                if ([peep.uuid isEqualToString: person.uuid]) {
                    personIsTied = YES;
                }
                else  // Option. Clean all others that were registered.
                {
//                  [[GMIClient sharedClient] unregisterApplicationForDeviceForPerson:peep success:nil failure:nil];
                }
            }
            
            // if the person isn't tied, we'll create an empty GMIError (but one that isn't nil)
            if (personIsTied) {
                // Valid user.
                [self hideWaiting];
                waitingForVerify = NO;  // Allow this method to be called again.
                success();
            } else {
            // Server will handle verification.
            [self hideWaiting];
            waitingForVerify = NO;
                
            self->appDelegate.thisPerson = person;  // Needed to process enroll messages.

            // Need to associate user with this device.
            [self registerDeviceUsingUserId:userId completion:^(GMIPerson *person){
                
                if (person)
                {
                    NSLog(@"Registration Successful for %@", userId);
                }
                else
                {
                    NSLog (@"Failed to register user: %@", userId);
                }
            
                // checkGmiMessages for enrollments
                
                dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 2 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
                    [self->appDelegate checkGmiMessages];
                });

            }];
                
            success();

            }
        } failure:^(GMIError *error) {
            [self hideWaiting];
            DBG_Log(@"checkUserAndDeviceStatus: getPeopleForThisDevice error: %@", [error description]);
            if (error.code == 404) {
                // Failure, this device is not registered. Can occur if user was deleted.
                [self registerDeviceUsingUserId:userId completion:^(GMIPerson *person){
                    if (person)
                    {
                        // Now we should be able to proceed.
                        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 10 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
                            waitingForVerify = NO;  // Allow this method to be called again.
                            [self checkUserAndDeviceStatus:userId success:success];
                        });
                   }
                    else
                    {
                        // Not good, should have registered.
                        waitingForVerify = NO;  // Allow this method to be called again.
                   }
                }];
                return;
            } else {
                // Likely other problem.  Delete this person, and try again.

                waitingForVerify = NO;  // Allow this method to be called again.
                return;
            }
        }];
        
    } failure:^(GMIError *error) {
        [self hideWaiting];
        //
        // Person not found.
        //
        DBG_Log(@"checkUserAndDeviceStatus: personforuserId error: %@", [error description]);
        if (error.code == 404) {
            // Failure, this device is not registered. Try again later.
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 10 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
                waitingForVerify = NO;  // Allow this method to be called again.
                [self checkUserAndDeviceStatus:userId success:success];
            });
            return;
        } else {
            // Likely network error.
            waitingForVerify = NO;  // Allow this method to be called again.
            return;
        }
    }];
}


-(void) registerDeviceUsingUserId: (NSString *) userId completion:(void (^)(GMIPerson * gmiPerson)) completion
{
    dispatch_async(dispatch_get_main_queue(), ^{
        
        __block GMIPerson *foundPerson;
        __block NSString *validUUID;
        __block NSString *validDeviceId;

        [[GMIClient sharedClient] registerDeviceWithUserId:userId success:^()
         {
             [self sampleRegisterUser:self->userName];
             
             //validDeviceId
             // Need UUID
             [[GMIClient sharedClient] getGMIPersonByUserId:userId success:^(GMIPerson *person) {
                 foundPerson = person;
                 validUUID = person.uuid;
                 [[NSUserDefaults standardUserDefaults] setObject:validUUID forKey:VALID_UUID];
                 
                 validDeviceId = [GMIUtility getUUID];
                 
                 __block GmiServerAccess *gmiServer = [[GmiServerAccess alloc] init];
                 NSDictionary *serverInfo = [GmiServerAccess serverCredentials];
                 
                 [gmiServer setBearerTokenForUser:serverInfo completion:^(NSString *bToken){
                     // Token is set internal.
                 }];
                 
                 //
                 // No Need to pass in token, already set in library.
                 //
                 [gmiServer validatehUserId:userId withDeviceId:validDeviceId completion:nil];
                 
                 if (completion)
                 {
                     completion(foundPerson);
                 }
                 
             } failure:^(GMIError *error){
                 
                 if (completion)
                 {
                     completion(nil);
                 }
                 
             }
              ];
             
         } failure:^(GMIError *error){
             completion(nil);
         }
         ];
    });

}



-(void) checkEnrollsForPerson: (GMIPerson *) person
{
    [self checkUserAndDeviceStatus:userName success:^{
        //
        //  ==== Check for enroll messages ======= //
        //
        if ([[GMIClient sharedClient] isRenderingMessage:self.navigationController])
        {
            return;
        }
        [self getEnrollsForPerson:person success:^(BOOL enrollsFound) {
            if (enrollsFound == NO)
            {
                // Call verify after enroll completed.
                if (self->verifyAfterEnroll && self->verifyComplete)
                {
                    [self completeVerification:self->verifyComplete];
                    self->verifyAfterEnroll = FALSE;
                    return;
                }
                NSString *messId = self->messageId;
                if (messId==nil || messId.length<2)
                  [self dismissViewControllerAnimated:YES completion:nil];
                
                
            }
            else if (self->disabledEnrollProcessing==NO)
            {
                [self processGMIMessages];
            }
        }];
    }];

}


-(void) registerUser: (NSString *) userId completion:(void (^)(GMIPerson * gmiPerson)) completion
{
    
    //
    // Check to see if the user already exist. //
    //

    __block NSString *validUUID = nil;
//    __block NSString *validDeviceId = nil;

    [[GMIClient sharedClient] getGMIPersonByUserId:userId success:^(GMIPerson *person) {
        //
        // Get a list of possible biometric types for this person.
        //
        // If person,algorithms != nil, no need to load.
        //
        
        validUUID = person.uuid;
        
        [[NSUserDefaults standardUserDefaults] setObject:validUUID forKey:VALID_UUID];
        
        // Optional method to get a list of biometrics used for this user.
        [person loadAlgorithms:^(BOOL success){
            //
            NSArray *bioList = [person algorithms];
            if (success)
            {
                [self setUserName:userId];
                [self sampleRegisterUser:userId];
                bioList = [person algorithms];
                for (GMIAlgorithm *aItem in bioList)
                {
                    NSLog(@"%@",aItem.captureType);
                    
                    // TODO: Save each type for reference.
                }
            }
            else // If not successful, possible this user is not associated with this tenant.
            {
                //
                NSLog(@"Unable to find algorithms");
                
                [[NSUserDefaults standardUserDefaults] removeObjectForKey:VALID_UUID];

                //  Server call to validate user.
                __block GmiServerAccess *gmiServer = [[GmiServerAccess alloc] init];
                NSDictionary *serverInfo = [GmiServerAccess serverCredentials];
                
                [gmiServer setBearerTokenForUser:serverInfo completion:^(NSString *bToken){
                
                  }];
                
                [gmiServer associatePerson:person.uuid completion:^(NSString *tokenString){

                    if (tokenString && [self sampleUserRegistered] == NO)  // Only need to register once.
                    {
                        [[GMIClient sharedClient] registerDeviceWithUserId:self->userName success:^()
                         {
                             // registered successful
                             [self sampleRegisterUser:self->userName];
                             
                             // Need UUID
                             [[GMIClient sharedClient] getGMIPersonByUserId:userId success:^(GMIPerson *person) {
                                 validUUID = person.uuid;
                                 [[NSUserDefaults standardUserDefaults] setObject:validUUID forKey:VALID_UUID];
                                 if (completion)
                                 {
                                     completion(person);
                                 }
                                 
                             } failure:^(GMIError *error){
                                 [[NSUserDefaults standardUserDefaults] removeObjectForKey:VALID_UUID];

                                 if (completion)
                                 {
                                     completion(nil);
                                 }
                                 
                             }
                              ];
                             
                         } failure:^(GMIError *error){
                             // Make sure ServerURL, tenant, and appcode are correct.
                             [self sampleResetUserRegistered];
                             if (completion)
                             {
                                 completion(person);
                             }
                         }];
                        return;
                    }
                    else
                    {
                        if (completion)
                        {
                            completion(tokenString?person:nil);
                        }
                    
                    }
                    [self dismissViewControllerAnimated:YES completion:nil];
                }];
                return;
             }
        
        // ========
        // Set up the valid person, so we can process messages.
            
            self->appDelegate.thisPerson = person;
            self->appDelegate.bioForPerson = bioList;  /// of type GMIAlgorithm
            
        //
        //  ==== Check the current biometric data for the person
        //
        //  If the user already exist on gmi and has biometric data, then they likely have already
        //  enrolled.
        //
        BOOL checkEnroll = TRUE; // Disable if any biometric found.
        //
        // Make sure object is correct class to avoid access error
        // person.biometricData could be NSNull class
        //
        if ([person.biometricData isKindOfClass:[NSDictionary class]])
        {
            /* // Sample of biometric metadata.
             {
             biometricMetadata =     {
             enrollmentData =         (
             {
             bioType = 40;
             enrollTimestamp = 1473465775724;
             },
             {
             bioType = 60;
             enrollTimestamp = 1470342131225;
             }
             );
             };
             }
             
             */
            if (person.biometricData.count > 0)
            {
                //
                // Check the current biometric data for this person. We are mainly checking to
                // see if ANY biometric has been enrolled, then setting the flag 'checkEnroll'
                // We can also always check for pending enrollments, which would allow re-enrolls
                //
                NSDictionary *enrollData = [person.biometricData objectForKey:@"biometricMetadata"];
                NSArray *bioTypes = [enrollData objectForKey:@"enrollmentData"];
                for (NSDictionary *bioData in bioTypes)
                {
                    NSNumber *bType = [bioData objectForKey:@"bioType"];
                    // ==== Sample of enrollment date conversion
                    NSNumber *timeStamp = [bioData objectForKey:@"enrollTimestamp"];  // Date created.
                    double doubleTime = [timeStamp doubleValue]/ 1000.0; // Convert milliseconds to seconds.
                    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
                    [dateFormatter setDateFormat:@"yyyy-MM-dd hh:mm:ss"];
                    NSDate *date = [NSDate dateWithTimeIntervalSince1970:doubleTime];
                    NSString *formattedDateString = [dateFormatter stringFromDate:date];
                    
                    // Look at all types of biometric capture types.
                    for (GMIAlgorithm *aItem in bioList)
                    {
                        if ( [bType integerValue] == aItem.bioType)
                        {
                            checkEnroll = FALSE;  // This will allow skipping of pending enroll messages 
                            NSLog(@"Found %@ enrollment. Enrolled on %@",aItem.captureType, formattedDateString);
                        }
                    }
                 }
            }
        }
        
        // Only need to register once, so after proper registration, save the name, so we know not to register again.
        if ([self sampleUserRegistered] == NO)
        {
            //
            // The '2nd factor  type' in the Applications section of the Admin Portal defines two type:
            //  tenant and email
            //
            //  For most applications 'email' is used.  When the device is registered, an email is sent
            //   and is used to verify the user. Registering the device is especially important for push notification.
            //
            //  For 'tenant' your server must make the REST call as follows:
            //  {gmiserver}/gmiserver/tenant/{tenant}/person/validate?userId={userId}&deviceId={deviceId}&appCode={appCode}
            //
            //
            // =====  Register this device ===== //
            //
            // Device registration is required to send secure messages.  This only need to be done once per userName.
            // If email vetting is enabled, the email will be generated from this call.  If you call this method multiple
            // times, an email will be generated each time.
            //
            
            [[GMIClient sharedClient] registerDeviceWithUserId:self->userName success:^()
             {
                 // registered successful
                 [self sampleRegisterUser:self->userName];
                 
                 // Need UUID
                 [[GMIClient sharedClient] getGMIPersonByUserId:userId success:^(GMIPerson *person) {
                     validUUID = person.uuid;
                     [[NSUserDefaults standardUserDefaults] setObject:validUUID forKey:VALID_UUID];
                     if (completion)
                     {
                         completion(person);
                     }

                 } failure:^(GMIError *error){
                    
                     if (completion)
                     {
                         completion(nil);
                     }

                     }
                  ];
                 
             } failure:^(GMIError *error){
                 // Make sure ServerURL, tenant, and appcode are correct.
                 [self sampleResetUserRegistered];
                 if (completion)
                 {
                     completion(person);
                 }
        }];
            
        
        }
        else {  // Already registered
            if (self->disabledEnrollProcessing==NO)
            {
                [self checkEnrollsForPerson:person];
            }
            
            if (completion)
            {
                completion(person);
            }

        }
            
            
        }];
     
    }
     
   failure:^(GMIError *error){
       //
       //  ==== Username not found. Create using server call
       //
       
       //  Server call to validate user.
       __block GmiServerAccess *gmiServer = [[GmiServerAccess alloc] init];
       NSDictionary *serverInfo = [GmiServerAccess serverCredentials];
       
       [gmiServer setBearerTokenForUser:serverInfo completion:^(NSString *bToken){
           // Token is set internal.
       }];
       
       // No Need to pass in token, already set in library.
       //
       [gmiServer createUser:userId completion:^(NSString *tokenString){
           if (completion)
           {
               
               if (tokenString)
               {
                   // User created, now validate.
                   // Now we can call registerDeviceWithUserId again to create enroll messages.
                   
                   [self registerDeviceUsingUserId:userId completion:completion];
               }
           }

       }];
       
   }];
        

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void) showAlertWithTitle : (NSString*) title withMessage: (NSString *) messageText
{
    
    __block UIAlertController *alertController = [UIAlertController
                                          alertControllerWithTitle:title
                                          message: messageText
                                          preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *okAction = [UIAlertAction
                               actionWithTitle:@"OK"
                               style:UIAlertActionStyleDefault
                               handler:^(UIAlertAction *action)
                               {
                                   [self dismissViewControllerAnimated:YES completion:nil];  // Dismiss this view controller.
                                   // For bypassed enrollments, we could restart the enroll process here, it would cause a endless loop that cannot be exited unless
                                   // biometric is enrolled.
                                   alertController = nil;
                               }];
    [alertController addAction:okAction];
    
    [self presentViewController:alertController animated:YES completion:^{
        NSLog(@"Presentation complete");
    }];

}

-(void) enrollComplete:(BOOL) cancelled
{
    if (enrollCount==0) // Check if already enrolled
    {
         if(appDelegate.thisPerson)
         {
             //
             // ==== Check all biotype that are valid.
             //
             if(appDelegate.bioForPerson)
             {
                 for (GMIAlgorithm *aItem in appDelegate.bioForPerson)  // Check for available types.
                 {
 //                    if ([SampleGmiViewController enrolledCaptureType:aItem.captureType forPerson:appDelegate.thisPerson])
                     if (cancelled==FALSE)
                     {
                         enrollCount++;  // Something already enrolled.
                     }
                  }
             }
             else  // data not loaded, just use face & voice.
             {
//                if ([SampleGmiViewController enrolledCaptureType:MODALITY_STRING_FACE forPerson:appDelegate.thisPerson] ||
//                    [SampleGmiViewController enrolledCaptureType:MODALITY_STRING_PASSPHRASE forPerson:appDelegate.thisPerson])
                 {
                     if (cancelled==FALSE)
                         enrollCount++;  // Something already enrolled.
                 }
             }
         }
    }
    if ([SampleGmiViewController fingerprintEnabled])
    {
        // If fingerprint allowed to access, increment enroll counter.
//        enrollCount++;  // Allow no biometrics, since fingerprint detected.
    }
    else  // No fingerprint detected. Must enroll biometric to access.
    {
        if (enrollCount==0)  // Nothing enrolled, reset suppression to force enrollment
        {
            //
            // ==== TODO: Add all biotype that are valid.
            //
            if(appDelegate.bioForPerson)
            {
                for (GMIAlgorithm *aItem in appDelegate.bioForPerson)
                {
//                    [SampleGmiViewController allowEnrollCaptureType:aItem.captureType];
                }
            }
            else // data not loaded, just use face & voice.
            {
//                [SampleGmiViewController allowEnrollCaptureType:MODALITY_STRING_PASSPHRASE];
 //               [SampleGmiViewController allowEnrollCaptureType:MODALITY_STRING_FACE];
            }
        }
        
    }
    
    
    // Call verify after enroll completed.
    if (verifyAfterEnroll && verifyComplete)
    {
        [self completeVerification:verifyComplete];
        verifyAfterEnroll = FALSE;
        return;
    }
    
    NSString *title = (enrollCount>0)?@"Enroll Complete":@"Warning";
    NSString *messageText = (enrollCount>0)?@"You can now use your biometrics to sign on":@"You are required to complete at least one biometric enrollment.";
    
    [self showAlertWithTitle:title withMessage:messageText];
    
}


#pragma mark - pull messaging support


- (BOOL) renderMessageOnThisView: (GMIMessage *) message {
    
    // Present the view using the standard GMI SDK view controller. In the OnMessageResponse, this view
    // will be dismissed.
    //
    if ([[GMIClient sharedClient] isRenderingMessage:self.navigationController])
    {
        return FALSE;
    }
    
    void (^localSuccessBlock)(void) = ^(void) {
        // Called after selection choice (voice)
        NSLog(@"success");
    };

    
    
    void (^failureBlock)(GMIError*) = ^(GMIError *error) {
        NSLog(@"failed");
        [SampleGmiViewController setOrientationAll];
    };

    [SampleGmiViewController setOrientationPortrait];

    [[GMIClient sharedClient] renderMessage:message withNavController:self.navigationController withAnimation:NO withSuccess:localSuccessBlock withFailure:failureBlock];
 
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 10 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        //
        // Wait a few seconds for the global GMIHTMLViewController to initialize, then
        // use method to disable internal time.  If called too early, won't work.
        //
        [GMIHTMLViewController cancelBiometricsAttempt];  // Disable internal 30 second timer.
    });

    NSLog(@"Done presentation");
    
    return TRUE;
}

-(void) skipCurrentEnroll
{
    if (gmi_message)
    {
        NSDictionary *enrolldata = [gmi_message.metadata objectForKey:@"enrollData"];
        
        NSString *captureType = [enrolldata objectForKey:@"captureType"];
        
        if (captureType)
        {
            NSString *message = [NSString stringWithFormat:@"Disabling enroll for %@", captureType];
            NSLog(@"%@", message);
 //           [SampleGmiViewController suppressEnrollCaptureType:captureType];
        }
    }
    gmi_message = nil;
}

#pragma mark - GMIResponseDelegate

/*  === Sample response for different templates:
 
 === Face verify (pulled template)
 Response! : GMIResponse: {
 event = complete;
 extras = "{\"data\":\"completed\"}";
 "is_direct" = yes;
 "person_id" = "c858f4a8-8481-4283-b8a9-e13bda89ba58";
 }
 (
 {
 "bio_event" = Verification;
 modality = face;
 successful = yes;
 }
 )
 
 == Verify fingerprint
 Response! : GMIResponse: {
 event = complete;
 extras = "{\"data\":\"completed\"}";
 "is_direct" = yes;
 "person_id" = "c858f4a8-8481-4283-b8a9-e13bda89ba58";
 }
 (
 {
 "bio_event" = Verification;
 modality = fingerprint;
 successful = yes;
 }
 )
 
 
 === Face enroll
 Response! : GMIResponse: {
 event = complete;
 extras = "{\"data\":\"completed\"}";
 "is_direct" = yes;
 "person_id" = "c858f4a8-8481-4283-b8a9-e13bda89ba58";
 }
 (
 {
 "bio_event" = Enrollment;
 modality = face;
 successful = yes;
 }
 )
 == Enroll face cancelled
 Response! : GMIResponse: {
 event = complete;
 extras = "{\"data\":\"cancelled\"}";
 "is_direct" = yes;
 "person_id" = "c858f4a8-8481-4283-b8a9-e13bda89ba58";
 }
 (
 )
 
 == Login rejected (first view)
 Response! : GMIResponse: {
 event = complete;
 extras = "{\"data\":\"User cancelled verification.\"}";
 "is_direct" = yes;
 "person_id" = "c858f4a8-8481-4283-b8a9-e13bda89ba58";
 }
 (
 )
 
 == Login cancelled in bio capture
 Response! : GMIResponse: {
 event = complete;
 extras = "{\"data\":\"cancelled\"}";
 "is_direct" = yes;
 "person_id" = "c858f4a8-8481-4283-b8a9-e13bda89ba58";
 }
 (
 )
 
 === GMI Message verify face
 Response! : GMIResponse: {
 event = complete;
 extras = "{\"data\":\"completed\"}";
 "is_direct" = no;
 "message_id" = "bea4c013-a1dc-497a-b918-907757c4def1";
 "person_id" = "c858f4a8-8481-4283-b8a9-e13bda89ba58";
 }
 (
 {
 "bio_event" = Verification;
 modality = face;
 successful = yes;
 }
 )
 
 === GMI message verify rejected
 Response! : GMIResponse: {
 event = complete;
 extras = "{\"data\":\"User cancelled verification.\"}";
 "is_direct" = no;
 "message_id" = "b0d1c29b-6f73-4292-95b3-e65a5f61cbfa";
 "person_id" = "c858f4a8-8481-4283-b8a9-e13bda89ba58";
 }
 (
 )
 */

//
// All template responses handle here.  The template
// has not been dismissed when this callback is invoked. Use dispatch_after-dispatch_time to allow the
// presented template to close completely. An alternate method would be to call the next step in the
// viewDidAppear which would occur after the template has been fully dismissed.
//
//  The 'response' may not contain enough information to determine the source of the message.
//  Setting a 'currentMessage' before the template is presented will allow message specifics
//  to be accessed.
//
//  The Sample example processes three types of messages.
//  1. Enroll template (from gmi getPendingEnrollmentMessagesForPerson)
//  2. Gmi message (usually a verify, but can be anything)
//
//  The returned data can be used to determine the source of the message.
//  1. response.data extras - completed or cancelled
//  2. response.data message_id - only valid if message from gmi server (notification)
//  3. response.data person_id - identity of message
//  4. response.biometricEvents - only valid on successful biometric capture (either verify or enroll)
//
// GVID tracks the message by setting the current Message.
//
//  Get the message ID, then use that to get the original message (only for GMI messages)
//
- (void) onMessageResponse: (GMIResponse *) response
{
    [SampleGmiViewController setOrientationAll];
    
    DBG_Log(@"Response! : %@", [response description]);
    NSDictionary *dictData = response.data;
    NSArray *bioData = response.biometricEvents;  // No data if template cancelled or rejected.
    NSString *messageId =  nil;
    NSString *eventType = nil;
    
    if (dictData.count > 0)
    {
        messageId = [dictData objectForKey:gmiresp_MessageId];
    }
    
    //
    //  Evaluate the response to determine if valid or not.
    //   If the verification failed, the template will request a restart, so we
    //   won't get here unless there is a reject or valid response.
    //
    if (dictData.count > 0 && bioData.count > 0)
    {
        // Multiple tries will produce more data. Last object normally the correct one, since template won't finish until valid match.
        NSDictionary *bioDict = [bioData lastObject];
        
        eventType = [bioDict objectForKey:gmiresp_event_BiometricEventType];
        NSString *successful = [bioDict objectForKey:gmiresp_VeriWasSuccessful];
        
        
        if ( [eventType compare: gmiresp_event_Verification] == NSOrderedSame)
        {
            if ([successful compare:gmiresp_Yes] == NSOrderedSame)
            {
                // Verification can be determined here. Server also contains results.
                DBG_Log(@"Good Match");
                gmi_message = nil;
            }
            
            if (self->successBlock)
            {
                self->successBlock(messageId);  // Return the valid messageId, so we can get results from the server.
                self->successBlock = nil;
                self->messageId = nil;
            }
            // Need to notify verification complete.
            [self dismissViewControllerAnimated:YES completion:nil];
            
            // Valid verfication.
        }
        if ( [eventType compare: gmiresp_event_Enrollment] == NSOrderedSame && [successful compare:gmiresp_Yes] == NSOrderedSame)
        {
            DBG_Log(@"Enroll successful");
            gmi_message = nil;
            
            enrollCount++;  // Keep a count of successful biometric enroll. We won't allow bypassing both.
            
            // Is there another enroll message to process?
            dispatch_async(dispatch_get_main_queue(), ^{
                 if ([self processGMIMessages] == FALSE)
                 {
                     [self enrollComplete:FALSE];
                 }
             });
         }
        // else not valid, stay on template.
    }
    else // No bioData, so either cancelled or rejected.
    {
        // Template returned, no data.
        // This could occur if 'Back' button pressed, or validation rejected.
        
        if (dictData.count > 0 )
        {
            NSString *eventString = [dictData objectForKey:gmiresp_Event];
            id extraInfo = [dictData objectForKey:gmiresp_Extras];
            NSString *extraString = nil;
            NSDictionary *extraDictionary = nil;
            if ([extraInfo isKindOfClass:[NSString class]])
            {
                extraString = [dictData objectForKey:gmiresp_Extras];
                if ([extraString hasPrefix:@"{"])
                {
                    NSData *data = [extraString dataUsingEncoding:NSUTF8StringEncoding];
                    id json = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
                    if ([json isKindOfClass:[NSDictionary class]])
                    {
                        extraDictionary = json;
                        NSString *extraMessage = [extraDictionary objectForKey:@"data"];
                        
                        if ([extraMessage containsString:@"cancelled"])
                        {
                            [self skipCurrentEnroll];
                            // Is there another enroll message to process?
                            
                            dispatch_async(dispatch_get_main_queue(), ^{
                                if ([self processGMIMessages] == FALSE)
                                {
                                    [self enrollComplete:TRUE];
                                }
                            });
                            return;
                        }
                    }
                }
            }
            if ([eventString compare:gmiresp_event_MessageAbandoned] == NSOrderedSame)
            {
                if (extraInfo)
                {   // Back button on nav bar
                    DBG_Log(@"Template Abandoned with %@", extraInfo);
                    [self dismissViewControllerAnimated:NO completion:nil];
                    return;
                }
                else
                {
                    DBG_Log(@"Template Rejected");
                }
                [self skipCurrentEnroll];
            }
            //
            //  Complete could occur if user cancelled.
            //
            if ([eventString compare:gmiresp_event_MessageComplete] == NSOrderedSame) // Usually always true
            {
                // Check if extras contains "cancelled"
                if (extraDictionary)
                {
                    extraString = [extraDictionary objectForKey:@"data"];
                    if ([extraString containsString:@"cancelled"])
                    {
                        // User cancelled. Both enroll and verify
                        DBG_Log(@"Template Cancelled");
                        
                        if (appDelegate.currentMessage) // Must be enroll or gmiMessage
                        {
                            [self skipCurrentEnroll];
                            // Check the original message to see if we are enrolling.
                            if ([appDelegate.currentMessage getEnrollmentMetadata])
                            {
                                dispatch_async(dispatch_get_main_queue(), ^{
                                // Try next enrollment.
                                     if ([self processGMIMessages] == FALSE) // We are done, no more enrollments.
                                    {
                                        [self enrollComplete:FALSE];
                                    }
                                });
                            }
                        }
                        
                        return;
                    }
                    else if ([extraString containsString:@"onRequestEnrollment"])
                    {
                            // Need to enroll.
                        self->disabledEnrollProcessing = NO;
                        // No message in queue, load the enroll messages.
                        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 2 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
                            [self checkEnrollsForPerson:self->thisGMIPerson];
                        });
                         return;
                     }

                }
             }
        }
    }
}


#pragma mark - Enroll Verify

//
//  The completion block is saved in the successBlock to be called AFTER the user responses
//  to the verification message in the GMI callback OnMessageResponse.
//

-(void) presentVerification:(void (^)(NSString *returnMessId)) completion
{
    verifyAfterEnroll = TRUE;
    verifyComplete = completion;
}

-(void) completeVerification:(void (^)(NSString *returnMessId)) completion
{
    NSString *uuid = [[NSUserDefaults standardUserDefaults] objectForKey:VALID_UUID];
    
    GmiServerAccess *gmiServer = [[GmiServerAccess alloc] init];
    NSDictionary *serverInfo = [GmiServerAccess serverCredentials];
    __block NSString *bearToken;
    
    [[GMIClient sharedClient] getGMIPersonByUUID:uuid success:^(GMIPerson *person) {
        GMIPerson *thisPerson = person;
                
        [gmiServer setBearerTokenForUser:serverInfo completion:^(NSString *bToken){
            // Token is set internal.
            bearToken = bToken;
        }];
        
        // Before creating verify message, should check for enrolls.
        
        
        
        //
        // No Need to pass in token, already set in library.
        //
        [gmiServer createVerifyMessageForUUID:uuid completion:^(NSString *messId){
            if (messId)
            {
                // messageId will be generated from server call
                
                NSString *thisMessageId = messId;
                
                if (thisMessageId)
                {
                    // Don't callback here, do in OnMessageResponse.
                    if (completion)
                    {
                        self->successBlock = completion;
                    }
                    
                   [self setMessageIdentifier:thisMessageId];
                    //
                    // Re-enroll message must be pulled from the server. Use the messageId to get a reference to the actual message.
                    //
                    [[GMIClient sharedClient] getMessage:thisMessageId forPerson:thisPerson success:^(GMIMessage *message) {
  
                        if (self->gmiMessages==nil)
                        {
                            self->gmiMessages = [[NSMutableArray alloc] initWithCapacity:3];
                        }
                        [self->gmiMessages addObject:message];  // Add to processing queue.
                        
                        
                        dispatch_async(dispatch_get_main_queue(), ^{
                            
                            // Process enrolls/verify message
                            [self processGMIMessages];
                         });
  

                    }  failure: ^(GMIError *error) {
                        DBG_Log(@"Error: %@:", [error localizedDescription]);
                    }
                     ];
                    
                }
                
             }
            else
            {
                self->successBlock = nil;
                if (completion)
                {
                    completion(nil);
                }
            }
        }];
    } failure:^(GMIError *error){
        self->successBlock = nil;
        if (completion)
        {
            completion(nil);
        }

    }
     ];
    

}



@end
