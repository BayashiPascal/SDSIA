# Import necessary modules
import os, sys, json

# Base directory
BASE_DIR = os.path.dirname(os.path.abspath(__file__))

# Function to print exceptions
def PrintExc(exc):
  exc_type, exc_obj, exc_tb = sys.exc_info()
  fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
  print(exc_type, fname, exc_tb.tb_lineno, str(exc))

def Main(dataSetFolderPath):
  '''
  Main function
  Inputs:
    'dataSetFolderPath': full path to the folder containing the data set
  '''
  try:
    
    # Check if the folder exists
    if not os.path.exists(dataSetFolderPath):
      print("The folder " + dataSetFolderPath + " doesn't exists")
      quit()
    
    # Check if the folder contains the data set description file
    descFilePath = os.path.join(dataSetFolderPath, "dataset.json")
    if not os.path.exists(descFilePath):
      print("The description file " + descFilePath + " doesn't exists")
      quit()
    
    # Load and decode the content of the description file
    with open(descFilePath, "r") as fp:
      dataSetDesc = json.load(fp)

    # Display info about the data set
    print("Description: " + dataSetDesc["desc"])
    print("Nb image: " + dataSetDesc["nbSample"])
    print("Nb mask: " + dataSetDesc["nbMask"])
    print("Dimension image (width, height): " + str(dataSetDesc["dim"]))
    print("Format image: " + dataSetDesc["format"])
    
    # Loop on the data set
    for iSample in range(int(dataSetDesc["nbSample"])):
      
      # Get the path to the image and mask
      imgFileName = dataSetDesc["samples"][iSample]["img"]
      maskFileNames = dataSetDesc["samples"][iSample]["mask"]
      imgFilePath = os.path.join(dataSetFolderPath, imgFileName)
     
      # Check the full paths are valid
      if not os.path.exists(imgFilePath):
        print("Description file corrupted. The image " + \
          imageFilePath + " doesn't exists")
        quit()
        
      # Loop on masks
      for maskFileName in maskFileNames:
        
        # Check for the existence of the mask
        maskFilePath = os.path.join(dataSetFolderPath, maskFileName)
        if not os.path.exists(maskFilePath):
          print("Description file corrupted. The mask " + \
            maskFilePath + " doesn't exists")
          quit()
        
        # Train on the pair image-mask
        # In the mask, the black pixels match the target and the white
        # pixels match the non-target
        print("Train on (" + imgFilePath + ", " + maskFilePath + ")")
    
  except Exception as exc:
    PrintExc(exc)

# Hook for the main function    
if __name__ == '__main__':
  try:
    
    # Get the data set folder path from the argument line
    if not len(sys.argv) == 2:
      print("Usage: python exampleUse.py <dataSetFolderPath>")
      quit()
    dataSetFolderPath = os.path.abspath(sys.argv[1])
    
    # Call the main function with the data set folder
    Main(dataSetFolderPath)

  except Exception as exc:
    PrintExc(exc)
