//
//  NSData+Extensions.h
//  PalmSDK-iOS
//
//  Created by Serhiy Redko on 6/7/17.
//  Copyright © 2017 RedRock Biometrics. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSData (RRBExtensions)

- (NSData *)rrbPalmSDK_saltedData;
- (NSData *)rrbPalmSDK_unsaltedData;

@end
