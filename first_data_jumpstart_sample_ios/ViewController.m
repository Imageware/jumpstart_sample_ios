//
//  ViewController.m
//  first_data_jumpstart_sample_ios
//
//  Created by Henry Chan on 1/4/19.
//  Copyright © 2019 ImageWare Systems, Inc. All rights reserved.
//
//
/*
  LOGIC FLOW (first install)

1. UserName entered (Verfiy button off)
2. Check if registered in GMI via SDK call.
3. If registered, goto step 5
4. Not registered. Server call to register user. Followed by validate user call.
5. Check enrollments messages. If none goto step 7
6. Process enroll messages.  Go to step 5.
7. Disable register button, enable verify button
8. Verify. Server call to generate verification message.
9. Present verify message from messageId
10. Display success or failure (from Server REST call using messageId)

 
 
   LOGIC FLOW (restart)
 
 1. User Name filled from last entry
 2. Verify button enabled, registrar button disabled.
 3. Edit of user name disables Verify button (logic continues as first install).
 4. Verify button creates verification message via Server REST call.

*/
#import "ViewController.h"
#import "SampleGmiViewController.h"
#import "GmiServerAccess.h"


#import <GMI/GMI.h>
#import <GMI/GMIInstance.h>

#import "AppDelegate.h"

@interface ViewController ()
{
    SampleGmiViewController *gmiVC;
    BOOL isVerifying;
    GMIPerson *registeredPerson;
}
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.serverData = [SampleGmiViewController clientCredentials];
    
    NSBundle *bundle = [NSBundle bundleWithURL:[[NSBundle mainBundle] URLForResource:@"GMI_Res" withExtension:@"bundle"]];
    NSString *gmiSDKVersion = [NSString stringWithFormat:@"%@.%@",[bundle infoDictionary][@"CFBundleShortVersionString"],[bundle infoDictionary][@"CFBundleVersion"]];
    
    
    NSString * version = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleShortVersionString"];
    NSString * build = [[NSBundle mainBundle] objectForInfoDictionaryKey: (NSString *)kCFBundleVersionKey];
    NSString *serverPath = self.serverData[SERVER_NAME];
    NSLog (@"GMI SDK version: %@", gmiSDKVersion);
    
    _appVersionLabel.text = [NSString stringWithFormat:@"App Version: %@(%@)",version, build];
    _SDKversionLabel.text = [NSString stringWithFormat:@"SDK Version: %@",gmiSDKVersion];
    _GMIserverLabel.text = [NSString stringWithFormat:@"GMI Server: %@",serverPath];

}

-(void) viewDidAppear:(BOOL)animated
{
    //
    // Save and restore value of text field
    //
    BOOL valueChanged = FALSE;
    NSString *savedValue = [[NSUserDefaults standardUserDefaults] stringForKey:VALID_USER];
    
    if (_userIdTextField.text.length > 1 && savedValue)  // CHeck is value has changed.
    {
        if ([savedValue isEqualToString:_userIdTextField.text] == NO)
        {
            valueChanged = TRUE;
        }
    }
    
    _userIdTextField.text = [[NSUserDefaults standardUserDefaults] stringForKey:VALID_USER];
    
    if (_userIdTextField.text.length > 1 && (valueChanged || gmiVC==nil))
    {
        // Process the existing user, since the value was saved.
        gmiVC = [[SampleGmiViewController alloc] initWithCredentials:self.serverData withUserName:_userIdTextField.text];
    }
    
}

-(void) viewWillDisappear:(BOOL)animated
{
    [[NSUserDefaults standardUserDefaults] setObject:_userIdTextField.text forKey:VALID_USER];
}


#pragma mark - UITextFieldDelegate
//
//  From keyboard.
//
- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    
    [_userIdTextField resignFirstResponder];
    
    return YES;
}



- (void)textFieldDidEndEditing:(UITextField *)textField
{
    // Do nothing, must press buttons for action.
}

