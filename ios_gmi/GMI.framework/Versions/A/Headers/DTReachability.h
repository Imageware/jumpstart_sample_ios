//
//  DTReachability.h
//  DeluxeHome
//
//  Created by Dana on 6/1/13.
//  Copyright (c) 2013 DevoeTechnologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Reachability.h"

@interface DTReachability : Reachability

+ (BOOL) is3GReachable:(NSString*)hostName;
+ (BOOL) isWiFiReachable:(NSString*)hostName;
+ (BOOL) isServerReachable:(NSString*)hostName;
+ (BOOL) isConnectionRequired:(NSString*)hostName;

+ (void) signal;

@end
