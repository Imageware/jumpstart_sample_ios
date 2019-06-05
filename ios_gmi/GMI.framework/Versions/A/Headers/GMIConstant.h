/*************************************************************************
 *
 * IMAGEWARE SYSTEMS, INC. CONFIDENTIAL
 * __________________
 *
 *  2000 - 2015 ImageWare Systems, Inc.
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains the
 * property of ImageWare Systems Inc. and its suppliers, if any.
 * The intellectual and technical concepts contained herein are
 * proprietary to ImageWare Systems Inc. and its suppliers and may
 * be covered by U.S. and Foreign Patents, patents in process, and
 * are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from ImageWare Systems Inc.
 *
 * Author(s)	: cbrooks
 *
 * Description	: A list of constant variables used throughout the static library.
 */ 

#if DEBUG
#define DBG_Log(...) NSLog(__VA_ARGS__)
#else
#define DBG_Log(...)
#endif

// GMI platform/SDK version constants
#define GMI_PLATFORM                                        @"ios"
#if TARGET_OS_SIMULATOR
    #define BUILD_FACETEC NO
    #define BUILD_REDROCKS NO
#else
    #define BUILD_FACETEC 0
    #define BUILD_REDROCKS 1
#endif

// GMIResponse flags
// TODO: turn these into an in-Response Object type group
#define gmiresp_event_MessageComplete                       @"complete"
#define gmiresp_event_MessageAbandoned                      @"abandoned"
#define gmiresp_event_Verification                          @"Verification"
#define gmiresp_event_Enrollment                            @"Enrollment"
#define gmiresp_event_Identification                        @"Identification"
#define gmiresp_event_BiometricEventType                    @"bio_event"
#define gmiresp_event_BiometricModality                     @"modality"
#define gmiresp_Event                                       @"event"
#define gmiresp_VeriWasSuccessful                           @"successful"
#define gmiresp_VeriFailError                               @"error"
#define gmiresp_Yes                                         @"yes"
#define gmiresp_No                                          @"no"
#define gmiresp_BiometricFace                               @"face"
#define gmiresp_BiometricVoice                              @"voice"
#define gmiresp_BiometricLiveVoice                          @"livevoice"
#define gmiresp_VeriModality                                @"modality"
#define gmiresp_MessageId                                   @"message_id"
#define gmiresp_PersonId                                    @"person_id"
#define gmiresp_IsDirect                                    @"is_direct"
#define gmiresp_MessageURI                                  @"message_uri"
#define gmiresp_Extras                                      @"extras"
//

#define GRANULAR_PERMISSIONS_DENIED @"sdk.granular_permissions_denied"
#define NO_REGISTERED_FINGERPRINTS @"sdk.no_registered_fingerprints"
#define NO_FINGERPRINT_SUPPORT @"sdk.no_fingerprint_support"
#define FACE_TIMEOUT @"sdk.face_timed_out"
#define BLINK_TIMEOUT @"sdk.blink_timed_out"
#define PALM_TIMEOUT @"sdk.palm_timed_out"
#define UNKNOWN_MODALITY @"sdk.unsupported_type"
#define ERROR_DIALOG @"error.sdk.error"
#define FACETEC_TIMEOUT @"sdk.facetec_liveness_timed_out"

// defines for all of the modality names the SDK supports. Some, like MODALITY_STRING_MULTI, aren't intended to be used by templates or by implementors, it simply represents a GMIResponse object's event when multiple biometrics have been performed at the same time.
// for reference, typically support modalities are MODALITY_STRING_VOICE, MODALITY_STRING_FACE, MODALITY_STRING_FACEVOICE, MODALITY_STRING_FINGERPRINT, MODALITY_STRING_PASSPHRASE (identical to voice), and MODALITY_STRING_YESNO.

#define MODALITY_STRING_VOICE @"voice"
#define MODALITY_STRING_FACE @"face"
#define MODALITY_STRING_LIVEFACE @"liveface"
#define MODALITY_STRING_LIVEVOICE @"livevoice"
#define MODALITY_STRING_FINGERPRINT @"fingerprint"
#define MODALITY_STRING_MULTI @"multi-type biometric"
#define MODALITY_STRING_FACEVOICE @"facevoice"
#define MODALITY_STRING_YESNO @"yesno"
#define MODALITY_STRING_PASSPHRASE @"passphrase"
#define MODALITY_STRING_PALM @"palm"
#define MODALITY_STRING_PIN @"pin"
#define MODALITY_STRING_FACETEC @"face"

