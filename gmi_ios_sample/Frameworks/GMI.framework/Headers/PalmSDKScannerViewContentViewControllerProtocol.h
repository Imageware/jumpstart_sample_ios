//
//  PalmSDKScannerViewContentViewControllerProtocol.h
//  PalmSDK-iOS
//
//  Created by Serhiy Redko on 6/5/17.
//  Copyright © 2017 RedRock Biometrics. All rights reserved.
//

@protocol PalmSDKScannerViewContentViewControllerProtocol <NSObject>

@property (nonatomic) BOOL dismissOnAppear;
@property (nonatomic) CGFloat bottomDistanceToViewArea;

- (UIImage *)gotoImage;

- (void)cancel;

- (BOOL)shouldShowLogo;

@end
