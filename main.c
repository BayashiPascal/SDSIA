#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "sdsia.h"

void UnitTestSDSIACreateFree() {
  srandom(1);
  char* testFolder = PBFSJoinPath(".", "UnitTestOut", "001", "001");
  SDSIA sdsia = SDSIACreateStatic(testFolder);
  VecLong* split = VecLongCreate(1);
  VecSet(split, 0, 3);
  if (strcmp(sdsia._name, "dataset-001-001") != 0 ||
    strcmp(sdsia._folderPath, testFolder) != 0 ||
    strcmp(sdsia._desc, "unitTest") != 0 ||
    sdsia._type != 0 ||
    VecGet(&(sdsia._dim), 0) != 10 ||
    VecGet(&(sdsia._dim), 1) != 20 ||
    GSetNbElem(&(sdsia._samples)) != 3 ||
    strcmp(((SDSIAPairName*)GSetGet(&(sdsia._samples), 0))->_imgName, 
      "img001.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(&(sdsia._samples), 0))->_maskName, 
      "mask001.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(&(sdsia._samples), 1))->_imgName, 
      "img002.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(&(sdsia._samples), 1))->_maskName, 
      "mask002.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(&(sdsia._samples), 2))->_imgName, 
      "img000.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(&(sdsia._samples), 2))->_maskName, 
      "mask000.tga") != 0 ||
    VecIsEqual(split, sdsia._split) == false ||
    GSetNbElem(sdsia._categories) != 3 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 0))->_imgName, 
      "img001.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 0))->_maskName, 
      "mask001.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 1))->_imgName, 
      "img002.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 1))->_maskName, 
      "mask002.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 2))->_imgName, 
      "img000.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 2))->_maskName, 
      "mask000.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetIterGet(sdsia._iterators))->_imgName, 
      "img001.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetIterGet(sdsia._iterators))->_maskName, 
      "mask001.tga") != 0 ||
    strcmp(sdsia._format, "tga") != 0) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIACreateStatic failed");
    PBErrCatch(SDSIAErr);
  }
  SDSIAFreeStatic(&sdsia);
  free(testFolder);
  VecFree(&split);
  printf("UnitTestSDSIACreateFree OK\n");
}

void UnitTestSDSIAGet() {
  srandom(1);
  char* testFolder = PBFSJoinPath(".", "UnitTestOut", "001", "001");
  SDSIA sdsia = SDSIACreateStatic(testFolder);
  if (strcmp(SDSIADesc(&sdsia), "unitTest") != 0) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIADesc failed");
    PBErrCatch(SDSIAErr);
  }
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 10); VecSet(&dim, 1, 20);
  if (VecIsEqual(SDSIADim(&sdsia), &dim) == false) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIADim failed");
    PBErrCatch(SDSIAErr);
  }
  VecShort2D sdsiadim = SDSIAGetDim(&sdsia);
  if (VecIsEqual(&sdsiadim, &dim) == false) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAGetDim failed");
    PBErrCatch(SDSIAErr);
  }
  if (strcmp(SDSIAFolderPath(&sdsia), testFolder) != 0) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAFolderPath failed");
    PBErrCatch(SDSIAErr);
  }
  if (strcmp(SDSIAFormat(&sdsia), "tga") != 0) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAFormat failed");
    PBErrCatch(SDSIAErr);
  }
  if (SDSIAGetNbCat(&sdsia) != 1) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAGetNbCat failed");
    PBErrCatch(SDSIAErr);
  }
  if (SDSIAGetSize(&sdsia) != 3) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAGetSize failed");
    PBErrCatch(SDSIAErr);
  }
  if (SDSIAGetSizeCat(&sdsia, 0) != 3) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAGetSizeCat failed");
    PBErrCatch(SDSIAErr);
  }
  if (SDSIAGetType(&sdsia) != 0) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAGetType failed");
    PBErrCatch(SDSIAErr);
  }
  if (strcmp(SDSIAName(&sdsia), "dataset-001-001") != 0) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAName failed");
    PBErrCatch(SDSIAErr);
  }
  SDSIAFreeStatic(&sdsia);
  free(testFolder);
  printf("UnitTestSDSIAGet OK\n");
}

