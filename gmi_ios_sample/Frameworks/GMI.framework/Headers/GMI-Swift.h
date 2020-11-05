// Generated by Apple Swift version 5.3 (swiftlang-1200.0.29.2 clang-1200.0.30.1)
#ifndef GMI_SWIFT_H
#define GMI_SWIFT_H
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgcc-compat"

#if !defined(__has_include)
# define __has_include(x) 0
#endif
#if !defined(__has_attribute)
# define __has_attribute(x) 0
#endif
#if !defined(__has_feature)
# define __has_feature(x) 0
#endif
#if !defined(__has_warning)
# define __has_warning(x) 0
#endif

#if __has_include(<swift/objc-prologue.h>)
# include <swift/objc-prologue.h>
#endif

#pragma clang diagnostic ignored "-Wauto-import"
#include <Foundation/Foundation.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#if !defined(SWIFT_TYPEDEFS)
# define SWIFT_TYPEDEFS 1
# if __has_include(<uchar.h>)
#  include <uchar.h>
# elif !defined(__cplusplus)
typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;
# endif
typedef float swift_float2  __attribute__((__ext_vector_type__(2)));
typedef float swift_float3  __attribute__((__ext_vector_type__(3)));
typedef float swift_float4  __attribute__((__ext_vector_type__(4)));
typedef double swift_double2  __attribute__((__ext_vector_type__(2)));
typedef double swift_double3  __attribute__((__ext_vector_type__(3)));
typedef double swift_double4  __attribute__((__ext_vector_type__(4)));
typedef int swift_int2  __attribute__((__ext_vector_type__(2)));
typedef int swift_int3  __attribute__((__ext_vector_type__(3)));
typedef int swift_int4  __attribute__((__ext_vector_type__(4)));
typedef unsigned int swift_uint2  __attribute__((__ext_vector_type__(2)));
typedef unsigned int swift_uint3  __attribute__((__ext_vector_type__(3)));
typedef unsigned int swift_uint4  __attribute__((__ext_vector_type__(4)));
#endif

#if !defined(SWIFT_PASTE)
# define SWIFT_PASTE_HELPER(x, y) x##y
# define SWIFT_PASTE(x, y) SWIFT_PASTE_HELPER(x, y)
#endif
#if !defined(SWIFT_METATYPE)
# define SWIFT_METATYPE(X) Class
#endif
#if !defined(SWIFT_CLASS_PROPERTY)
# if __has_feature(objc_class_property)
#  define SWIFT_CLASS_PROPERTY(...) __VA_ARGS__
# else
#  define SWIFT_CLASS_PROPERTY(...)
# endif
#endif

