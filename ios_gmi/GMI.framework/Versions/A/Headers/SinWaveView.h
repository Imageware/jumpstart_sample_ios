//
//  SinWave.h
//  GMI
//
//  Created by iws03cbrooks1 on 7/11/16.
//  Copyright © 2016 ImageWare Systems Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SinWaveView : UIView

// set the amplitude to 0
- (void) resetAmplitude;

// sets our amplitude to help us generate points...
- (void) setAmplitude: (float) amps;

// sets the maximum size for our circle, used in the calculation
// CIRCLE_RADIUS (maxRadius == 125 ? MIN(maxRadius, ([self bounds].size.width / 2) - 6) : maxRadius)
- (void) setCircleRadius: (float) radius;

@end
