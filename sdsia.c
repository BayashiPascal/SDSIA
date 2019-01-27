// ============ SDSIA.C ================

// ================= Include =================

#include "sdsia.h"
#if BUILDMODE == 0
#include "sdsia-inline.c"
#endif

// ================= Define ==================

// ================ Functions declaration ====================


// ================ Functions implementation ====================

// Create a new SDSIA with the content of the directory 'folderPath'
// The random generator must have been initialized before calling 
// this function
SDSIA SDSIACreateStatic(const char* const folderPath) {
#if BUILDMODE == 0
  if (folderPath == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'folderPath' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  // Declare the new SDSIA
  SDSIA that;
  // Get the name of the description file
  that._folderPath = PBFSJoinPath(folderPath);
  char* descFileName = PBFSJoinPath(folderPath, SDSIA_DESCFILENAME);
  // Open the description file
  FILE* descFile = fopen(descFileName, "r");
  // If the description file doesn't exist
  if (descFile == NULL) {
    SDSIAErr->_type = PBErrTypeInvalidArg;
    sprintf(SDSIAErr->_msg, "Can't open the description file");
    PBErrCatch(SDSIAErr);
  }
  // Declare a json to load the encoded data
  JSONNode* json = JSONCreate();
  // Load the whole encoded data
  if (JSONLoad(json, descFile) == false) {
    printf("%s\n", SDSIAErr->_msg);
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "Can't load the description file");
    PBErrCatch(SDSIAErr);
  }
  // Decode dataSet
  JSONNode* prop = JSONProperty(json, "dataSet");
  if (prop == NULL) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, 
      "Invalid description file (dataSet missing)");
    PBErrCatch(SDSIAErr);
  }
  JSONNode* val = JSONValue(prop, 0);
  that._name = PBErrMalloc(SDSIAErr, 
    sizeof(char) * (strlen(JSONLabel(val)) + 1));
  strcpy(that._name, JSONLabel(val));
  // Decode desc
  prop = JSONProperty(json, "desc");
  if (prop == NULL) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, 
      "Invalid description file (desc missing)");
    PBErrCatch(SDSIAErr);
  }
  val = JSONValue(prop, 0);
  that._desc = PBErrMalloc(SDSIAErr, 
    sizeof(char) * (strlen(JSONLabel(val)) + 1));
  strcpy(that._desc, JSONLabel(val));
  // Decode format
  prop = JSONProperty(json, "format");
  if (prop == NULL) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, 
      "Invalid description file (format missing)");
    PBErrCatch(SDSIAErr);
  }
  val = JSONValue(prop, 0);
  that._format = PBErrMalloc(SDSIAErr, 
    sizeof(char) * (strlen(JSONLabel(val)) + 1));
  strcpy(that._format, JSONLabel(val));
  // Decode dataSetType
  prop = JSONProperty(json, "dataSetType");
  if (prop == NULL) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, 
      "Invalid description file (dataSetType missing)");
    PBErrCatch(SDSIAErr);
  }
  val = JSONValue(prop, 0);
  that._type = atoi(JSONLabel(val));
  // Decode dim
  prop = JSONProperty(json, "dim");
  if (prop == NULL) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, 
      "Invalid description file (dim missing)");
    PBErrCatch(SDSIAErr);
  }
  if (JSONGetNbValue(prop) != 2) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, 
      "Invalid description file (dim's dimension != 2)");
    PBErrCatch(SDSIAErr);
  }
  that._dim = VecShortCreateStatic2D();
  VecSet(&(that._dim), 0, atoi(JSONLabel(JSONValue(prop, 0))));
  VecSet(&(that._dim), 1, atoi(JSONLabel(JSONValue(prop, 1))));
  // Decode nbImg
  prop = JSONProperty(json, "nbImg");
  if (prop == NULL) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, 
      "Invalid description file (nbImg missing)");
    PBErrCatch(SDSIAErr);
  }
  val = JSONValue(prop, 0);
  int nbImg = atoi(JSONLabel(val));
  // Decode images
  prop = JSONProperty(json, "images");
  if (prop == NULL) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, 
      "Invalid description file (images missing)");
    PBErrCatch(SDSIAErr);
  }
  if (JSONGetNbValue(prop) != nbImg) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, 
      "Invalid description file (images's dimension != nbImg)");
    PBErrCatch(SDSIAErr);
  }
  that._samples = GSetCreateStatic();
  for (int iImg = 0; iImg < nbImg; ++iImg) {
    val = JSONValue(prop, iImg);
    // Allocate memory for the pair image/mask
    SDSIAPairName* pair = PBErrMalloc(SDSIAErr, sizeof(SDSIAPairName));
    // Decode img
    JSONNode* subProp = JSONProperty(val, "img");
    if (subProp == NULL) {
      SDSIAErr->_type = PBErrTypeUnitTestFailed;
      sprintf(SDSIAErr->_msg, 
        "Invalid description file (images.img missing)");
      PBErrCatch(SDSIAErr);
    }
    JSONNode* subVal = JSONValue(subProp, 0);
    pair->_imgName = PBErrMalloc(SDSIAErr, 
      sizeof(char) * (strlen(JSONLabel(subVal)) + 1));
    strcpy(pair->_imgName, JSONLabel(subVal));
    // Decode mask
    subProp = JSONProperty(val, "mask");
    if (subProp == NULL) {
      SDSIAErr->_type = PBErrTypeUnitTestFailed;
      sprintf(SDSIAErr->_msg, 
        "Invalid description file (images.mask missing)");
      PBErrCatch(SDSIAErr);
    }
    subVal = JSONValue(subProp, 0);
    pair->_maskName = PBErrMalloc(SDSIAErr, 
      sizeof(char) * (strlen(JSONLabel(subVal)) + 1));
    strcpy(pair->_maskName, JSONLabel(subVal));
    GSetAppend(&(that._samples), pair);
  }
  that._split = NULL;
  that._categories = NULL;
  that._iterators = NULL;
  VecLong* split = VecLongCreate(1);
  VecSet(split, 0, nbImg);
  SDSIASplit(&that, split);
  VecFree(&split);
  
  // Free the memory used by the JSON
  JSONFree(&json);
  // Close the description file
  fclose(descFile);
  // Free memory
  free(descFileName);
  
  // Return the new SDSIA
  return that;
}