#if __has_attribute(objc_runtime_name)
# define SWIFT_RUNTIME_NAME(X) __attribute__((objc_runtime_name(X)))
#else
# define SWIFT_RUNTIME_NAME(X)
#endif
#if __has_attribute(swift_name)
# define SWIFT_COMPILE_NAME(X) __attribute__((swift_name(X)))
#else
# define SWIFT_COMPILE_NAME(X)
#endif
#if __has_attribute(objc_method_family)
# define SWIFT_METHOD_FAMILY(X) __attribute__((objc_method_family(X)))
#else
# define SWIFT_METHOD_FAMILY(X)
#endif
#if __has_attribute(noescape)
# define SWIFT_NOESCAPE __attribute__((noescape))
#else
# define SWIFT_NOESCAPE
#endif
#if __has_attribute(ns_consumed)
# define SWIFT_RELEASES_ARGUMENT __attribute__((ns_consumed))
#else
# define SWIFT_RELEASES_ARGUMENT
#endif
#if __has_attribute(warn_unused_result)
# define SWIFT_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
# define SWIFT_WARN_UNUSED_RESULT
#endif
#if __has_attribute(noreturn)
# define SWIFT_NORETURN __attribute__((noreturn))
#else
# define SWIFT_NORETURN
#endif
#if !defined(SWIFT_CLASS_EXTRA)
# define SWIFT_CLASS_EXTRA
#endif
#if !defined(SWIFT_PROTOCOL_EXTRA)
# define SWIFT_PROTOCOL_EXTRA
#endif
#if !defined(SWIFT_ENUM_EXTRA)
# define SWIFT_ENUM_EXTRA
#endif
#if !defined(SWIFT_CLASS)
# if __has_attribute(objc_subclassing_restricted)
#  define SWIFT_CLASS(SWIFT_NAME) SWIFT_RUNTIME_NAME(SWIFT_NAME) __attribute__((objc_subclassing_restricted)) SWIFT_CLASS_EXTRA
#  define SWIFT_CLASS_NAMED(SWIFT_NAME) __attribute__((objc_subclassing_restricted)) SWIFT_COMPILE_NAME(SWIFT_NAME) SWIFT_CLASS_EXTRA
# else
#  define SWIFT_CLASS(SWIFT_NAME) SWIFT_RUNTIME_NAME(SWIFT_NAME) SWIFT_CLASS_EXTRA
#  define SWIFT_CLASS_NAMED(SWIFT_NAME) SWIFT_COMPILE_NAME(SWIFT_NAME) SWIFT_CLASS_EXTRA
# endif
#endif
#if !defined(SWIFT_RESILIENT_CLASS)
# if __has_attribute(objc_class_stub)
#  define SWIFT_RESILIENT_CLASS(SWIFT_NAME) SWIFT_CLASS(SWIFT_NAME) __attribute__((objc_class_stub))
#  define SWIFT_RESILIENT_CLASS_NAMED(SWIFT_NAME) __attribute__((objc_class_stub)) SWIFT_CLASS_NAMED(SWIFT_NAME)
# else
#  define SWIFT_RESILIENT_CLASS(SWIFT_NAME) SWIFT_CLASS(SWIFT_NAME)
#  define SWIFT_RESILIENT_CLASS_NAMED(SWIFT_NAME) SWIFT_CLASS_NAMED(SWIFT_NAME)
# endif
#endif

#if !defined(SWIFT_PROTOCOL)
# define SWIFT_PROTOCOL(SWIFT_NAME) SWIFT_RUNTIME_NAME(SWIFT_NAME) SWIFT_PROTOCOL_EXTRA
# define SWIFT_PROTOCOL_NAMED(SWIFT_NAME) SWIFT_COMPILE_NAME(SWIFT_NAME) SWIFT_PROTOCOL_EXTRA
#endif

#if !defined(SWIFT_EXTENSION)
# define SWIFT_EXTENSION(M) SWIFT_PASTE(M##_Swift_, __LINE__)
#endif

