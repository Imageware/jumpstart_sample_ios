//
//  ModalFlexAlertViewController.h
//  alertTest
//
//  Created by Dana on 7/19/17.
//  Copyright © 2017 ImageWare Systems, Inc. All rights reserved.
//

/*
 This class is designed to simplify use of this class, as you no longer need to
 1. Create button
 2. Create a selector for the target of that button
 3. Assign that selector to the button
 4. Assign the text to the button
 */

#import "ModalAlertViewController.h"

@interface ButtonSpec
@property (nonatomic,strong) NSString* title;
@end


@interface ModalFlexAlertViewController : ModalAlertViewController

- (instancetype)initWithType:(ModalAlertTypes)showType withTitle:(NSString *)titleString usingButtons:(NSArray<NSString *> *)buttonList completion:(void (^)(NSInteger buttonIndex))completion;


@end
