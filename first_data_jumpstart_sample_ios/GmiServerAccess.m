//
//  GmiServerAccess.m
//  first_data_jumpstart_sample_ios
//
//  Created by Henry Chan on 2/5/19.
//  Copyright © 2019 ImageWare Systems, Inc. All rights reserved.
//
//
//  ======= THIS SHOULD NEVER BE DONE IN A MOBILE APP =======
//

#import "GmiServerAccess.h"
#import "SampleGmiViewController.h"


//
// Reference values from Admin Portal settings.
//
#define SCOPE_TENANT  @"SCOPE_GMI_USER"

@interface GmiServerAccess ()
{
    __strong NSString *bearerToken;
    NSDictionary *serverData;
}
@end


@implementation GmiServerAccess



//
// ============= GMI server data =========
//
+(NSDictionary *) serverCredentials
{
    NSDictionary *serverDict = @{
                                 TENANT_NAME : @"fdata",
                                 CLIENT_ID : @"fdadmin",
                                 CLIENT_SECRET : @"p3AQlzCdJDIt",
                                 APP_CODE : @"authenticare",
                                 SERVER_NAME : @"https://deployment-gmi.iwsinc.com",  //"https://deployment-gmi.iwsinc.com"
                                 CODED_64 :  @"ZmRhZG1pbjpwM0FRbHpDZEpESXQ="  // clientID:ClientSecret ===> 64baseEncodedValue
                                 };
    
    return serverDict;
}



/*
 The sample HTTP methods below are slightly modified from the Postman generated code. Main changes are the custom specific values are parameters.
 */
-(void) setBearerTokenForUser : (NSDictionary *) userData completion:(void (^)(NSString * tokenString)) completion
{
    if (self->bearerToken)
    {
        completion(self->bearerToken);
        return;
    }
    serverData = userData;
    NSString *secret64 = userData[CODED_64];
    if (secret64==nil)  // If no value, manually encode.
    {
        NSString *encodeValue = [NSString stringWithFormat:@"%@:%@", userData[CLIENT_ID], userData[CLIENT_SECRET]];
        NSData *dataValue = [encodeValue dataUsingEncoding:NSUTF8StringEncoding];
        secret64 = [dataValue base64EncodedStringWithOptions:NSDataBase64Encoding64CharacterLineLength];
    }
    NSString *serverURL = userData[SERVER_NAME];
    NSString *authorizationString = [NSString stringWithFormat:@"Basic %@", secret64];
    NSDictionary *headers = @{ @"authorization": authorizationString,
                               @"content-type": @"application/x-www-form-urlencoded",
                               @"cache-control": @"no-cache" };
    
    NSString *bearerTokenRequest = [NSString stringWithFormat:@"%@/usermanager/oauth/token?scope=%@&grant_type=client_credentials", serverURL, SCOPE_TENANT];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:bearerTokenRequest]
                                                           cachePolicy:NSURLRequestUseProtocolCachePolicy
                                                       timeoutInterval:10.0];
    [request setHTTPMethod:@"POST"];
    [request setAllHTTPHeaderFields:headers];
    
    dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
    
    NSURLSession *session = [NSURLSession sharedSession];
    NSURLSessionDataTask *dataTask = [session dataTaskWithRequest:request
                                                completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
                                                    NSString *bearToken = nil;
                                                    if (error) {
                                                        NSLog(@"%@", error);
                                                        completion(bearToken);
                                                    } else {
                                                        NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *) response;
                                                        NSError *error = nil;
                                                        NSDictionary *jsonDict = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&error];
                                                        
                                                        if (error != nil) {
                                                            NSLog(@"Error parsing JSON.");
                                                        }
                                                        else {
                                                            NSLog(@"Dict: %@", jsonDict);
                                                            bearToken = [jsonDict valueForKey:@"access_token"];
                                                        }
                                                        NSLog(@"%@", httpResponse);
                                                    }
                                                    self->bearerToken = bearToken;
                                                    dispatch_semaphore_signal(semaphore);       // signal message received
                                                    completion(bearToken);
                                                    
                                                }];
    [dataTask resume];
    
    dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
    
}