#if !defined(OBJC_DESIGNATED_INITIALIZER)
# if __has_attribute(objc_designated_initializer)
#  define OBJC_DESIGNATED_INITIALIZER __attribute__((objc_designated_initializer))
# else
#  define OBJC_DESIGNATED_INITIALIZER
# endif
#endif
#if !defined(SWIFT_ENUM_ATTR)
# if defined(__has_attribute) && __has_attribute(enum_extensibility)
#  define SWIFT_ENUM_ATTR(_extensibility) __attribute__((enum_extensibility(_extensibility)))
# else
#  define SWIFT_ENUM_ATTR(_extensibility)
# endif
#endif
#if !defined(SWIFT_ENUM)
# define SWIFT_ENUM(_type, _name, _extensibility) enum _name : _type _name; enum SWIFT_ENUM_ATTR(_extensibility) SWIFT_ENUM_EXTRA _name : _type
# if __has_feature(generalized_swift_name)
#  define SWIFT_ENUM_NAMED(_type, _name, SWIFT_NAME, _extensibility) enum _name : _type _name SWIFT_COMPILE_NAME(SWIFT_NAME); enum SWIFT_COMPILE_NAME(SWIFT_NAME) SWIFT_ENUM_ATTR(_extensibility) SWIFT_ENUM_EXTRA _name : _type
# else
#  define SWIFT_ENUM_NAMED(_type, _name, SWIFT_NAME, _extensibility) SWIFT_ENUM(_type, _name, _extensibility)
# endif
#endif
#if !defined(SWIFT_UNAVAILABLE)
# define SWIFT_UNAVAILABLE __attribute__((unavailable))
#endif
#if !defined(SWIFT_UNAVAILABLE_MSG)
# define SWIFT_UNAVAILABLE_MSG(msg) __attribute__((unavailable(msg)))
#endif
#if !defined(SWIFT_AVAILABILITY)
# define SWIFT_AVAILABILITY(plat, ...) __attribute__((availability(plat, __VA_ARGS__)))
#endif
#if !defined(SWIFT_WEAK_IMPORT)
# define SWIFT_WEAK_IMPORT __attribute__((weak_import))
#endif
#if !defined(SWIFT_DEPRECATED)
# define SWIFT_DEPRECATED __attribute__((deprecated))
#endif
#if !defined(SWIFT_DEPRECATED_MSG)
# define SWIFT_DEPRECATED_MSG(...) __attribute__((deprecated(__VA_ARGS__)))
#endif
#if __has_feature(attribute_diagnose_if_objc)
# define SWIFT_DEPRECATED_OBJC(Msg) __attribute__((diagnose_if(1, Msg, "warning")))
#else
# define SWIFT_DEPRECATED_OBJC(Msg) SWIFT_DEPRECATED_MSG(Msg)
#endif
#if !defined(IBSegueAction)
# define IBSegueAction
#endif
#if __has_feature(modules)
#if __has_warning("-Watimport-in-framework-header")
#pragma clang diagnostic ignored "-Watimport-in-framework-header"
#endif
@import AVFoundation;
@import CoreGraphics;
@import CoreMedia;
@import Foundation;
@import ObjectiveC;
@import RealmSwift;
@import UIKit;
#endif

#pragma clang diagnostic ignored "-Wproperty-attribute-mismatch"
#pragma clang diagnostic ignored "-Wduplicate-method-arg"
#if __has_warning("-Wpragma-clang-attribute")
# pragma clang diagnostic ignored "-Wpragma-clang-attribute"
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wnullability"

#if __has_attribute(external_source_symbol)
# pragma push_macro("any")
# undef any
# pragma clang attribute push(__attribute__((external_source_symbol(language="Swift", defined_in="GMI",generated_declaration))), apply_to=any(function,enum,objc_interface,objc_category,objc_protocol))
# pragma pop_macro("any")
#endif


SWIFT_CLASS("_TtC3GMI5Model")
@interface Model : RealmSwiftObject
+ (NSString * _Nullable)primaryKey SWIFT_WARN_UNUSED_RESULT;
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end

@class OAuth;
@class Config;
@class NSDate;

SWIFT_CLASS("_TtC3GMI7Account")
@interface Account : Model
@property (nonatomic, copy) NSString * _Nonnull email;
@property (nonatomic, copy) NSString * _Nonnull deviceId;
@property (nonatomic, strong) OAuth * _Nullable oauth;
@property (nonatomic, strong) Config * _Nullable config;
@property (nonatomic, copy) NSString * _Nullable tenantEnrollServer;
@property (nonatomic, copy) NSString * _Nullable id;
@property (nonatomic, copy) NSString * _Nullable verifyServer;
@property (nonatomic, copy) NSString * _Nullable voiceDigits;
@property (nonatomic, copy) NSString * _Nullable tenantVerifyServer;
@property (nonatomic, copy) NSString * _Nullable enrollServer;
@property (nonatomic, copy) NSString * _Nullable name;
@property (nonatomic, strong) NSDate * _Nullable date2FARequestThisDevice;
/// whether this account has completed the 2FA step and registered their email, defaults to false
@property (nonatomic) BOOL emailVerified;
@property (nonatomic, strong) NSDate * _Nullable dateDeviceRegister;
@property (nonatomic, strong) NSDate * _Nullable eulaAccepted;
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI12AlertManager")
@interface AlertManager : NSObject
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end