void UnitTestSDSIASplitUnsplitShuffle() {
  srandom(1);
  char* testFolder = PBFSJoinPath(".", "UnitTestOut", "001", "001");
  SDSIA sdsia = SDSIACreateStatic(testFolder);
  SDSIAShuffle(&sdsia, 0);
  if (strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 0))->_imgName, 
      "img000.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 0))->_maskName, 
      "mask000.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 1))->_imgName, 
      "img001.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 1))->_maskName, 
      "mask001.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 2))->_imgName, 
      "img002.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 2))->_maskName, 
      "mask002.tga") != 0) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAShuffle failed");
    PBErrCatch(SDSIAErr);
  }
  VecLong2D split = VecLongCreateStatic2D();
  VecSet(&split, 0, 1);
  VecSet(&split, 1, 2);
  SDSIASplit(&sdsia, (VecLong*)&split);
  if (SDSIAGetNbCat(&sdsia) != 2 ||
    GSetNbElem(sdsia._categories) != 1 ||
    GSetNbElem(sdsia._categories + 1) != 2 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 0))->_imgName, 
      "img002.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 0))->_maskName, 
      "mask002.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(
      sdsia._categories + 1, 0))->_imgName, "img000.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(
      sdsia._categories + 1, 0))->_maskName, "mask000.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(
      sdsia._categories + 1, 1))->_imgName, "img001.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(
      sdsia._categories + 1, 1))->_maskName, "mask001.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetIterGet(sdsia._iterators))->_imgName, 
      "img002.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetIterGet(sdsia._iterators))->_maskName, 
      "mask002.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetIterGet(
      sdsia._iterators + 1))->_imgName, "img000.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetIterGet(
      sdsia._iterators + 1))->_maskName, "mask000.tga") != 0) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIACreateStatic failed");
    PBErrCatch(SDSIAErr);
  }
  SDSIAShuffleAll(&sdsia);
  if (SDSIAGetNbCat(&sdsia) != 2 ||
    GSetNbElem(sdsia._categories) != 1 ||
    GSetNbElem(sdsia._categories + 1) != 2 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 0))->_imgName, 
      "img002.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 0))->_maskName, 
      "mask002.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(
      sdsia._categories + 1, 0))->_imgName, "img001.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(
      sdsia._categories + 1, 0))->_maskName, "mask001.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(
      sdsia._categories + 1, 1))->_imgName, "img000.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(
      sdsia._categories + 1, 1))->_maskName, "mask000.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetIterGet(sdsia._iterators))->_imgName, 
      "img002.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetIterGet(sdsia._iterators))->_maskName, 
      "mask002.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetIterGet(
      sdsia._iterators + 1))->_imgName, "img001.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetIterGet(
      sdsia._iterators + 1))->_maskName, "mask001.tga") != 0) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIACreateStatic failed");
    PBErrCatch(SDSIAErr);
  }
  SDSIAUnsplit(&sdsia);
  if (SDSIAGetNbCat(&sdsia) != 1 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 0))->_imgName, 
      "img000.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 0))->_maskName, 
      "mask000.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 1))->_imgName, 
      "img001.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 1))->_maskName, 
      "mask001.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 2))->_imgName, 
      "img002.tga") != 0 ||
    strcmp(((SDSIAPairName*)GSetGet(sdsia._categories, 2))->_maskName, 
      "mask002.tga") != 0) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAShuffle failed");
    PBErrCatch(SDSIAErr);
  }
  SDSIAFreeStatic(&sdsia);
  free(testFolder);
  printf("UnitTestSDSIASplitUnsplitShuffle OK\n");
}

