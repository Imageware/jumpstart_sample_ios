//
//  RenderLock.h
//  GMI
//
//  Created by iws03cbrooks1 on 12/14/16.
//  Copyright © 2016 ImageWare Systems Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface RenderLock : NSObject

// whether this RenderLock is currently locked
- (BOOL) isLocked;

// locks this RenderLock
- (void) lock;

// releases this RenderLock
- (void) releaseLock;

@end
