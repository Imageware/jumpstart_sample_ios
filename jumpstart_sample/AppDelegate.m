//
//  AppDelegate.m
//  jumpstart_sample
//
//  Created by Henry Chan on 1/4/19.
//  Copyright © 2019 ImageWare Systems, Inc. All rights reserved.
//

#import "AppDelegate.h"
#import "SampleGmiViewController.h"
#import <GMI/GMILocalizer.h> // For SDK version 3.9.2+

@interface AppDelegate ()
{
    UIInterfaceOrientationMask currentMask;  // Used to determine orientation
}

@end

@implementation AppDelegate

//
//  GMI.framework image capture using the device camera only works in Portrait.  We should force Portrait mode before
//  presenting verification or registration messages.
//

-(void) fixedOrientation: (UIInterfaceOrientationMask) orientationMask
{
    currentMask = orientationMask;
}

//
// This will override any project and view settings.
// =============================================================================================================
// === IMPORTANT == will only work if the box "Requires full screen" in the General Targets setting is checked
// =============================================================================================================
// https://stackoverflow.com/questions/32621014/supportedinterfaceorientationsforwindow-not-called-in-ios-9-appdelegate
//
- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
{
    if (currentMask == UIInterfaceOrientationUnknown)  // First time, default (zero)
    {
        // For iPhone, if the app is restricted to portrait, we leave as default.
        // Don't change the currentMask from UIInterfaceOrientationUnknown
#ifndef ROTATE_PHONE
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
        {
            currentMask = UIInterfaceOrientationMaskAll;
        }
        else
        {
            currentMask = UIInterfaceOrientationMaskPortrait;
        }
#else
        currentMask = UIInterfaceOrientationMaskAll;
#endif
    }
    return currentMask;
}


//
// The check for messages should be called only after startGMI call.  Most of the GMI functionality is contained
// in the SampleGmiViewController.  Since the SampleGmiViewController view is not loaded initially, we won't check for
// messages until it is.  GMI messages are presented via a GMI method.
//
-(void) checkGmiMessages
{
    //
    // Once GMI has established a user and device, the username is saved.
    // The 'thisPerson' is set once gmi is fully initialized.
    //
    NSString *lastUser = [SampleGmiViewController sampleGetUserRegistered];
    
    if (lastUser==nil || self.thisPerson == nil)
    {
        return;
    }
    // The [GMIClient sharedClient] is initialized in the SampleGmiViewController.
    // The thisPerson is set there. We can only present messages after GMI has been initialized.
    
    //
    // Get the current navigation controller, we need it to present the message.
    // To properly evaluate the results, the onMessageResponse delegate should be setup for the navControl
    //
    //
    UIViewController *rooted = [UIApplication sharedApplication].keyWindow.rootViewController;
    UINavigationController *navControl;
    
    if ([rooted.presentedViewController isKindOfClass:[UINavigationController class]])
    {
        navControl = (UINavigationController *) rooted.presentedViewController;  // This works using bio login
    }
    else if ([rooted isKindOfClass:[UINavigationController class]])
    {
        navControl = (UINavigationController *)rooted; // This case for password login. Won't work cuz gmiviewcontroller not in heirarchy
    }
    else
    {
        navControl = rooted.presentingViewController.navigationController;
    }
    
    if (navControl)
    {
        // After processing the message, the onMessageResponse within the SampleGmiViewController will be called, since the
        // gmiClient.delegate was set when initialized.
        //
        [[GMIClient sharedClient] getFirstMessageForPerson: self.thisPerson success:^(GMIMessage *message) {
            //
            // Save a reference to the message, so we can access in onMessageResponse (SampleGmiViewController) to mark as read.
            //
            self.currentMessage = message;
            
            [SampleGmiViewController setOrientationPortrait];
            
            [[GMIClient sharedClient] renderMessage:message withNavController: navControl withAnimation:YES withSuccess:^{
                DBG_Log(@"OK");  // Message loaded properly.
                // Once the message has been completed, currentMessage is reset in the onMessageResponse
            } withFailure:^(GMIError *error) {
                self.currentMessage = nil;
                DBG_Log(@"Couldn't render message: %@", [error description]);
            }];
            
        }  failure: ^(GMIError *error) {
            DBG_Log(@"Error: %@:", [error localizedDescription]);
        }
         ];
    }
}

//
// For verification, a message is genereated on the server, and the GMI messageId is returned
// to this app for processing.
//
-(void) presentGmiMessageId: (NSString *) messageId
{
    //
    // Once GMI has established a user and device, the username is saved.
    // The 'thisPerson' is set once gmi is fully initialized.
    //
    NSString *lastUser = [SampleGmiViewController sampleGetUserRegistered];
    
    if (lastUser==nil || self.thisPerson == nil)
    {
        return;
    }
    // The sharedClient is initialized in the SampleGmiViewController.
    // The thisPerson is set there. We can only present messages after GMI has been initialized.
    
    //
    // Get the current navigation controller, we need it to present the message.
    // To properly evaluate the results, the onMessageResponse delegate should be setup for the navControl
    //
    //
    UIViewController *rooted = [UIApplication sharedApplication].keyWindow.rootViewController;
    UINavigationController *navControl;
    
    if ([rooted.presentedViewController isKindOfClass:[UINavigationController class]])
    {
        navControl = (UINavigationController *) rooted.presentedViewController;  // This works using bio login
    }
    else if ([rooted isKindOfClass:[UINavigationController class]])
    {
        navControl = (UINavigationController *)rooted; // This case for password login. Won't work cuz gmiviewcontroller not in heirarchy
    }
    else
    {
        navControl= rooted.presentingViewController.navigationController;
    }
    
    if (navControl)
    {
        // After processing the message, the onMessageResponse within the SampleGmiViewController will be called, since the
        // gmiClient.delegate was set when initialized.
        //
        [[GMIClient sharedClient] getMessage:messageId forPerson:self.thisPerson success:^(GMIMessage *message) {
            //
            // Save a reference to the message, so we can access in onMessageResponse (SampleGmiViewController) to mark as read.
            //
            self.currentMessage = message;
            
            [SampleGmiViewController setOrientationPortrait];
            
            [[GMIClient sharedClient] renderMessage:message withNavController: navControl withAnimation:YES withSuccess:^{
                DBG_Log(@"OK");  // Message loaded properly.
                // Once the message has been completed, currentMessage is reset in the onMessageResponse
            } withFailure:^(GMIError *error) {
                self.currentMessage = nil;
                DBG_Log(@"Couldn't render message: %@", [error description]);
            }];
            
        }  failure: ^(GMIError *error) {
            DBG_Log(@"Error: %@:", [error localizedDescription]);
        }
         ];
    }
}



- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
    [GMILocalizer useBundle:[NSBundle mainBundle]]; // Necessary if the SDK bundle was modified. // For SDK version 3.9.2+
            
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.

}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}



@end
