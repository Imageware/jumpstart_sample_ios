//
//  ModalAlertViewController.h
//  ios_newPillPhone
//
//  Created by Henry Chan on 6/29/17.
//  Copyright © 2017 Henry Chan. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum
{
    ModalTypeAlert,     // Blue - info icon
    ModalTypeInfo,      // Blue - info icon
    ModalTypeWarning,   // Red - ! icon in stop sign
    ModalTypeMail,      // Yellow - mail icon
    ModalTypeAddPerson, // Blue - person + icon
    ModalTypeCheckmark, // Green - Check icon
    ModalTypeWait,       // Blue - spinner
    ModalTypeError,     // Yellow - ! icon
    ModelTypeCritical   // RED - ! yeild sign
} ModalAlertTypes;

@interface ModalAlertViewController : UIViewController

@property (weak, nonatomic) IBOutlet UIView *alertView;

@property (strong, nonatomic) NSString *titleStr;
@property (strong, nonatomic) NSString *message;


- (instancetype)initWithType:(ModalAlertTypes)showType withTitle:(NSString *)titleString completion:(void (^)(void))completion;
- (instancetype)initWithType:(ModalAlertTypes)showType withTitle:(NSString *)titleString usingButtons:(NSArray *)buttonList completion:(void (^)(void))completion;

- (CGRect)messageFrame;

- (void) stopWait;  // Only for ModalTypeWait

@end