//
/*
 THIS SHOULD NEVER BE DONE IN A MOBILE APP
 
 This method is only for demo purposes.  Usually this call is performed on the server.
 The message from the server is sent to the app as a message.
 
 //  POST /tenant/{tenant_code}/app/{app_code}/template/{template_code}/person/{person_uuid}/message
 //  Requires ROLE_TENANT_ADMIN privileges or higher.
 //  For 'tenant' your server must make the REST call as follows:
 
 //  {gmiserver}/gmiserver/tenant/{tenant}/person/validate?userId={userId}&deviceId={deviceId}&appCode={appCode}
 //  Requires ROLE_TENANT_ADMIN privileges or higher.
 
 */
-(void) validatehUserId:(NSString *)userId withDeviceId: (NSString *) devId completion:(void (^)(NSString * tokenString)) completion
{
    NSString *serverURL = serverData[SERVER_NAME];
    NSString *tenantName = serverData[TENANT_NAME];
    NSString *appCode = serverData[APP_CODE];
    NSString *authValue = [NSString stringWithFormat:@"Bearer %@",self->bearerToken];
    NSDictionary *headers = @{ @"authorization": authValue,
                               @"cache-control": @"no-cache" };
    
    // Encode, so special characters can be sent
    NSString *encodeUserId = [userId stringByAddingPercentEncodingWithAllowedCharacters:[[NSCharacterSet characterSetWithCharactersInString:@"/+=\n"] invertedSet]];
    NSString *gmiServerRequest = [NSString stringWithFormat:@"%@/gmiserver/tenant/%@/person/validate?userId=%@&deviceId=%@&appCode=%@",
                                  serverURL, tenantName, encodeUserId, devId, appCode];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:gmiServerRequest]
                                                           cachePolicy:NSURLRequestUseProtocolCachePolicy
                                                       timeoutInterval:10.0];
    
    [request setHTTPMethod:@"POST"];
    [request setAllHTTPHeaderFields:headers];
    
    NSURLSession *session = [NSURLSession sharedSession];
    NSURLSessionDataTask *dataTask = [session dataTaskWithRequest:request
                                                completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
                                                    if (error) {
                                                        NSLog(@"%@", error);
                                                    } else {
                                                        NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *) response;
                                                        NSLog(@"%@", httpResponse);
                                                        
                                                        NSError *error = nil;
                                                        NSDictionary *jsonDict = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&error];
                                                        
                                                        if (error != nil) {
                                                            NSLog(@"Error parsing JSON.");
                                                        }
                                                        else {
                                                            NSLog(@"Dict: %@", jsonDict);
                                                            
                                                        }
                                                        // Done. exit?
                                                        if(completion)
                                                        {
                                                            completion(userId);  // TODO: Get uuid from json?
                                                        }
                                                    }
                                                }];
    [dataTask resume];
}




-(void) createUser: (NSString *) userId completion:(void (^)(NSString * tokenString)) completion
{
    NSString *serverURL = serverData[SERVER_NAME];
    NSString *tenantName = serverData[TENANT_NAME];
//    NSString *appCode = serverData[APP_CODE];
    NSString *authValue = [NSString stringWithFormat:@"Bearer %@",self->bearerToken];
    NSDictionary *headers = @{ @"authorization": authValue,
                               @"Content-Type": @"application/json",
                               @"cache-control": @"no-cache" };

    NSDictionary *parameters = @{ @"userId": userId
                              };
    
    NSData *postData = [NSJSONSerialization dataWithJSONObject:parameters options:0 error:nil];
    
    NSString *gmiServerRequest = [NSString stringWithFormat:@"%@/gmiserver/tenant/%@/person",
                                  serverURL, tenantName];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:gmiServerRequest]
                                                           cachePolicy:NSURLRequestUseProtocolCachePolicy
                                                       timeoutInterval:10.0];
    
    [request setHTTPMethod:@"POST"];
    [request setAllHTTPHeaderFields:headers];
    [request setHTTPBody:postData];

    NSURLSession *session = [NSURLSession sharedSession];
    NSURLSessionDataTask *dataTask = [session dataTaskWithRequest:request
                                                completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
                                                    if (error) {
                                                        NSLog(@"%@", error);
                                                    } else {
                                                        NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *) response;
                                                        NSLog(@"%@", httpResponse);
                                                        
                                                        NSError *error = nil;
                                                        NSDictionary *jsonDict = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&error];
                                                        
                                                        if (error != nil) {
                                                            NSLog(@"Error parsing JSON.");
                                                        }
                                                        else {
                                                            NSLog(@"Dict: %@", jsonDict);
                                                            
                                                        }
                                                        
                                                        NSString *gmi_id = nil;
                                                        
                                                        if (jsonDict)
                                                        {
                                                            gmi_id = jsonDict[@"id"];
                                                        }
                                                        
                                                        // Need UUID to validate.
                                                        
                                                        if (gmi_id)
                                                        {
                                                            // Need to register device with uuid.
 
                                                        }
                                                        // Done. exit?
                                                        if(completion)
                                                        {
                                                            completion(gmi_id);  // uuid used to register device.
                                                        }
                                                    }
                                                }];
    [dataTask resume];
}