//Algorithm names
#define MODALITY_STRING_RRPALM @"rrpalm"
#define MODALITY_STRING_HTPALM @"htpalm"
#define ALGO_STRING_VALIDSOFT @"validsoftpass"

// LIveness related defines
#define FLAG_LIVENESS @"liveness"
#define FLAG_LIVENESS_FACETEC @"zoom"
#define FLAG_LIVENESS_BLINK @"blink"

// convenient constant for the bundle identifier, can be used if you make use of the GMILocalizer class
#define GMI_RES_BUNDLE_IDENTIFIER @"GMI_Res"

// various error codes the SDK can return.
#define ILLEGAL_ARG_CODE -808 // returned when you pass a bad argument to one of the GMIClient methods. The GMIError object with this code will contain more information on what parameters were bad and why.
#define BAD_ENROLL_OR_VERIFY_JS_CALL -1001 // returned when you make an enrollment or verification request that's ill-formed. For more information on correctly-formed verification requests, refer to sample applications or the SDK documentation about making your own requests. The easiest way to avoid this error is to just use the message rendering interface, though.

// special UI case values
#define GMI_HEIGHT_IPHONE_5 568
#define GMI_ON_IPHONE_5 ([[UIScreen mainScreen] bounds].size.height == GMI_HEIGHT_IPHONE_5)
#define GMI_ON_IPAD     UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad

#define gmi_paramOrNil(param) ([param isEqual:[NSNull null]] || param == nil ? nil : param)

typedef NS_ENUM(NSInteger, DeviceRegistrationStatus) {
    FAILED = 0, SUCCESSFUL, PENDING
};

// definition of enums for different device types
typedef NS_ENUM(NSInteger, CaptureDeviceType) {
    UNKNOWN, CAMERA, VIDEO, MICROPHONE, FINGERPRINT_SCANNER, LIVE_CAM_MIC
};

typedef NSString* CaptureType;

typedef NS_ENUM(NSInteger, BioType) {
    FACE_PIC = 40, VOICE_PASS_REC = 60, LIVE_VOICE_PASS_REC = 61, FINGERPRINT_SCAN = 9001, FACEVOICE = 9002,LIVE_FACE = 40,
    YESNO = 9003, MULTI = -1,HTPALM = 150 , RRPALM = 152, PIN = 141, RANK_FACE = 42, FACETEC_FACE = 43,
    INVALID = -90, LEGACY_PALM_L = 151, LEGACY_PALM_R = 150
};

// conveniently get the Obj-C object for a given BioType
#define objBioType(biotype) [NSNumber numberWithLong:biotype]

// camera orientations
typedef NS_ENUM(NSInteger, Orientation) {
    LANDSCAPE, PORTRAIT
};

// biometric capture types
typedef NS_ENUM(NSInteger, BioEventType){
    ENROLLMENT, VERIFICATION, IDENTIFICATION
};

typedef NS_ENUM(NSUInteger, DefaultCamera) {
    FRONT, BACK
};

typedef NSString* GMIPermissionStatus;

#define KEY_PALM_SEL_HAND @"selected_hand"

typedef NSString* GMIOAuthToken;

#define GMIPermissionStatusOK @"ok"
#define GMIPermissionStatusDenied @"sdk.granular_permissions_denied"
#define GMIPermissionStatusNoPrints @"sdk.no_registered_fingerprints"
#define GMIPermissionStatusNoTouchID @"sdk.no_fingerprint_support"

#define GMILeftHandImage @"LeftHand"
#define GMIRightHandImage @"RightHand"

#define ENROLL_DATA @"enrollData"
#define VOICE_DIGITS @"voiceDigits" // we include this in the metadata for the new voice digit prompt setup that replaced liveness

//DTTImer keys
#define TIMER_NAME_PALM_TIMEOUT @"com.timer.palmimage.timeout"
#define TIMER_KEY_WEBTIMEOUT @"timer.key.web.timeout"

#define DO_ONCE(b) { static dispatch_once_t onceToken; dispatch_once(&onceToken,^(){b} );}






