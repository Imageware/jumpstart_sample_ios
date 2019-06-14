//
//  GmiServerAccess.h
//  first_data_jumpstart_sample_ios
//
//  Created by Henry Chan on 2/5/19.
//  Copyright © 2019 ImageWare Systems, Inc. All rights reserved.
//
//  ======= THIS SHOULD NEVER BE DONE IN A MOBILE APP =======
//
#import <Foundation/Foundation.h>


@interface GmiServerAccess : NSObject

+(NSDictionary *) serverCredentials;


-(void) setBearerTokenForUser : (NSDictionary *) userData completion:(void (^)(NSString * tokenString)) completion;
-(void) validatehUserId:(NSString *)personUUID withDeviceId: (NSString *) devId completion:(void (^)(NSString * tokenString)) completion;
-(void) createUser : (NSString *) userId completion:(void (^)(NSString * tokenString)) completion;
-(void) createVerifyMessageForUUID:(NSString *)userId completion:(void (^)(NSString *messageId)) completion;
-(void) getVerificationResponseForUUID:(NSString *)personUUID withMessage:(NSString *)messageId completion:(void (^)(NSString *messageId)) completion;
-(void) associatePerson:(NSString *)personUUID completion:(void (^)(NSString *messageId)) completion;


#define VERIFY_TEMPLATE_NAME  @"UNIVERSAL_VERIFY_VOICE"


@end