@class NSBundle;
@class NSCoder;

SWIFT_CLASS("_TtC3GMI19AlertViewController")
@interface AlertViewController : UIViewController
- (nonnull instancetype)initWithNibName:(NSString * _Nullable)nibNameOrNil bundle:(NSBundle * _Nullable)nibBundleOrNil OBJC_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder * _Nonnull)coder OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI8AppState")
@interface AppState : Model
@property (nonatomic, strong) Config * _Nullable config;
@property (nonatomic, copy) NSString * _Nullable pushToken;
@property (nonatomic, copy) NSDate * _Nullable onboardingShown;
@property (nonatomic, copy) NSString * _Nullable whatsNewVersion;
@property (nonatomic, copy) NSDate * _Nullable pushWasAsked;
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end



SWIFT_CLASS("_TtC3GMI6Device")
@interface Device : NSObject
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI18CameraDeviceParent")
@interface CameraDeviceParent : Device
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI12CameraDevice")
@interface CameraDevice : CameraDeviceParent <AVCaptureVideoDataOutputSampleBufferDelegate>
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end

@class AVCapturePhotoOutput;
@class AVCaptureResolvedPhotoSettings;
@class AVCaptureBracketedStillImageSettings;

@interface CameraDevice (SWIFT_EXTENSION(GMI)) <AVCapturePhotoCaptureDelegate>
- (void)captureOutput:(AVCapturePhotoOutput * _Nonnull)captureOutput didFinishProcessingPhotoSampleBuffer:(CMSampleBufferRef _Nullable)photoSampleBuffer previewPhotoSampleBuffer:(CMSampleBufferRef _Nullable)previewPhotoSampleBuffer resolvedSettings:(AVCaptureResolvedPhotoSettings * _Nonnull)resolvedSettings bracketSettings:(AVCaptureBracketedStillImageSettings * _Nullable)bracketSettings error:(NSError * _Nullable)error;
@end



SWIFT_CLASS("_TtC3GMI10Capturable")
@interface Capturable : Model
@property (nonatomic, copy) NSDate * _Nullable hidden;
@property (nonatomic, copy) NSDate * _Nullable skipped;
@property (nonatomic, strong) Account * _Nullable account;
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI12CaptureDatum")
@interface CaptureDatum : NSObject
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI11CaptureFlow")
@interface CaptureFlow : NSObject
- (nonnull instancetype)init SWIFT_UNAVAILABLE;
+ (nonnull instancetype)new SWIFT_UNAVAILABLE_MSG("-init is unavailable");
@end


SWIFT_CLASS("_TtC3GMI18CaptureFlowManager")
@interface CaptureFlowManager : NSObject
- (nonnull instancetype)init SWIFT_UNAVAILABLE;
+ (nonnull instancetype)new SWIFT_UNAVAILABLE_MSG("-init is unavailable");
@end


SWIFT_CLASS("_TtC3GMI25CaptureFlowViewController")
@interface CaptureFlowViewController : UIViewController
- (nullable instancetype)initWithCoder:(NSCoder * _Nonnull)aDecoder SWIFT_UNAVAILABLE;
@property (nonatomic, readonly, copy) NSString * _Nonnull description;
- (void)viewDidLoad;
- (void)viewDidDisappear:(BOOL)animated;
- (nonnull instancetype)initWithNibName:(NSString * _Nullable)nibNameOrNil bundle:(NSBundle * _Nullable)nibBundleOrNil SWIFT_UNAVAILABLE;
@end


