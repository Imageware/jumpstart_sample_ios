//
//  UILabel+iws.h
//  GMI
//
//  Created by ddevoe on 1/24/18.
//  Copyright © 2018 ImageWare Systems Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UILabel (iws)

@property (nonatomic,strong) NSString *outlineText;
@property (nonatomic,strong) UIColor *dropShadow;

- (void) roundedCorners ;

@end
