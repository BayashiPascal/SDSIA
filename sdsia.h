// ============ SDSIA.H ================

#ifndef SDSIA_H
#define SDSIA_H

// ================= Include =================

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <execinfo.h>
#include <errno.h>
#include <string.h>
#include "pberr.h"
#include "gset.h"
#include "pbjson.h"
#include "genbrush.h"
#include "pbfilesys.h"

// ================= Define ==================

#define SDSIA_DESCFILENAME "dataset.json"

// ================= Data structure ===================

typedef struct SDSIAPairName {
  // Names of img and mask
  char* _imgName;
  char* _maskName;
} SDSIAPairName;

typedef struct SDSIASample {
  // Image and mask
  GenBrush* _img;
  GenBrush* _mask;
} SDSIASample;

typedef struct SDSIA {
  // Path to the folder of the data set
  char* _folderPath;
  // Name of the data set
  char* _name;
  // Description of the data set
  char* _desc;
  // Format of images and masks
  char* _format;
  // Type of set
  int _type;
  // Dimensions of images and masks
  VecShort2D _dim;
  // Set of SDSIAPairName
  GSet _samples;
  // Splitting of samples
  VecLong* _split;
  // Sets of splitted samples
  GSet* _categories;
  // Iterators on the sets of splitted samples
  GSetIterForward* _iterators;
} SDSIA;

// ================ Functions declaration ====================

// Create a new SDSIA with the content of the directory 'folderPath'
// The random generator must have been initialized before calling 
// this function
SDSIA SDSIACreateStatic(const char* const folderPath);

// Free the memory used by a SDSIA
void SDSIAFreeStatic(SDSIA* const that);

// Get the total number of images in the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
long SDSIAGetSize(const SDSIA* const that);

// Get the total number of images/masks in the SDSIA 'that' for the 
// category 'iCat'. Return 0 if the category doesn't exists
#if BUILDMODE != 0
inline
#endif 
long SDSIAGetSizeCat(const SDSIA* const that, const long iCat);

// Split the samples of the SDSIA 'that' into several categories
// defined by 'cat'. The dimension of 'cat' gives the number of 
// categories and the value for each dimension of 'cat' gives the 
// number of samples in the correpsonding category. For example <3,4> 
// would mean 2 categories with 3 samples in the first one and 4 
// samples in the second one. There must me at least as many samples 
// in the data set as the sum of samples in 'cat'.
// Each category must have at least one sample. Samples are allocated // randomly to the categories.
// If 'that' was already splitted the previous splitting is discarded.
void SDSIASplit(SDSIA* const that, const VecLong* const cat);

// Unsplit the SDSIA 'that', i.e. after calling SDSIAUnsplit 'that' 
// has only one category containing all the samples
#if BUILDMODE != 0
inline
#endif 
void SDSIAUnsplit(SDSIA* const that);

// Shuffle the samples of the category 'iCat' of the SDSIA 'that'.
// Reset the iterator of the category
#if BUILDMODE != 0
inline
#endif 
void SDSIAShuffle(SDSIA* const that, const long iCat);

// Shuffle the samples of all the categories of the SDSIA 'that'.
// Reset the iterator of the categories
#if BUILDMODE != 0
inline
#endif 
void SDSIAShuffleAll(SDSIA* const that);

// Get the name of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
const char* SDSIAName(const SDSIA* const that);

// Get the description of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
const char* SDSIADesc(const SDSIA* const that);

// Get the format of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
const char* SDSIAFormat(const SDSIA* const that);

// Get the path of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
const char* SDSIAFolderPath(const SDSIA* const that);

// Get the type of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
int SDSIAGetType(const SDSIA* const that);

// Get the number of categories of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
long SDSIAGetNbCat(const SDSIA* const that);

// Get the dimensions of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D SDSIAGetDim(const SDSIA* const that);
#if BUILDMODE != 0
inline
#endif 
const VecShort2D* SDSIADim(const SDSIA* const that);

// Release the memory used by the SDSIASample 'that'
#if BUILDMODE != 0
inline
#endif 
void SDSIASampleFreeStatic(SDSIASample* const that);

// Get the current sample in the category 'iCat' of the SDSIA 'that'
// The pointers in the returned SDSIASample may be null
// Example of correct usage to loop through all samples:
// SDSIA* sdsia = ...;
// do {
//   SDSIASample sample = SDSIAGetSample(sdsia, 0);
//   ...
//   SDSIASampleFreeStatic(&sample);
// } while(SDSIAStepSample(sdsia, 0));
#if BUILDMODE != 0
inline
#endif 
SDSIASample SDSIAGetSample(const SDSIA* const that, const long iCat);

// If there is a next sample move to the next sample of the category 
// 'iCat' and return true, else return false
#if BUILDMODE != 0
inline
#endif 
bool SDSIAStepSample(const SDSIA* const that, const long iCat);

// Reset the iterator on category 'iCat' of the SDSIA 'that', i.e. 
// the next call to SDSIAGetNextSample will give the first sample of 
// the category 'iCat'
#if BUILDMODE != 0
inline
#endif 
void SDSIAReset(SDSIA* const that, const long iCat);

// Reset the iterator on all categories of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
void SDSIAResetAll(SDSIA* const that);

// ================= Polymorphism ==================

// ================ Inliner ====================

#if BUILDMODE != 0
#include "sdsia-inline.c"
#endif

#endif