SWIFT_CLASS("_TtC3GMI14CaptureManager")
@interface CaptureManager : NSObject
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI25CaptureableUploadResponse")
@interface CaptureableUploadResponse : Model
@end


SWIFT_CLASS("_TtC3GMI6Config")
@interface Config : Model
@property (nonatomic, copy) NSString * _Nonnull presetName;
@property (nonatomic, copy) NSString * _Nonnull presetTitle;
@property (nonatomic, copy) NSString * _Nonnull gmiServerUrl;
@property (nonatomic, copy) NSString * _Nonnull gmiClientSecret;
@property (nonatomic, copy) NSString * _Nonnull gmiClientId;
@property (nonatomic, copy) NSString * _Nonnull gmiUserManagerUrl;
@property (nonatomic, copy) NSString * _Nonnull defaultTenantCode;
@property (nonatomic, copy) NSString * _Nonnull applicationCode;
+ (NSArray<NSString *> * _Nonnull)ignoredProperties SWIFT_WARN_UNUSED_RESULT;
- (nonnull instancetype)init SWIFT_UNAVAILABLE;
+ (nonnull instancetype)new SWIFT_UNAVAILABLE_MSG("-init is unavailable");
@end

@class Enroll;
@class Message;

SWIFT_PROTOCOL("_TtP3GMI15DatabaseManager_")
@protocol DatabaseManager
- (void)delete:(Model * _Nonnull)object;
- (void)delete:(Model * _Nonnull)object completion:(void (^ _Nullable)(void))completion;
- (void)persistWithObject:(Model * _Nonnull)object;
/// attempts to write the passed item to disc
/// the passed closure returns true if the item was written, false if not
- (void)persistWithObject:(Model * _Nonnull)object completion:(SWIFT_NOESCAPE void (^ _Nonnull)(BOOL))completion;
- (void)write:(SWIFT_NOESCAPE void (^ _Nonnull)(void))writeBlock;
- (void)write:(SWIFT_NOESCAPE void (^ _Nonnull)(void))writeBlock completion:(void (^ _Nullable)(void))completion;
/// Retrieves all of the Enrolls in the current database and calls the passed closure for the array of found items.
- (NSArray<Enroll *> * _Nonnull)enrolls SWIFT_WARN_UNUSED_RESULT;
- (void)enrolls:(void (^ _Nonnull)(NSArray<Enroll *> * _Nonnull))completion;
- (NSArray<Enroll *> * _Nonnull)activeEnrolls SWIFT_WARN_UNUSED_RESULT;
/// Retrieves all of the Alerts in the current database and calls the passed closure for the array of found items.
- (NSArray<Message *> * _Nonnull)alerts SWIFT_WARN_UNUSED_RESULT;
- (void)alerts:(void (^ _Nonnull)(NSArray<Message *> * _Nonnull))completion;
- (NSArray<Message *> * _Nonnull)unreadAlerts SWIFT_WARN_UNUSED_RESULT;
- (void)unreadAlerts:(void (^ _Nonnull)(NSArray<Message *> * _Nonnull))completion;
- (NSArray<Message *> * _Nonnull)activeAlerts SWIFT_WARN_UNUSED_RESULT;
- (AppState * _Nonnull)appState SWIFT_WARN_UNUSED_RESULT;
- (NSArray<Config *> * _Nonnull)configs SWIFT_WARN_UNUSED_RESULT;
- (NSArray<Account *> * _Nonnull)accounts SWIFT_WARN_UNUSED_RESULT;
- (void)accounts:(void (^ _Nonnull)(NSArray<Account *> * _Nonnull))completion;
@end


@class EnrollMetadata;

