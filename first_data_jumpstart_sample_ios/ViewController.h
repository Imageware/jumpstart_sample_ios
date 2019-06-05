//
//  ViewController.h
//  first_data_jumpstart_sample_ios
//
//  Created by Henry Chan on 1/4/19.
//  Copyright © 2019 ImageWare Systems, Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController

@property (weak, nonatomic) IBOutlet UITextField *userIdTextField;
@property (weak, nonatomic) IBOutlet UIButton *registerButton;
@property (weak, nonatomic) IBOutlet UIButton *enrollButton;
@property (weak, nonatomic) IBOutlet UIButton *verifyButton;
@property (weak, nonatomic) IBOutlet UILabel *appVersionLabel;
@property (weak, nonatomic) IBOutlet UILabel *SDKversionLabel;
@property (weak, nonatomic) IBOutlet UILabel *GMIserverLabel;

@property (strong, nonatomic) IBOutlet NSDictionary *serverData;

- (IBAction)verifyButtonAction:(id)sender;
- (IBAction)registerButtonAction:(id)sender;

@end

