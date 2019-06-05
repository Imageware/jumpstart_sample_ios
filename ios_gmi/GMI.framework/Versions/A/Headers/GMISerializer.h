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
 * Description	: Interface providing de-/serializer methods for the GMI system. The serializer manages de-/serialization of devices, persons, messages and lists of message URIs.
 *
 * Custom Implementations of this interface must implement each of its methods as well.
 */

@class GMIDevice;
@class GMIPerson;
@class GMIXMLTemplate;
@class GMIMessage;

@protocol GMISerializer <NSObject>

@required

/**
 *  Serializes a device, returning an NSString JSON object.
 *
 *  @param device the device to serialize
 *  @param an error set if the serialization fails. This can be NULL if you don't care about the error.
 *
 *  @return a correctly-formatted JSON string, or Nil if there was an error
 */
- (NSString*) serializeDevice: (GMIDevice*) device error: (NSError**) error;

/**
 *  Deserializes a string into a Device.
 *
 *  @param jsonstr the JSON string
 *  @param an error set if the deserialization fails. This can be NULL if you don't care about the error.
 *
 *  @return a GMIDevice built from the passed JSON or Nil if there was an error
 */
- (GMIDevice*) deserializeDevice: (NSString*) jsonstr error: (NSError**) error;

/**
 *  Serializes a person, returning an NSString JSON object.
 *
 *  @param person the person to serialize
 *  @param an error set if the serialization fails. This can be NULL if you don't care about the error.
 *
 *  @return a correctly-formatted JSON string, or Nil if there was an error
 */
- (NSString*) serializePerson: (GMIPerson*) person error: (NSError**) error;

/**
 *  Deserializes a string into a Person.
 *
 *  @param jsonstr the JSON string
 *  @param an error set if the deserialization fails. This can be NULL if you don't care about the error.
 *
 *  @return a GMIPerson built from the passed JSON or Nil if there was an error
 */
- (GMIPerson*) deserializePerson: (NSString*) jsonstr error: (NSError**) error;

#pragma mark Non-JSON serializations

/**
 *  Serializes a GMIMessage, returning the message's JSON String representation.
 *
 *  @param message to serialize
 *  @param an error set if the serialization fails. This can be NULL if you don't care about the error.
 *
 *  @return a correctly-formatted JSON string, or Nil if there was an error
 */
- (NSString*) serializeGMIMessage: (GMIMessage*) message error: (NSError**) error;

/**
 *  Deserializes the passed JSON string, turning it into a GMIMessage. The message's template url is populated, but the actual GMIXMLTemplate will not be in the case of an XML template.
 */
- (GMIMessage*) deserializeGMIMessage: (NSString*) jsonstr error: (NSError**) error;

/**
 *  Deserializes the passed NSDictionary, turning it into a GMIMessage. The message's template url is populated, but the actual GMIXMLTemplate will not be in the case of an XML template.
 */
- (GMIMessage*) deserializeGMIMessageFromDict: (NSDictionary*) dict error: (NSError**) error;

/**
 *  Serializes an array, returning an equivalent JSON string.
 *
 *  @param array to serialize
 *
 *  @return the array but as JSON!
 */
- (NSString*) serializeJSONArray: (NSArray*) array error:(NSError *__autoreleasing *)error;

/**
 *  Deserializes a string into an NSArray.
 *
 *  @param str a json array string
 *
 *  @return an NSArray built from the passed string, or nil if there's an error
 */
- (NSArray*) deserializeJSONArray: (NSString*) str error:(NSError *__autoreleasing *)error;

/**
 *  Turns the passed message metadata string into an NSDictionary if possible, setting the passed error if there is an error along the way.
 *
 *  @param metadat the metadata to deserialize
 * 
 *  @return an NSDictionary representing the JSON in the metadata, or nil. If there is an error, this will return nil and the error will be set.
 */
- (NSMutableDictionary*) deserializeMetadata: (NSString*) metadata error: (NSError**) error;

/**
 *  Turns the passed message metadata string into an NSDictionary if possible, setting the passed error if there is an error along the way.
 *
 *  @param metadat the metadata to deserialize
 *
 *  @return an NSDictionary representing the JSON in the metadata, or nil. If there is an error, this will return nil and the error will be set.
 */
- (NSDictionary *) deserializeAlgorithmList:(NSString *)jsonstr error:(NSError *__autoreleasing *)error;


@end