// Free the memory used by a SDSIA
void SDSIAFreeStatic(SDSIA* const that) {
  if (that == NULL)
    return;
  // Free memory
  free(that->_name);
  free(that->_desc);
  free(that->_format);
  free(that->_folderPath);
  for (int iCat = SDSIAGetNbCat(that); iCat--;) {
    GSetFlush(that->_categories + iCat);
  }
  free(that->_categories);
  free(that->_iterators);
  VecFree(&(that->_split));
  if (SDSIAGetSize(that) > 0) {
    do {
      SDSIAPairName* pair = GSetPop(&(that->_samples));
      if (pair->_imgName != NULL)
        free(pair->_imgName);
      if (pair->_maskName != NULL)
        free(pair->_maskName);
      free(pair);
    } while (SDSIAGetSize(that) > 0);
  }
}

// Split the samples of the SDSIA 'that' into several categories
// defined by 'cat'. The dimension of 'cat' gives the number of 
// categories and the value for each dimension of 'cat' gives the 
// number of samples in the correpsonding category. For example <3,4> 
// would mean 2 categories with 3 samples in the first one and 4 
// samples in the second one. There must me at least as many samples 
// in the data set as the sum of samples in 'cat'.
// Each category must have at least one sample. Samples are allocated // randomly to the categories.
// If 'that' was already splitted the previous splitting is discarded.
void SDSIASplit(SDSIA* const that, const VecLong* const cat) {
#if BUILDMODE == 0
  if (that == NULL) {
    SDSIAErr->_type = PBErrTypeNullPointer;
    sprintf(PBImgAnalysisErr->_msg, "'that' is null");
    PBErrCatch(PBImgAnalysisErr);
  }
  long nb = 0;
  for (long iCat = VecGetDim(cat); iCat--;)
    nb += VecGet(cat, iCat);
  if (nb > SDSIAGetSize(that)) {
    SDSIAErr->_type = PBErrTypeInvalidArg;
    sprintf(PBImgAnalysisErr->_msg, 
      "Not enough images for the requested splitting");
    PBErrCatch(PBImgAnalysisErr);
  }
#endif
  // Free the current splitting if necessary
  if (that->_categories != NULL) {
    if (that->_split != NULL) {
      for (int iCat = SDSIAGetNbCat(that); iCat--;) {
        GSetFlush(that->_categories + iCat);
      }
    }
    free(that->_categories);
  }
  if (that->_iterators)
    free(that->_iterators);
  VecFree(&(that->_split));
  // Get the number of categories
  long nbCat = VecGetDim(cat);
  // Allocate memory for the categories
  that->_categories = PBErrMalloc(SDSIAErr, sizeof(GSet) * nbCat);
  for (long iCat = nbCat; iCat--;) {
    that->_categories[iCat] = GSetCreateStatic();
  }
  // Copy the splitting
  that->_split = VecClone(cat);
  // Shuffle the samples
  GSetShuffle(&(that->_samples));
  // Declare an iterator on the samples
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_samples));
  // Loop on categories
  for (long iCat = nbCat; iCat--;) {
    // Get the nb of samples for this category
    long nbSample = VecGet(cat, iCat);
    // Loop on the sample
    for (long iSample = nbSample; iSample--; GSetIterStep(&iter)) {
      // Get the next sample
      SDSIAPairName* pair = GSetIterGet(&iter);
      // Add the sample to the category
      GSetAppend(that->_categories + iCat, pair);
    }
  }
  // Allocate memory for the iterators
  that->_iterators = PBErrMalloc(SDSIAErr, 
    sizeof(GSetIterForward) * nbCat);
  for (long iCat = nbCat; iCat--;) {
    that->_iterators[iCat] = 
      GSetIterForwardCreateStatic(that->_categories + iCat);
  }
}