void UnitTestSDSIAGetSample() {
  srandom(1);
  char* testFolder = PBFSJoinPath(".", "UnitTestOut", "001", "001");
  SDSIA sdsia = SDSIACreateStatic(testFolder);
  char* imgName[3] = {"img001.tga", "img002.tga", "img000.tga"};
  char* maskName[3] = {"mask001.tga", "mask002.tga", "mask000.tga"};
  int iSample = 0;
  do {
    SDSIASample sample = SDSIAGetSample(&sdsia, 0);
    char* pi = PBFSJoinPath(testFolder, imgName[iSample]);
    char* pm = PBFSJoinPath(testFolder, maskName[iSample]);
    if (sample._img == NULL ||
      strcmp(GBFileName(sample._img), pi) != 0 ||
      sample._mask == NULL ||
      strcmp(GBFileName(sample._mask), pm) != 0) {
      SDSIAErr->_type = PBErrTypeUnitTestFailed;
      sprintf(SDSIAErr->_msg, "SDSIAGetSample failed");
      PBErrCatch(SDSIAErr);
    }
    free(pi);
    free(pm);
    ++iSample;
    SDSIASampleFreeStatic(&sample);
  } while(SDSIAStepSample(&sdsia, 0));
  if (iSample != 3) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAStepSample failed");
    PBErrCatch(SDSIAErr);
  }
  SDSIAReset(&sdsia, 0);
  iSample = 0;
  do {
    SDSIASample sample = SDSIAGetSample(&sdsia, 0);
    char* pi = PBFSJoinPath(testFolder, imgName[iSample]);
    char* pm = PBFSJoinPath(testFolder, maskName[iSample]);
    if (sample._img == NULL ||
      strcmp(GBFileName(sample._img), pi) != 0 ||
      sample._mask == NULL ||
      strcmp(GBFileName(sample._mask), pm) != 0) {
      SDSIAErr->_type = PBErrTypeUnitTestFailed;
      sprintf(SDSIAErr->_msg, "SDSIAReset failed");
      PBErrCatch(SDSIAErr);
    }
    free(pi);
    free(pm);
    ++iSample;
    SDSIASampleFreeStatic(&sample);
  } while(SDSIAStepSample(&sdsia, 0));
  if (iSample != 3) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAReset failed");
    PBErrCatch(SDSIAErr);
  }
  VecLong2D split = VecLongCreateStatic2D();
  VecSet(&split, 0, 1);
  VecSet(&split, 1, 2);
  SDSIASplit(&sdsia, (VecLong*)&split);
  char* imgNameA[1] = {"img002.tga"};
  char* maskNameA[1] = {"mask002.tga"};
  iSample = 0;
  do {
    SDSIASample sample = SDSIAGetSample(&sdsia, 0);
    char* pi = PBFSJoinPath(testFolder, imgNameA[iSample]);
    char* pm = PBFSJoinPath(testFolder, maskNameA[iSample]);
    if (sample._img == NULL ||
      strcmp(GBFileName(sample._img), pi) != 0 ||
      sample._mask == NULL ||
      strcmp(GBFileName(sample._mask), pm) != 0) {
      SDSIAErr->_type = PBErrTypeUnitTestFailed;
      sprintf(SDSIAErr->_msg, "SDSIAGetSample failed");
      PBErrCatch(SDSIAErr);
    }
    free(pi);
    free(pm);
    ++iSample;
    SDSIASampleFreeStatic(&sample);
  } while(SDSIAStepSample(&sdsia, 0));
  if (iSample != 1) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAStepSample failed");
    PBErrCatch(SDSIAErr);
  }
  char* imgNameB[2] = {"img000.tga", "img001.tga"};
  char* maskNameB[2] = {"mask000.tga", "mask001.tga"};
  iSample = 0;
  do {
    SDSIASample sample = SDSIAGetSample(&sdsia, 1);
    char* pi = PBFSJoinPath(testFolder, imgNameB[iSample]);
    char* pm = PBFSJoinPath(testFolder, maskNameB[iSample]);
    if (sample._img == NULL ||
      strcmp(GBFileName(sample._img), pi) != 0 ||
      sample._mask == NULL ||
      strcmp(GBFileName(sample._mask), pm) != 0) {
      SDSIAErr->_type = PBErrTypeUnitTestFailed;
      sprintf(SDSIAErr->_msg, "SDSIAGetSample failed");
      PBErrCatch(SDSIAErr);
    }
    free(pi);
    free(pm);
    ++iSample;
    SDSIASampleFreeStatic(&sample);
  } while(SDSIAStepSample(&sdsia, 1));
  if (iSample != 2) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAStepSample failed");
    PBErrCatch(SDSIAErr);
  }
  SDSIAResetAll(&sdsia);
  iSample = 0;
  do {
    SDSIASample sample = SDSIAGetSample(&sdsia, 0);
    char* pi = PBFSJoinPath(testFolder, imgNameA[iSample]);
    char* pm = PBFSJoinPath(testFolder, maskNameA[iSample]);
    if (sample._img == NULL ||
      strcmp(GBFileName(sample._img), pi) != 0 ||
      sample._mask == NULL ||
      strcmp(GBFileName(sample._mask), pm) != 0) {
      SDSIAErr->_type = PBErrTypeUnitTestFailed;
      sprintf(SDSIAErr->_msg, "SDSIAGetSample failed");
      PBErrCatch(SDSIAErr);
    }
    free(pi);
    free(pm);
    ++iSample;
    SDSIASampleFreeStatic(&sample);
  } while(SDSIAStepSample(&sdsia, 0));
  if (iSample != 1) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAStepSample failed");
    PBErrCatch(SDSIAErr);
  }
  iSample = 0;
  do {
    SDSIASample sample = SDSIAGetSample(&sdsia, 1);
    char* pi = PBFSJoinPath(testFolder, imgNameB[iSample]);
    char* pm = PBFSJoinPath(testFolder, maskNameB[iSample]);
    if (sample._img == NULL ||
      strcmp(GBFileName(sample._img), pi) != 0 ||
      sample._mask == NULL ||
      strcmp(GBFileName(sample._mask), pm) != 0) {
      SDSIAErr->_type = PBErrTypeUnitTestFailed;
      sprintf(SDSIAErr->_msg, "SDSIAGetSample failed");
      PBErrCatch(SDSIAErr);
    }
    free(pi);
    free(pm);
    ++iSample;
    SDSIASampleFreeStatic(&sample);
  } while(SDSIAStepSample(&sdsia, 1));
  if (iSample != 2) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "SDSIAStepSample failed");
    PBErrCatch(SDSIAErr);
  }
  SDSIAFreeStatic(&sdsia);
  free(testFolder);
  printf("UnitTestSDSIAGetSample OK\n");
}

void UnitTestPython() {
  int ret = system("python generateDataSet.py -unitTest");
  if (ret != 0) {
    SDSIAErr->_type = PBErrTypeUnitTestFailed;
    sprintf(SDSIAErr->_msg, "UnitTestPython failed");
    PBErrCatch(SDSIAErr);
  }
  printf("UnitTestPython OK\n");
}

void UnitTestAll() {
  UnitTestPython();
  UnitTestSDSIACreateFree();
  UnitTestSDSIAGet();
  UnitTestSDSIASplitUnsplitShuffle();
  UnitTestSDSIAGetSample();
}

int main(void) {
  UnitTestAll();
  return 0;
}