SWIFT_CLASS("_TtC3GMI6Enroll")
@interface Enroll : Capturable
@property (nonatomic) BOOL completed;
@property (nonatomic) NSInteger messageID;
@property (nonatomic, strong) EnrollMetadata * _Nullable metadata;
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI10EnrollData")
@interface EnrollData : Model
@property (nonatomic, copy) NSString * _Nullable algCode;
@property (nonatomic, strong) Account * _Nullable account;
+ (NSArray<NSString *> * _Nonnull)ignoredProperties SWIFT_WARN_UNUSED_RESULT;
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI14EnrollMetadata")
@interface EnrollMetadata : Model
@property (nonatomic, strong) EnrollData * _Nullable enrollData;
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end


@class RESTManager;
@class InteractionManager;

SWIFT_CLASS("_TtC3GMI8Instance")
@interface Instance : NSObject
SWIFT_CLASS_PROPERTY(@property (nonatomic, class, readonly, copy) NSString * _Nonnull gmiVersion;)
+ (NSString * _Nonnull)gmiVersion SWIFT_WARN_UNUSED_RESULT;
@property (nonatomic, readonly, copy) NSString * _Nonnull appVersion;
@property (nonatomic, readonly, copy) NSString * _Nonnull deviceID;
SWIFT_CLASS_PROPERTY(@property (nonatomic, class, readonly, strong) Instance * _Nonnull shared;)
+ (Instance * _Nonnull)shared SWIFT_WARN_UNUSED_RESULT;
@property (nonatomic, readonly, strong) RESTManager * _Nonnull restManager;
@property (nonatomic, readonly, strong) CaptureFlowManager * _Nonnull captureFlowManager;
@property (nonatomic, readonly, strong) InteractionManager * _Nonnull interactionManager;
@property (nonatomic, readonly, strong) id <DatabaseManager> _Nonnull databaseManager;
SWIFT_CLASS_PROPERTY(@property (nonatomic, class, copy) NSString * _Nullable configsPath;)
+ (NSString * _Nullable)configsPath SWIFT_WARN_UNUSED_RESULT;
+ (void)setConfigsPath:(NSString * _Nullable)value;
- (nonnull instancetype)init SWIFT_UNAVAILABLE;
+ (nonnull instancetype)new SWIFT_UNAVAILABLE_MSG("-init is unavailable");
+ (void)debugModeWithDebugMockDB:(id <DatabaseManager> _Nonnull)debugMockDB;
@end

@class InteractionManagerRootOverlayVC;

SWIFT_CLASS("_TtC3GMI18InteractionManager")
@interface InteractionManager : NSObject
@property (nonatomic, strong) UIViewController * _Nullable rootVCFromApp;
@property (nonatomic, strong) InteractionManagerRootOverlayVC * _Nullable interactionOverlayRoot;
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end

@class UIStoryboardSegue;

SWIFT_CLASS("_TtC3GMI31InteractionManagerRootOverlayVC")
@interface InteractionManagerRootOverlayVC : UIViewController
- (void)prepareForSegue:(UIStoryboardSegue * _Nonnull)segue sender:(id _Nullable)sender;
- (nonnull instancetype)initWithNibName:(NSString * _Nullable)nibNameOrNil bundle:(NSBundle * _Nullable)nibBundleOrNil OBJC_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder * _Nonnull)coder OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI7Message")
@interface Message : Capturable
@property (nonatomic, copy) NSString * _Nullable messageID;
+ (NSArray<NSString *> * _Nonnull)ignoredProperties SWIFT_WARN_UNUSED_RESULT;
- (nonnull instancetype)init SWIFT_UNAVAILABLE;
+ (nonnull instancetype)new SWIFT_UNAVAILABLE_MSG("-init is unavailable");
@end



SWIFT_CLASS("_TtC3GMI14NoProfileAlert")
@interface NoProfileAlert : UIViewController
- (nonnull instancetype)initWithNibName:(NSString * _Nullable)nibNameOrNil bundle:(NSBundle * _Nullable)nibBundleOrNil OBJC_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder * _Nonnull)coder OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI5OAuth")
@interface OAuth : Model
@property (nonatomic, copy) NSString * _Nullable tokenType;
@property (nonatomic, copy) NSString * _Nullable scope;
@property (nonatomic, copy) NSString * _Nullable accessToken;
@property (nonatomic, strong) Config * _Nullable config;
@property (nonatomic) NSInteger expiresAt;
- (nonnull instancetype)init SWIFT_UNAVAILABLE;
+ (nonnull instancetype)new SWIFT_UNAVAILABLE_MSG("-init is unavailable");
@end


