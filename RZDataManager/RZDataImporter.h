//
//  RZDataImporter.h
//
//  Created by Nick Donaldson on 2/26/13.
//  Copyright (c) 2013 Raizlabs. All rights reserved.
//

#import <Foundation/Foundation.h>

@class RZDataManager;

@protocol RZDataImporterModelObject <NSObject>

@optional

//! Implement this method to return a cached object matching the passed-in unique key-value pair
+ (id)cachedObjectWithUniqueValue:(id)uniqueValue forKeyPath:(id)key;

//! Implement this method to prepare the object to be updated with new data
- (void)prepareForImportFromData:(NSDictionary*)data;

//! Implement this method to finalize the import and send "updated" notifications
- (void)finalizeImportFromData:(NSDictionary*)data;

@end

@class RZDataImporterDiffInfo;

@interface RZDataImporter : NSObject

//! Weak reference to parent Data Manager
@property (nonatomic, weak) RZDataManager *dataManager;

//! Set to override on importer-level whether should decode HTML entities from strings (defaults to NO)
@property (nonatomic, assign) BOOL shouldDecodeHTML;

//! Externally set a mapping dictionary for a particular class
- (void)setMapping:(NSDictionary*)mapping forClassNamed:(NSString*)className;

//! Import data from a dictionary to an object based on a provided (or TBD: assumed) mapping. Can provide a type name (maybe useful for
//  managed objects with entity name different from class) or infer name from class.
- (void)importData:(NSDictionary*)data toObject:(NSObject<RZDataImporterModelObject>*)object;
- (void)importData:(NSDictionary*)data toObject:(NSObject<RZDataImporterModelObject>*)object ofType:(NSString*)objTypeName;

//! Return array of indices for objects that have been inserted, deleted, or moved, based on passed-in object array and data array. Basically calculates diff-update info of model object collection with incoming data set.
/*!
    objects - array of objects to update. Should be of type objClass.
 data - either an array of dictionaries or a dictionary containing data with which to update objects
    dataIdKeyPath - key path to value uniquely identifying object in raw dictionary
    modelIdKeyPath - key path to value uniquely identifying model object. Value will be compared against value for dataIdKey in raw data
*/
- (RZDataImporterDiffInfo*)diffInfoForObjects:(NSArray*)objects
                                     withData:(id)data
                                dataIdKeyPath:(NSString*)dataIdKeyPath
                               modelIdKeyPath:(NSString*)modelIdKeyPath;


@end


@interface RZDataImporterDiffInfo : NSObject

// Indices at which new objects are present in data array
@property (strong, nonatomic) NSMutableArray* insertedObjectIndices;

// Indices in the object array of objects that should be removed (not present in data array)
@property (strong, nonatomic) NSMutableArray* removedObjectIndices;

// Indices for objects that have moved (the indices in the data array, which represent the final index of the object after the update)
@property (strong, nonatomic) NSMutableArray* movedObjectIndices;

@end