//
//  GMIDialogManager.h
//  GMI
//
//  Created by ddevoe on 10/26/17.
//  Copyright © 2017 ImageWare Systems Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GMIClient.h"

@class UINavigationController;
@class GMIMessage;

@interface GMIDialogManager : NSObject


- (instancetype) initWithNav:(UINavigationController *)navigator;

- (void) dismissAny;

//
// For a list of the look and meaning of each of these dialogs refere to
// 11.0_Polish3.5 Build_Dialogs listed in Numeric order
//

- (void) displayD_16:( void (^)(NSInteger button) ) completion;
- (void) displayD_17:( void (^)(NSInteger button) ) completion;
- (void) displayD_20:( void (^)(NSInteger button) ) completion;
- (void) displayD_40:( void (^)(NSInteger button) ) completion;
- (void) displayD_45:( void (^)(NSInteger button) ) completion;
- (void) displayD_47:( void (^)(NSInteger button) ) completion;
- (void) displayD_51:( void (^)(NSInteger button) ) completion;
- (void) displayD_68WithUserID:(NSString*)userID withCompletion:( void (^)(NSInteger button) ) completion;
- (void) displayD_62:( void (^)(NSInteger button) ) completion;
- (void) displayD_65:( void (^)(NSInteger button) ) completion;
- (void) displaySparseD_65:( void (^)(NSInteger button) ) completion;
- (void) displayD_70:( void (^)(NSInteger button) ) completion;
- (void) displayD_73:( void (^)(NSInteger button) ) completion;

- (void) displayD_83:( void (^)(NSInteger button) ) completion;

- (void) displayD_86:( NSString *)title completion:( void (^)(NSInteger button) ) completion;
- (void) displayD_90:( void (^)(NSInteger button) ) completion;

//
// This method will remove any of the dialog boxes presented above
//
- (void) dismissAny;

+ (void) palmSelection:(NSArray<GMIMessage *> *)Msgs nav:(UINavigationController *)navController completion:(void (^)(GMIMessage *msg)) block;
+ (GMIMessage *) createPalm:(NSInteger)bioType withMsg:(GMIMessage *)msg;

@end