-(void) createVerifyMessageForUUID:(NSString *)personUUID  completion:(void (^)(NSString *messageId)) completion
{
    NSString *serverURL = serverData[SERVER_NAME];
    NSString *tenantName = serverData[TENANT_NAME];
    NSString *appCode = serverData[APP_CODE];
    NSString *templateName = VERIFY_TEMPLATE_NAME;
    NSString *authValue = [NSString stringWithFormat:@"Bearer %@", self->bearerToken];
    NSDictionary *headers = @{ @"authorization": authValue,
                               @"Content-Type": @"application/json",
                               @"cache-control": @"no-cache" };
    
    NSString *templatePath = [NSString stringWithFormat:@"%@/gmiserver/tenant/%@/app/%@/template/%@", serverURL, tenantName, appCode, templateName];
                              
    NSDictionary *parameters = @{ @"maxResponseAttempts": @3,
                                  @"postbackUrl": @"",
                                  @"template": templatePath,
                                  @"metadata": @{ @"reason": @"Verification request." },
                                  /*@"expiresIn": @240 */ };

    NSData *postData = [NSJSONSerialization dataWithJSONObject:parameters options:0 error:nil];

    
    NSString *gmiServerRequest = [NSString stringWithFormat:@"%@/person/%@/message",
                                  templatePath, personUUID];

    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:gmiServerRequest]
                                                           cachePolicy:NSURLRequestUseProtocolCachePolicy
                                                       timeoutInterval:10.0];
    
    
    [request setHTTPMethod:@"POST"];
    [request setAllHTTPHeaderFields:headers];
    [request setHTTPBody:postData];

    NSURLSession *session = [NSURLSession sharedSession];
    NSURLSessionDataTask *dataTask = [session dataTaskWithRequest:request
                                                completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
                                                    if (error) {
                                                        NSLog(@"%@", error);
                                                        if(completion)
                                                        {
                                                            completion(nil);
                                                        }
                                                   } else {
                                                        NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *) response;
                                                        NSLog(@"%@", httpResponse);
                                                        
                                                        NSError *localError = nil;
                                                       NSDictionary *jsonDict = nil;
                                                       id jsonObject = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&localError];
                                                       
                                                       if ([jsonObject isKindOfClass:[NSArray class]]) {
                                                           NSArray *jsonArray = (NSArray *)jsonObject;
                                                           jsonDict = [ jsonArray firstObject];
                                                       }
                                                       else
                                                       {
                                                          jsonDict = (NSDictionary *)jsonObject;
                                                       }

                                                        NSString *messageId;
                                                       
                                                        if (error != nil) {
                                                            NSLog(@"Error parsing JSON.");
                                                        }
                                                        else {
                                                            NSLog(@"Dict: %@", jsonDict);
                                                            messageId = [jsonDict objectForKey:@"messageId"];
                                                        }
                                                        // Done. exit?
                                                        if(completion)
                                                        {
                                                            completion(messageId);  // TODO: Get uuid from json?
                                                        }
                                                    }
                                                }];
    [dataTask resume];
}


