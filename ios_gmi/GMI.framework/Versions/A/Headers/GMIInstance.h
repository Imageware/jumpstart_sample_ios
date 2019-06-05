//
//  GMIInstance.h
//  GMI
//
//  Created by ddevoe on 11/15/17.
//  Copyright © 2017 ImageWare Systems Inc. All rights reserved.
//
#import <Foundation/Foundation.h>
@class GMIPerson;
@class GMIMessage;

@interface GMIInstance : NSObject

+ (instancetype)shared;

+(NSString*)frameworkVersion;

- (void) removePerson:(NSString *)uuid ;

- (NSArray<NSString *> *)enrolledCaptureTypesFor:(NSString *)uuid;

- (void) addCaptureTypeFrom:(GMIPerson *)person;

- (void) addCaptureType:(NSString *)capType forPerson:(NSString *)uuid;

- (void) addCaptureTypes:(NSArray<NSString *>*)capTypes forPerson:(NSString *)uuid;

- (NSMutableDictionary<NSString *,NSString *> *)getPersonCaptureTypes:(NSString *)uuid;

- (void) removeCapture:(NSString *)type fromPerson:(NSString *)uuid;

- (void) addCaptureTypeFromMsg:(GMIMessage *)msg;

- (BOOL) isCapture:(NSString *)type enrolledForPerson:(NSString *)uuid;

- (void) removeAll;

- (NSArray<NSString *> *)allEnrollments;

//
//

- (void) storePalmMessages:(NSArray<GMIMessage *> *)messages;
- (NSArray<GMIMessage *> *)getPalmMessages;

@end
