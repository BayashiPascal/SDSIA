// ============ SDSIA_INLINE.C ================

// ================ Functions implementation ====================

// Shuffle the samples of the category 'iCat' of the SDSIA 'that'.
// Reset the iterator of the category
#if BUILDMODE != 0
inline
#endif 
void SDSIAShuffle(SDSIA* const that, const long iCat) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
  if (that->_categories == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that->_categories' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
  if (iCat < 0 || iCat >= SDSIAGetNbCat(that)) {
    SDSIAErr->_type = PBErrTypeInvalidArg;
    sprintf(PBImgAnalysisErr->_msg, "'iCat' is invalid (0<=%ld<%ld)",
      iCat, SDSIAGetNbCat(that));
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  // Shuffle the GSet of the category
  GSetShuffle(that->_categories + iCat);
  // Reset the iterator
  SDSIAReset(that, iCat);
}

// Shuffle the samples of all the categories of the SDSIA 'that'.
// Reset the iterator of the categories
#if BUILDMODE != 0
inline
#endif 
void SDSIAShuffleAll(SDSIA* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  // Loop on categories
  for (int iCat = SDSIAGetNbCat(that); iCat--;)
    // Shuffle the category
    SDSIAShuffle(that, iCat);
}

// Unsplit the SDSIA 'that', i.e. after calling SDSIAUnsplit 'that' 
// has only one category containing all the samples
#if BUILDMODE != 0
inline
#endif 
void SDSIAUnsplit(SDSIA* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  // Unsplitting is equivalent to splitting in one category with all the 
  // samples
  VecLong* split = VecLongCreate(1);
  VecSet(split, 0, SDSIAGetSize(that));
  SDSIASplit(that, split);
  VecFree(&split);
}

// Get the total number of images in the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
long SDSIAGetSize(const SDSIA* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  return GSetNbElem(&(that->_samples));
}

// Get the total number of images/masks in the SDSIA 'that' for the 
// category 'iCat'. Return 0 if the category doesn't exists
#if BUILDMODE != 0
inline
#endif 
long SDSIAGetSizeCat(const SDSIA* const that, const long iCat) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
  if (that->_split == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that->_split' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
  if (iCat < 0 || iCat >= SDSIAGetNbCat(that)) {
    SDSIAErr->_type = PBErrTypeInvalidArg;
    sprintf(PBImgAnalysisErr->_msg, "'iCat' is invalid (0<=%ld<%ld)",
      iCat, SDSIAGetNbCat(that));
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  return VecGet(that->_split, iCat);
}

// Get the name of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
const char* SDSIAName(const SDSIA* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  return that->_name;
}

// Get the description of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
const char* SDSIADesc(const SDSIA* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  return that->_desc;
}

// Get the format of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
const char* SDSIAFormat(const SDSIA* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  return that->_format;
}

// Get the path of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
const char* SDSIAFolderPath(const SDSIA* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  return that->_folderPath;
}

// Get the type of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
int SDSIAGetType(const SDSIA* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  return that->_type;
}

// Get the number of categories of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
long SDSIAGetNbCat(const SDSIA* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  return VecGetDim(that->_split);
}

// Get the dimensions of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D SDSIAGetDim(const SDSIA* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  return that->_dim;
}
#if BUILDMODE != 0
inline
#endif 
const VecShort2D* SDSIADim(const SDSIA* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  return &(that->_dim);
}

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
SDSIASample SDSIAGetSample(const SDSIA* const that, const long iCat) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
  if (that->_iterators == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that->_iterators' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
  if (iCat < 0 || iCat >= SDSIAGetNbCat(that)) {
    SDSIAErr->_type = PBErrTypeInvalidArg;
    sprintf(PBImgAnalysisErr->_msg, "'iCat' is invalid (0<=%ld<%ld)",
      iCat, SDSIAGetNbCat(that));
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  SDSIAPairName* pair = GSetIterGet(that->_iterators + iCat);
  SDSIASample sample;
  char *path = PBFSJoinPath(SDSIAFolderPath(that), pair->_imgName);
  sample._img = GBCreateFromFile(path);
  GBSetFileName(sample._img, path);
  free(path);
  path = PBFSJoinPath(SDSIAFolderPath(that), pair->_maskName);
  sample._mask = GBCreateFromFile(path);
  GBSetFileName(sample._mask, path);
  free(path);
  return sample;
}

// If there is a next sample move to the next sample of the category 
// 'iCat' and return true, else return false
#if BUILDMODE != 0
inline
#endif 
bool SDSIAStepSample(const SDSIA* const that, const long iCat) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
  if (that->_iterators == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that->_iterators' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
  if (iCat < 0 || iCat >= SDSIAGetNbCat(that)) {
    SDSIAErr->_type = PBErrTypeInvalidArg;
    sprintf(PBImgAnalysisErr->_msg, "'iCat' is invalid (0<=%ld<%ld)",
      iCat, SDSIAGetNbCat(that));
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  return GSetIterStep(that->_iterators + iCat);
}

// Reset the iterator on category 'iCat' of the SDSIA 'that', i.e. 
// the next call to SDSIAGetNextSample will give the first sample of 
// the category 'iCat'
#if BUILDMODE != 0
inline
#endif 
void SDSIAReset(SDSIA* const that, const long iCat) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
  if (that->_iterators == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that->_iterators' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
  if (iCat < 0 || iCat >= SDSIAGetNbCat(that)) {
    SDSIAErr->_type = PBErrTypeInvalidArg;
    sprintf(PBImgAnalysisErr->_msg, "'iCat' is invalid (0<=%ld<%ld)",
      iCat, SDSIAGetNbCat(that));
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  GSetIterReset(that->_iterators + iCat);
}

// Reset the iterator on all categories of the SDSIA 'that'
#if BUILDMODE != 0
inline
#endif 
void SDSIAResetAll(SDSIA* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  // Loop on categories
  for (int iCat = SDSIAGetNbCat(that); iCat--;)
    // Shuffle the category
    SDSIAReset(that, iCat);
}

// Release the memory used by the SDSIASample 'that'
#if BUILDMODE != 0
inline
#endif 
void SDSIASampleFreeStatic(SDSIASample* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  GBFree(&(that->_img));
  GBFree(&(that->_mask));
}