SWIFT_CLASS("_TtC3GMI30OnboardingScreenViewController")
@interface OnboardingScreenViewController : UIViewController
- (void)viewDidLoad;
- (void)didReceiveMemoryWarning;
- (void)prepareForSegue:(UIStoryboardSegue * _Nonnull)segue sender:(id _Nullable)sender;
- (nonnull instancetype)initWithNibName:(NSString * _Nullable)nibNameOrNil bundle:(NSBundle * _Nullable)nibBundleOrNil OBJC_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder * _Nonnull)coder OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI24OnboardingViewController")
@interface OnboardingViewController : UIPageViewController <UIPageViewControllerDataSource, UIPageViewControllerDelegate>
- (UIViewController * _Nullable)pageViewController:(UIPageViewController * _Nonnull)pageViewController viewControllerBeforeViewController:(UIViewController * _Nonnull)viewController SWIFT_WARN_UNUSED_RESULT;
- (UIViewController * _Nullable)pageViewController:(UIPageViewController * _Nonnull)pageViewController viewControllerAfterViewController:(UIViewController * _Nonnull)viewController SWIFT_WARN_UNUSED_RESULT;
- (NSInteger)presentationCountForPageViewController:(UIPageViewController * _Nonnull)for_ SWIFT_WARN_UNUSED_RESULT;
- (NSInteger)presentationIndexForPageViewController:(UIPageViewController * _Nonnull)pageViewController SWIFT_WARN_UNUSED_RESULT;
- (void)viewDidLoad;
- (void)viewWillAppear:(BOOL)animated;
- (void)didReceiveMemoryWarning;
- (nonnull instancetype)initWithTransitionStyle:(UIPageViewControllerTransitionStyle)style navigationOrientation:(UIPageViewControllerNavigationOrientation)navigationOrientation options:(NSDictionary<UIPageViewControllerOptionsKey, id> * _Nullable)options OBJC_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder * _Nonnull)coder OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI18PersonEnrollStatus")
@interface PersonEnrollStatus : Model
@property (nonatomic, copy) NSDate * _Nullable hidden;
@property (nonatomic, copy) NSDate * _Nullable skipped;
@property (nonatomic, copy) NSString * _Nullable captureType;
@property (nonatomic, copy) NSString * _Nullable algCode;
@property (nonatomic, copy) NSString * _Nullable tenantCode;
@property (nonatomic, copy) NSString * _Nullable userID;
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI9RESTError")
@interface RESTError : Model
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI11RESTManager")
@interface RESTManager : NSObject
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end



