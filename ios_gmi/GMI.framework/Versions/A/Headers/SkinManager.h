//
//  SkinManager.h
//  GMI
//
//  Created by iws03cbrooks1 on 7/27/16.
//  Copyright © 2016 ImageWare Systems Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

// definition for a Skin object that's used to provide custom colors, fonts and images for the various capture screens used by the SDK.
@interface Skin : NSObject

// the font for this skin, replacing fonts in buttons and labels for capture view controllers.
@property UIFont *font;

@end



// simple protocol used by views to apply the global skin to them
@protocol Skinnable <NSObject>

// applies the passed skin to this Skinnable object. This will replace the view's specific fonts, colors or images attributed to the skin.
@optional - (void) applySkin: (Skin *) skin;

@end



//
@interface SkinManager : NSObject

// sets the global skin to be used by the SDK when rendering capture view controllers.
+ (void) setGlobalSkin: (Skin *) skin;

// return the set global skin. If the skin is nil or hasn't been set yet, this will return the default skin used by the SDK.
+ (Skin *) getGlobalSkin;

@end
