//
//  GMIAlgorithm.h
//  GMI
//
//  Created by ddevoe on 7/9/18.
//  Copyright © 2018 ImageWare Systems Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface GMIAlgorithm : NSObject

@property (readonly) NSString *code;
@property (readonly) NSString *vendor;
@property (readonly) NSInteger enrollActionAsInterger;
@property (readonly) NSInteger enrollTypleAsInterger;
@property (readonly) NSString *captureType;
@property (readonly) NSInteger bioType;
@property (readonly) NSInteger templateType;
@property (readonly) NSArray<NSDictionary *> *livenessChecks; // @{ "name" : "code" }
@property (readonly) NSArray *enrollSamples;
@property (readonly) NSArray *verifySamples;
@property (readonly) NSInteger scoreThreshold;
@property (readonly) NSInteger zscoreThreshold;
@property (readonly) NSInteger fusionWeight;
@property (readonly) NSString *enrollType;
@property (readonly) NSString *enrollAction;
@property (readonly) NSString *version;

- (instancetype) initWithDict:(NSDictionary *)dict;


@end