/*
 List of tranaction Id, match with messageID to get response.
 
 GET /tenant/{tenant_code}/person/{person_uuid}/message /response

*/
-(void) getVerificationResponseForUUID:(NSString *)personUUID withMessage:(NSString *)messageId completion:(void (^)(NSString *messageId)) completion
{
    NSString *serverURL = serverData[SERVER_NAME];
    NSString *tenantName = serverData[TENANT_NAME];
    NSString *authValue = [NSString stringWithFormat:@"Bearer %@", self->bearerToken];
    NSDictionary *headers = @{ @"authorization": authValue,
                               @"Content-Type": @"application/json",
                               @"cache-control": @"no-cache" };
    
    NSString *gmiServerRequest = [NSString stringWithFormat:@"%@/gmiserver/tenant/%@/person/%@/message/%@/response", serverURL, tenantName, personUUID, messageId];
//    NSString *gmiServerRequest = [NSString stringWithFormat:@"%@/gmiserver/tenant/%@/person/%@/message/response", serverURL, tenantName, personUUID];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:gmiServerRequest]
                                                           cachePolicy:NSURLRequestUseProtocolCachePolicy
                                                       timeoutInterval:10.0];
    
    
    [request setHTTPMethod:@"GET"];
    [request setAllHTTPHeaderFields:headers];
    
    NSURLSession *session = [NSURLSession sharedSession];
    NSURLSessionDataTask *dataTask = [session dataTaskWithRequest:request
                                                completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
                                                    if (error) {
                                                        NSLog(@"%@", error);
                                                        if(completion)
                                                        {
                                                            completion(nil);
                                                        }
                                                    } else {
                                                        NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *) response;
                                                        NSLog(@"%@", httpResponse);
                                                        
                                                        NSError *localError = nil;
                                                        // Array of all transactions. Match with original messageID to find correct entry.
                                                        id jsonObject = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&localError];
                                                        
                                                        NSArray *jsonArray = nil;
                                                        
                                                        if ([jsonObject isKindOfClass:[NSArray class]]) {
                                                            jsonArray = (NSArray *)jsonObject;
                                                        }
                                                        else
                                                        {
                                                            NSDictionary *jsonDictionary = (NSDictionary *)jsonObject;
                                                            jsonArray = @[jsonDictionary];
                                                        }
                                                        BOOL foundMatch = false;
                                                        
                                                        for (NSDictionary *xactDict in jsonArray)
                                                        {
                                                            NSString *messy = xactDict[@"messageUuid"];
                                                            NSNumber *success = xactDict[@"succeeded"];
                                                            
                                                            if (messy && [messy isEqualToString: messageId] == TRUE)
                                                            {
                                                                if ([success integerValue]>0)
                                                                {
                                                                    if (completion)
                                                                    {
                                                                        completion(messy);
                                                                        foundMatch = true;
                                                                        break;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        
                                                        if( foundMatch == false)
                                                        {
                                                            if (completion)
                                                            {
                                                                completion(nil);
                                                            }

                                                        }
                                                        
                                                        
                                                        if (error != nil) {
                                                            NSLog(@"Error parsing JSON.");
                                                        }
                                                        else {
                                                            NSLog(@"jObj: %@", jsonArray);
                                                        }
                                                        // Done. exit?
  
                                                    }
                                                }];
    [dataTask resume];
}


-(void) associatePerson:(NSString *)personUUID completion:(void (^)(NSString *messageId)) completion
{
    NSString *serverURL = serverData[SERVER_NAME];
    NSString *tenantName = serverData[TENANT_NAME];
    NSString *authValue = [NSString stringWithFormat:@"Bearer %@", self->bearerToken];
    NSDictionary *headers = @{ @"authorization": authValue,
                               @"Content-Type": @"application/json",
                               @"cache-control": @"no-cache" };
 
    NSString *gmiServerRequest = [NSString stringWithFormat:@"%@/gmiserver/tenant/%@/person/%@", serverURL, tenantName, personUUID];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:gmiServerRequest]
                                                           cachePolicy:NSURLRequestUseProtocolCachePolicy
                                                       timeoutInterval:10.0];
    
    
    [request setHTTPMethod:@"PUT"];
    [request setAllHTTPHeaderFields:headers];
    
    NSURLSession *session = [NSURLSession sharedSession];
    NSURLSessionDataTask *dataTask = [session dataTaskWithRequest:request
                                                completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
                                                    if (error) {
                                                        NSLog(@"%@", error);
                                                        if(completion)
                                                        {
                                                            completion(nil);
                                                        }
                                                    } else {
                                                        NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *) response;
                                                        NSLog(@"%@", httpResponse);
                                                        long statusCode = [httpResponse statusCode];
                                                        
                                                        if( statusCode == 200 || statusCode == 201)
                                                        {
                                                            if (completion)
                                                            {
                                                                completion(@"success");
                                                            }
                                                        }
                                                        else if (completion)
                                                        {
                                                            completion(nil);
                                                        }

                                                        
                                                    }
                                                }];
    [dataTask resume];
    
}

@end
