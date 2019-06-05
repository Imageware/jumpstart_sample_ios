// Copyright © 2017 RedRock Biometrics. All rights reserved.
#ifndef __PALM_FRAME_H__
#define __PALM_FRAME_H__

#ifndef PALM_EXPORT
#ifdef _MSC_VER
#define PALM_EXPORT __declspec(dllexport)
#else
#define PALM_EXPORT __attribute__((visibility("default")))
#endif
#endif

#ifndef PALM_CALL
#ifdef _MSC_VER
#define PALM_CALL __stdcall
#else
#define PALM_CALL
#endif
#endif

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Possible return values from the various API functions.
 */
typedef enum _ePalmStatus {
  /**
   * The desired operation completed successfully.
   */
  ePalm_Success = 0,

  /**
   * The function timed-out before completing the desired operation.
   */
  ePalm_Timeout = 1,

  /**
   * There was an error completing the desired operation, but for an unknown
   * reasons.
   */
  ePalm_UnknownError = 0x8000,

  /**
   * An operation was requested when it was not expected.
   */
  ePalm_InvalidHandle = 0x8001,

  /**
   * One of the provided arguments was invalid or incomplete.
   */
  ePalm_InvalidArgument = 0x8002,

  /**
   * Insufficient memory in buffer.
   */
  ePalm_OutOfMemory = 0x8003,

  /**
   * An operation was requested when it was not expected.
   */
  ePalm_UnexpectedRequest = 0x8004,

  /**
   * The license id used was invalid.
   */
  ePalm_InvalidLicense = 0x8005,

  /**
   * The model does not exist or is invalid.
   */
  ePalm_InvalidModel = 0x8006,

  /**
   * Error connecting to API server.
   */
  ePalm_ServerConnectionError = 0x8007,

  /**
  * Error in data serialization.
  */
  ePalm_SerializationError = 0x8008
} ePalmStatus;

/**
* Possible palm image types
*/
typedef enum _ePalmImageType {
  /*Unknown*/
  ePalmImage_Unknown = 0,

  /*8 bits gray scale*/
  ePalmImage_Gray = 1,

  /*24 bits RGB*/
  ePalmImage_RGB = 2,

  /*8 bits infra-red*/
  ePalmImage_IR = 3
} ePalmImageType;

/**
* Camera settings of the device used to acquire the image frame.
*/
typedef struct _CameraSettings {
  /**
  * Size of this structure in byte.
  */
  uint32_t size;

  /**
  * The camera gain setting.
  */
  int32_t gain;

  /**
  * The camera shutter setting.
  */
  int32_t shutter;

  /**
  * The camera brightness setting.
  */
  int32_t brightness;

  /**
  * The camera focus setting.
  */
  int32_t focus;
} CameraSettings;

typedef struct _PalmImage {
  /**
   * Size of this structure in byte.
   */
  uint32_t size;

  /**
   * Type of the image.
   */
  ePalmImageType type;

  /**
   * Pointer to the raw image data.
   */
  uint8_t* data;

  /**
   * Width of image in pixels.
   */
  uint32_t width;

  /**
   * Height of image in pixels.
   */
  uint32_t height;

  /**
   * Size of a row of pixels in bytes, including any padding at end.
   */
  uint32_t stride;

  /**
   * Number of color planes.
   */
  uint32_t planes;

  /**
   * Color depth of image in bits.
   */
  uint32_t depth;

  /**
   * Number of bytes offset from the beginning of data to the start of image.
   */
  uint32_t offset;

  /**
   * The time at which the frame was captured in microseconds.
   */
  int64_t timestamp;

  /**
   * Camera settings of the device used to capture this frame.
   */
  CameraSettings camera_settings;
} PalmImage;

/**
 * A camera device frame.
 */
typedef struct _PalmFrame {
  /**
   * Size of this structure in byte.
   */
  uint32_t size;

  /**
   * Unique frame identifier for this frame.
   */
  int64_t frame_id;

  /**
  * Number of images associated with this frame.
  */
  uint32_t num_images;

  /**
   * Pointers to images associated with this frame. Support up to 8 images but only the first 'num_images' are allocated and used.
   */
  PalmImage* images[8];
} PalmFrame;

/**
* Insert a palm image to a palm frame. This function creates a new palm image of specified type and size and allocates memories
* for its image data buffer. If the input frame already contains the maximum number (8) images, this function does nothing.
*
* @param frame  A palm frame to add the new image.
*        width  Image width
*        height Image height
*        type   Image type. This also determines 'depth' which equals 8 * (number of channels). For instance, ePalm_RGB is 24 bits.
*
* @note call this function only if you intend to allocate memory for the palm image inside Palm API shared library (other than
*       inside the app).
*/
PALM_EXPORT void PALM_CALL PalmFrame_AddImage(PalmFrame* frame, uint32_t width, uint32_t height, ePalmImageType type);

/**
* Create an empty palm frame with no images.
*
* @param frame      A pointer to a palm frame that will be initialized as an empty frame with no images.

* @note call PalmFrame_AddImage to insert an image into a frame. Note that the memory for the palm frame and images are allocated
*       inside the PalmAPI shared library.
*/
PALM_EXPORT void PALM_CALL PalmFrame_Create(PalmFrame* frame);

/**
* Free a palm frame and its images.
*
* @param frame A palm frame allocated inside Palm API shared library.
*
* @note This function releases the memories allocated for a frame and all its images.
*/
PALM_EXPORT void PALM_CALL PalmFrame_Free(PalmFrame* frame);

#ifdef __cplusplus
}
#endif

#endif /* __PALM_FRAME_H__ */