- (void) processUser:(BOOL ) isRegistering
{
    // Pass the username into the next viewcontroller.
    
    NSString *trimmedString = [_userIdTextField.text stringByTrimmingCharactersInSet:
                               [NSCharacterSet whitespaceAndNewlineCharacterSet]];
    
    [[NSUserDefaults standardUserDefaults] setObject:trimmedString forKey:VALID_USER];

    if (gmiVC == nil)
    {
        gmiVC = [[SampleGmiViewController alloc] initWithCredentials:self.serverData withUserName:trimmedString];
        
   }
    else
    {
        [gmiVC setUserName:trimmedString];
    }
    
    if (isRegistering)// && !isRegistering)
    {
        [gmiVC view];   // Force viewDidLoad.
        
        [gmiVC noEnrollProcessing:TRUE];

        [gmiVC registerUser: trimmedString completion:^(GMIPerson *person) {
            
            NSLog(@"Person: %@", person);
            
            if (person)
            {
                // Valid person, already registered or new user. Save the data for later access.
                self->registeredPerson = person;                
                NSString *uMessage = [NSString stringWithFormat:@"'%@' Registered", trimmedString];
                UIAlertController *alertController = [UIAlertController
                                                      alertControllerWithTitle:@"Success"
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
            else  // Unable to create user name (bad characters?)
            {
                [[NSUserDefaults standardUserDefaults] removeObjectForKey:VALID_USER];
                
                NSString *uMessage = [NSString stringWithFormat:@"Unable to create '%@'", trimmedString];
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

    }
    else  // Process enrolls.
    {
        
        [gmiVC noEnrollProcessing:FALSE];
        
        if (registeredPerson == nil)
        {
            [gmiVC view];   // Force viewDidLoad.

            [gmiVC registerUser: trimmedString completion:^(GMIPerson *person) {
                
                if (person)
                {
                    UINavigationController *navControl = [[UINavigationController alloc] initWithRootViewController:self->gmiVC];
                    
                    self->registeredPerson = person;
                    
                    [self->gmiVC setCurrentGmiPerson: self->registeredPerson];
                    
                    [self presentViewController:navControl animated:YES completion:nil];  // Present to validate email, process enrolls.
                    
                    [self.view setNeedsDisplay];
                }
            }];

        }
        else  // User previously registered in this session.
        {
            if (self->registeredPerson == nil)
            {
                [self showRegisterFirstDialog];
                return;
            }
            UINavigationController *navControl = [[UINavigationController alloc] initWithRootViewController:gmiVC];
            [self->gmiVC setCurrentGmiPerson: self->registeredPerson];

            [self presentViewController:navControl animated:YES completion:nil];  // Present to validate email, process enrolls.
    
            [self.view setNeedsDisplay];
        }
    }

}

//
//   keyboard is active.
//
- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{    
    [SampleGmiViewController clearEnrollCaptureType];  // Allow all enroll capture types, since may be new user.
    
    [[NSUserDefaults standardUserDefaults] removeObjectForKey:VALID_UUID];

   return YES;
}




- (IBAction)registerButtonAction:(id)sender {
    //
    // Check for user, and create if necessary.
    //
    if ([_userIdTextField isFirstResponder])
    {
        [_userIdTextField resignFirstResponder];
    }
    else
    {
        [[NSUserDefaults standardUserDefaults] removeObjectForKey:VALID_UUID];  // Could be new user, wipe out old info.
        [self processUser:YES];  // Registering
    }

}



- (IBAction)enrollButtonAction:(id)sender {
    //
    // Process enroll messages.
    //
    [SampleGmiViewController clearEnrollCaptureType];  // Allow all enroll capture types, since may be new user.

    if ([_userIdTextField isFirstResponder])
    {
        [_userIdTextField resignFirstResponder];
    }
    else
    {
        [self processUser:NO];
    }
}


-(void) showRegisterFirstDialog
{
    UIAlertController *alertController = [UIAlertController
                                          alertControllerWithTitle:@"Register first"
                                          message: @""
                                          preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *okAction = [UIAlertAction
                               actionWithTitle:@"OK"
                               style:UIAlertActionStyleDefault
                               handler:nil];
    [alertController addAction:okAction];
    
    [self presentViewController:alertController animated:YES completion:nil];
    
}

/*
    The verify request is generated from the server, and the message is process here.  The Callback OnMessageResponse
    signals when the user has completed message.  The confirmation is obtained from the server using the messageId.
*/
- (IBAction)verifyButtonAction:(id)sender
{    
    if ([_userIdTextField isFirstResponder])  // Keyboard present?
    {  
        [[NSUserDefaults standardUserDefaults] removeObjectForKey:VALID_UUID];  // Could be new user, wipe out old info.
     }

    NSString *userName = [[NSUserDefaults standardUserDefaults] objectForKey:VALID_USER];

    NSString *uuid = [[NSUserDefaults standardUserDefaults] objectForKey:VALID_UUID];
    
    //
    // Before processing the verification message, we should check to see if there are pending enrollments.
    // Re-enroll will create a new enroll message that should be processed before a verification.
    //
    // Since all messages are processed in one routine, add the vericatioh message to the end of
    // the message queue will allow enroll messages to be processed first.

    if (gmiVC == nil)
    {
        gmiVC = [[SampleGmiViewController alloc] initWithCredentials:self.serverData withUserName:userName];
    }
    
    
    if (uuid==nil || registeredPerson == nil)
    {
        // Check if registered first.
        [gmiVC findUserId : userName success:^(GMIPerson *person){
            if (person)
            {
                // Already registered.
                [[NSUserDefaults standardUserDefaults] setObject:person.userId forKey:VALID_USER];
                [[NSUserDefaults standardUserDefaults] setObject:person.uuid forKey:VALID_UUID];
                self->registeredPerson = person;
                [self verifyButtonAction:nil];
            }
            else {
                [self showRegisterFirstDialog];
            }
        }];
        
        return;
    }
    
    [gmiVC noEnrollProcessing:TRUE];
    [gmiVC setCurrentGmiPerson: registeredPerson];

    
    UINavigationController *navControl = [[UINavigationController alloc] initWithRootViewController:gmiVC];
    
    [self presentViewController:navControl animated:YES completion:^{
        //
        // Server call to generate verification message.  This will add the message to the processing queue.
        //

        [self->gmiVC presentVerification:^(NSString *messageId){  // returns a valid messageId
            
            // Need to process after messages processed.
            if (messageId)
            {
                // Verification message displayed.

                //  Server call to validate user.
                __block GmiServerAccess *gmiServer = [[GmiServerAccess alloc] init];
                NSDictionary *serverInfo = [GmiServerAccess serverCredentials];
                
                [gmiServer setBearerTokenForUser:serverInfo completion:^(NSString *bToken){
                    // Token is set internal.
                }];
                
                // Make sure this is the correct UUID
                NSString *uuid = [[NSUserDefaults standardUserDefaults] objectForKey:VALID_UUID];
                __block NSString *localMessageId = messageId;

                // No Need to pass in token, already set in library.
                //
                [gmiServer getVerificationResponseForUUID:uuid withMessage:localMessageId completion:^(NSString *messageId) {
                    dispatch_async(dispatch_get_main_queue(), ^{

                    NSString *alertTitle = nil;
                    
                    if (messageId)
                    {
                        NSLog(@"Valid: %@", messageId);
                        alertTitle = @"Successful verification";
                    }
                    else
                    {
                        alertTitle = @"Failed to verify";
                        NSLog (@"Failed with uuid: %@", uuid);
                    }
                    
                    UIAlertController *alertController = [UIAlertController
                                                          alertControllerWithTitle:alertTitle
                                                          message: @""
                                                          preferredStyle:UIAlertControllerStyleAlert];
                    UIAlertAction *okAction = [UIAlertAction
                                               actionWithTitle:@"OK"
                                               style:UIAlertActionStyleDefault
                                               handler:^(UIAlertAction *action)
                                               {
                                                   [[self navigationController] popViewControllerAnimated:YES]; // UIAlertAction
                                               }];
                    [alertController addAction:okAction];
                    
                    [self presentViewController:alertController animated:YES completion:nil];

                    
                    });
                    
                }];
 
                
            }
            else
            {
                dispatch_async(dispatch_get_main_queue(), ^{
               // User cancelled, no response message to obtain.
                UIAlertController *alertController = [UIAlertController
                                                      alertControllerWithTitle:@"Aborted"
                                                      message: @""
                                                      preferredStyle:UIAlertControllerStyleAlert];
                UIAlertAction *okAction = [UIAlertAction
                                           actionWithTitle:@"OK"
                                           style:UIAlertActionStyleDefault
                                           handler:^(UIAlertAction *action)
                                           {
                                               [[self navigationController] popViewControllerAnimated:YES]; // UIAlertAction
                                           }];
                [alertController addAction:okAction];
                
                [self presentViewController:alertController animated:YES completion:nil];
                });
                
            }
        }];
    }];

}

@end