SWIFT_CLASS("_TtC3GMI20RealmDatabaseManager")
@interface RealmDatabaseManager : NSObject <DatabaseManager>
- (nonnull instancetype)init SWIFT_UNAVAILABLE;
+ (nonnull instancetype)new SWIFT_UNAVAILABLE_MSG("-init is unavailable");
- (void)delete:(Model * _Nonnull)object;
- (void)delete:(Model * _Nonnull)object completion:(void (^ _Nullable)(void))completion;
- (void)persistWithObject:(Model * _Nonnull)object;
- (void)persistWithObject:(Model * _Nonnull)object completion:(SWIFT_NOESCAPE void (^ _Nonnull)(BOOL))completion;
- (void)write:(SWIFT_NOESCAPE void (^ _Nonnull)(void))writeBlock;
/// Write to the database using the passed writeBlock param. Afterwards, calls the passed completion parameter, if any.
/// <ul>
///   <li>
///     Parameters:
///   </li>
///   <li>
///     writeBlock: a database write transaction
///   </li>
///   <li>
///     completion: a closure to call when the write has completed, successful or not. <em>NOTE</em> for convenience, this closure is  run on the main thread.
///   </li>
/// </ul>
- (void)write:(SWIFT_NOESCAPE void (^ _Nonnull)(void))writeBlock completion:(void (^ _Nullable)(void))completion;
- (NSArray<Enroll *> * _Nonnull)enrolls SWIFT_WARN_UNUSED_RESULT;
- (void)enrolls:(void (^ _Nonnull)(NSArray<Enroll *> * _Nonnull))completion;
- (NSArray<Enroll *> * _Nonnull)activeEnrolls SWIFT_WARN_UNUSED_RESULT;
- (NSArray<Message *> * _Nonnull)alerts SWIFT_WARN_UNUSED_RESULT;
- (void)alerts:(void (^ _Nonnull)(NSArray<Message *> * _Nonnull))completion;
- (NSArray<Message *> * _Nonnull)unreadAlerts SWIFT_WARN_UNUSED_RESULT;
- (void)unreadAlerts:(void (^ _Nonnull)(NSArray<Message *> * _Nonnull))completion;
- (NSArray<Message *> * _Nonnull)activeAlerts SWIFT_WARN_UNUSED_RESULT;
- (NSArray<Account *> * _Nonnull)accounts SWIFT_WARN_UNUSED_RESULT;
- (void)accounts:(void (^ _Nonnull)(NSArray<Account *> * _Nonnull))completion;
- (AppState * _Nonnull)appState SWIFT_WARN_UNUSED_RESULT;
- (NSArray<Config *> * _Nonnull)configs SWIFT_WARN_UNUSED_RESULT;
@end



SWIFT_CLASS("_TtC3GMI17SelectedBiometric")
@interface SelectedBiometric : Model
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end

@class NSTextContainer;

SWIFT_CLASS("_TtC3GMI17SelfSizedTextView")
@interface SelfSizedTextView : UITextView
@property (nonatomic, readonly) CGSize intrinsicContentSize;
@property (nonatomic) CGSize contentSize;
- (nonnull instancetype)initWithFrame:(CGRect)frame textContainer:(NSTextContainer * _Nullable)textContainer OBJC_DESIGNATED_INITIALIZER SWIFT_AVAILABILITY(ios,introduced=7.0);
- (nullable instancetype)initWithCoder:(NSCoder * _Nonnull)coder OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI11SystemAlert")
@interface SystemAlert : AlertViewController
- (void)viewDidLoad;
- (nonnull instancetype)initWithNibName:(NSString * _Nullable)nibNameOrNil bundle:(NSBundle * _Nullable)nibBundleOrNil OBJC_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder * _Nonnull)coder OBJC_DESIGNATED_INITIALIZER;
@end


SWIFT_CLASS("_TtC3GMI22TutorialViewController")
@interface TutorialViewController : OnboardingViewController <UIGestureRecognizerDelegate>
- (void)viewDidLoad;
- (void)viewDidAppear:(BOOL)animated;
- (void)viewDidDisappear:(BOOL)animated;
- (nonnull instancetype)initWithTransitionStyle:(UIPageViewControllerTransitionStyle)style navigationOrientation:(UIPageViewControllerNavigationOrientation)navigationOrientation options:(NSDictionary<UIPageViewControllerOptionsKey, id> * _Nullable)options OBJC_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder * _Nonnull)coder OBJC_DESIGNATED_INITIALIZER;
@end




















SWIFT_CLASS("_TtC3GMI17VideoCameraDevice")
@interface VideoCameraDevice : CameraDevice
- (nonnull instancetype)init OBJC_DESIGNATED_INITIALIZER;
@end

#if __has_attribute(external_source_symbol)
# pragma clang attribute pop
#endif
#pragma clang diagnostic pop
#endif
