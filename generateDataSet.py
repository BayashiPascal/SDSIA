# Import necessary modules
import os, sys, json, glob, subprocess, re, shutil, cv2, numpy

# Base directory
BASE_DIR = os.path.dirname(os.path.abspath(__file__))

# Function to print exceptions
def PrintExc(exc):
  exc_type, exc_obj, exc_tb = sys.exc_info()
  fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
  print(exc_type, fname, exc_tb.tb_lineno, str(exc))

class DataSet:
  '''
  Class containing the information about a dataSet
  '''
  # Name of the data set
  _name = ""
  # Description of the data set
  _desc = ""
  # Type of the data set (place holder for future extension)
  _type = ""
  # Number of images in the data set
  _nbSample = "0"
  # Dimensions of the images in the data set [width, height]
  _dim = ["1", "1"]
  # format of the images in the data set
  _format = "tga"
  # List of images and their masks
  _images = []
  

  def __init__(self, templateFilePath):
    '''
    Constructor
    Inputs:
      'templateFilePath': full path to the template of the description
        file for this data set
    '''
    try:
      
      # Load and decode the template file
      with open(templateFilePath, "r") as fp:
        dataSetDesc = json.load(fp)
      
      # Get the name of the data set
      dataSetName = os.path.basename(templateFilePath)[0:-5]
      
      # Set properties according to the template
      self._name = dataSetName
      self._desc = dataSetDesc["desc"]
      self._type = dataSetDesc["dataSetType"]
      self._nbSample = dataSetDesc["nbSample"]
      self._dim = dataSetDesc["dim"]
      self._format = dataSetDesc["format"]
      self._nbMask = dataSetDesc["nbMask"]
      
    except Exception as exc:
      PrintExc(exc)

  def GetDescFileContent(self):
    '''
    Create the content of the file describing a DataSet
    Input:
      'self': the dataSet for which we want to create the description 
        file
    Output:
      Return the content of the file describing the DataSet as a string
      in JSON format
    '''
    try:

      # Init the return value
      ret = "{}"

      # Init a variable to memorize the content as an object
      content = {}

      # Add the info about the DataSet to the content
      content["dataSet"] = self._name
      content["desc"] = self._desc
      content["dataSetType"] = self._type
      content["nbSample"] = self._nbSample
      content["dim"] = self._dim
      content["format"] = self._format
      content["samples"] = self._images
      content["nbMask"] = self._nbMask

      # Encode the content to JSON format and return it
      ret = json.dumps(content, \
        sort_keys = True, indent = 2, separators = (',', ': '))

    except Exception as exc:
      PrintExc(exc)
    finally:
      return ret

  def Render(self, inFolder, outFolder):
    '''
    Render (create images and masks) the DataSet
    Inputs:
      'inFolder': the full path to the folder where the pov file is
      'outFolder': the full path of the folder where images and masks
        are output
    '''
    try:
      
      # Flush the result list
      self._images = []
      
      # Loop on the number of images to be rendered
      for iRender in range(int(self._nbSample)):
        
        # Create the file name of the image
        iRenderPadded = str(iRender).zfill(3)
        imgFileName = "img" + iRenderPadded + "." + self._format

        # Create the full path of the image
        imgFilePath = os.path.join(outFolder, imgFileName)
        
        # Create the full path to the pov file
        povFilePath = os.path.join(inFolder, self._name + ".pov")
        
        # Create the full path to the ini file used to render
        iniFilePath = os.path.join(outFolder, "pov.ini")
        
        # Create the command to render the image
        cmd = []
        cmd.append("povray")
        cmd.append("+O" + imgFileName)
        cmd.append("-W" + self._dim["_val"][0])
        cmd.append("-H" + self._dim["_val"][1])
        cmd.append("-D")
        cmd.append("-P")
        cmd.append("-Q9")
        cmd.append("+A")
        cmd.append("+k" + str(iRender))
        if self._format == "png":
          povFormat = "+FN"
        elif self._format == "tga":
          povFormat = "+FC"
        else:
          print("Unsupported format: " + self._format)
          return False
        cmd.append(povFormat)
        cmd.append("+I" + povFilePath)
        cmd.append(iniFilePath)

        # Create the ini file used to render
        with open(iniFilePath, "w") as fp:
          fp.write("Declare=Mask=0")

        # Render the image silently
        imgPath =  os.path.join(outFolder, \
          "img" + iRenderPadded + "." + self._format)
        print(iRenderPadded + "/" + self._nbSample.zfill(3) + \
          " Rendering image " + imgPath + " ...")
        FNULL = open(os.devnull, 'w')
        subprocess.call(cmd, stderr = FNULL, cwd = outFolder)
        
        # Check if the image has been created
        if not os.path.exists(imgPath):
          return False

        # Loop on masks
        maskFileNames = []
        maskBoundingBoxes = []
        for iMask in range(int(self._nbMask)):
          iMaskPadded = str(iMask).zfill(3)
          maskFileName = "mask" + iRenderPadded + "-" + iMaskPadded + \
            "." + self._format
          maskFilePath = os.path.join(outFolder, maskFileName)

          # Update the command and the ini file to render the mask
          cmd[1] = "+O" + maskFileName
          cmd[6] = "-Q0"

          with open(iniFilePath, "w") as fp:
            fp.write("Declare=Mask=" + str(iMask + 1))
          
          # Render the mask silently
          print("        Rendering mask " + maskFilePath + " ...");
          subprocess.call(cmd, stderr = FNULL, cwd = outFolder)

          # Check if the mask has been created
          if not os.path.exists(maskFilePath):
            return False
          
          # Load the mask
          imgMask = cv2.imread(maskFilePath, cv2.IMREAD_GRAYSCALE)

          # If we could load the mask
          if not imgMask is None:

            # Get the bounding coordinates of the mask
            yMax, xMax = numpy.max(numpy.where(imgMask != 255), 1)
            yMin, xMin = numpy.min(numpy.where(imgMask != 255), 1)
            
            # Convert the bounding coordinates into yolo format
            xRelCenter = \
              0.5 * (xMax + xMin) / float(self._dim["_val"][0])
            yRelCenter = \
              0.5 * (yMax + yMin) / float(self._dim["_val"][1])
            widthRel = (xMax - xMin + 1) / float(self._dim["_val"][0])
            heightRel = (yMax - yMin + 1) / float(self._dim["_val"][1])
            maskBoundingBox = \
              [str(xRelCenter), str(yRelCenter), \
              str(widthRel), str(heightRel)]

            # Add the bounding box
            maskBoundingBoxes.append(maskBoundingBox)
            
          # Add the mask's name to the array of names
          maskFileNames.append(maskFileName)
          
        # Remove the ini file
        os.remove(iniFilePath)
        
        # Add the image and mask to the list
        self._images.append( \
          {"img":imgFileName, "mask":maskFileNames, \
          "bounding":maskBoundingBoxes})
      
      # Return the success flag
      return True

    except Exception as exc:
      PrintExc(exc)
      return False

class DataSetGenerator:
  '''
  Class to generate the data sets
  '''
  # Folder containing the POV files
  # Only files matching dataSet-[0-9][0-9][0-9]-[0-9][0-9][0-9].pov are
  # processed
  _povFolder = os.path.join(BASE_DIR, "POV")
  # Folder containing the data sets
  _dataSetFolder = os.path.join(BASE_DIR, "DataSets")
  # Flag to memorize if the data sets are generated even if their
  # description file is younger than their pov file
  _force = False
  # Name of the description file
  _descFileName = "dataset.json"
  # Variables to memorize the successful generation
  _successDataSets = []
  # Variables to memorize the failed generation
  _failedDataSets = []
  # Variables to memorize the skipped generation
  _skipDataSets = []
  # Variable to memorize if we are in simulation mode
  _simul = False
  # Variable to memorize if we are in listing mode
  _list = False

  def __init__(self, args):
    '''
    Constructor
    Inputs:
      'args': the arguments passed to this script
    '''
    try:

      # Init a flag to memorize if the user requested unit tests
      flagUnitTest = False

      # Process arguments
      for iArg in range (len(args)):
        
        # Help
        if args[iArg] == "-help":
          print("server.py" + \
            " [-in <povFolder|povFile>] [-out <dataSetFolder>]" + \
            " [-force] [-simul] [-list] [-unitTest] [-help]")
          print("-in: folder containing the pov files, or one pov file")
          print("-out: folder where the data sets will be generated")
          print("-force: don't check time stamp and always generate" + \
            " all the data sets")
          print("-simul: don't actually generate the data sets")
          print("-list: display a list of the data sets")
          print("-unitTest: run the unit tests")
          quit()
        
        # POV files folder
        if args[iArg] == "-in":
          folder = args[iArg + 1]
          if os.path.exists(folder):
            self._povFolder = os.path.abspath(folder)
          else:
            print("The folder/file " + str(folder) + " doesn't exists.")
            quit()
        
        # DataSets folder
        if args[iArg] == "-out":
          folder = args[iArg + 1]
          if os.path.exists(folder):
            self._dataSetFolder = os.path.abspath(folder)
          else:
            print("The folder " + str(folder) + " doesn't exists.")
            quit()
        
        # Simulation mode, the sets are not actually rendered
        if args[iArg] == "-simul":
          self._simul = True
        
        # Force generation of all data sets even if their
        # description file is younger than their pov file
        if args[iArg] == "-force":
          self._force = True
        
        # Listing mode
        if args[iArg] == "-list":
          self._list = True

        # Unit tests
        if args[iArg] == "-unitTest":
          flagUnitTest = True

      # Run the unit tests if requested
      if flagUnitTest:
        ret = self.RunUnitTest()
        quit(ret)

    except Exception as exc:
      PrintExc(exc)
  
  def Run(self):
    '''
    Generate the dataSets according to the properties of the 
    DataSetGenerator 'self'
    '''
    try:

      # Reset the successful/failed/skipped dataSets lists
      self._successDataSets = []
      self._failedDataSets = []
      self._skippedDataSets = []

      # Get the list of POV files path
      # If the -in argument was a pov file, consider only this file,
      # else consider the pov files in the folder
      if self._povFolder[-4:] == ".pov":
        pattern = re.compile(
          "dataset-[0-9][0-9][0-9]-[0-9][0-9][0-9].pov")
        if pattern.match(os.path.basename(self._povFolder)):
          povFilePaths = [self._povFolder]
        else:
          print("\nThe pov file \n  " + self._povFolder + \
            "\ndoesn't match " + \
            "dataset-[0-9][0-9][0-9]-[0-9][0-9][0-9].pov\n")
          return None
      else:
        povFilePaths = glob.glob(os.path.join(self._povFolder, \
          "dataset-[0-9][0-9][0-9]-[0-9][0-9][0-9].pov"))

      # Sort the list of POV files path
      povFilePaths.sort()
      
      # If there are no Pov files in the input folder, inform the user
      # and stop
      if len(povFilePaths) == 0:
        print("\nNo Pov files in\n  " + self._povFolder + \
          "\nmatching dataset-[0-9][0-9][0-9]-[0-9][0-9][0-9].pov\n")
        return None
      
      # Loop on the POV file pathes
      for povFilePath in povFilePaths:
        
        # Get the filename of the pov file
        povFileName = os.path.basename(povFilePath)
        
        # Get the path to the input folder
        inFolder = os.path.dirname(povFilePath)
        
        # Get the path to the template of the description file
        templateFilePath = povFilePath[0:-4] + ".json"
        
        # Get the dataSet group number from the file name
        groupNum = povFileName[8:11]
      
        # Get the dataSet subgroup number from the file name
        subGroupNum = povFileName[12:15]

        # Get the name of the corresponding output folder
        outFolder = os.path.join(self._dataSetFolder, \
          groupNum, subGroupNum)
        
        # Get the path to the description file for this data set
        descFilePath = os.path.join(outFolder, self._descFileName)
        
        # Init a flag to memorize if this data set must be generated
        isGenNecessary = False
        
        # If the output folder doesn't exist, the generation is necessary
        if not os.path.exists(outFolder):
          isGenNecessary = True
        
        # Else, the output folder exists
        else:
          
          # If the description file exists
          if os.path.exists(descFilePath):

            # Get the last modification time of the description file
            dateLastModifDesc = os.path.getmtime(descFilePath)
            
            # Get the last modification time of the pov file
            dateLastModifPov = os.path.getmtime(povFilePath)
            
            # Get the last modification time of the template for
            # the description file
            dateLastModifTemplate = os.path.getmtime(templateFilePath)
            
            # If the Pov or template file as been modified more 
            # recently than the description file, the generation is 
            # necessary
            if dateLastModifPov > dateLastModifDesc or \
              dateLastModifTemplate > dateLastModifDesc:
              isGenNecessary = True

          # Else, the description file doesn't exist, the generation is 
          # necessary
          else:
            isGenNecessary = True
        
        # If we are in listing mode
        if self._list:
          
          # Print a mark to show if the set has been generated
          if isGenNecessary:
            print("[ ] "),
          else:
            print("[*] "),

          # Load the data set info
          dataSet = DataSet(templateFilePath)
          
          # Print the set name and descrition
          print(dataSet._name + ": " + dataSet._desc)

          
        # Else we are not in listing mode
        else:
          
          # If the generation is necessary or forced for this data set
          if isGenNecessary or self._force:
            
            # If we are not in simulation or listing mode
            if not self._simul and not self._list:
              
              # Ensure the output folder exists
              if not os.path.exists(outFolder):
                os.makedirs(outFolder)
              
              # Ensure the output folder is empty of the description file 
              # and images
              try:
                os.remove(os.path.join(outFolder, self._descFileName))
              except:
                pass
              for f in glob.glob(os.path.join(outFolder, "img*.*")):
                try:
                  os.remove(f)
                except:
                  pass
              for f in glob.glob(os.path.join(outFolder, "mask*.*")):
                try:
                  os.remove(f)
                except:
                  pass
            
            # If we are not in listing mode
            if not self._list:

              # Generate this data set
              self.Generate(povFilePath, povFileName, groupNum, \
                subGroupNum, outFolder, descFilePath, templateFilePath, \
                inFolder)
          
          # Else, the generation of this data set is skipped
          else:
            
            # If we are not in listing mode
            if not self._list:

              # Append this data set to the list of skipped data sets
              self._skipDataSets.append(povFilePath)

      # If we are not in listing mode
      if not self._list:

        # If some generation were successful
        if len(self._successDataSets) > 0:
          
          # Inform the user
          print("\nThe following data sets were " + \
            "generated successfully:")
          for d in self._successDataSets:
            print("  " + d)
            
        # If some generation failed
        if len(self._failedDataSets) > 0:
          
          # Inform the user
          print("\nThe following data sets couldn't be " + \
            "generated successfully:")
          for d in self._failedDataSets:
            print("  " + d)
            
        # If some generation were skipped
        if len(self._skipDataSets) > 0:
          
          # Inform the user
          print("\nThe following data sets were skipped:")
          for d in self._skipDataSets:
            print("  " + d)

        # Skip a line
        print("")
    
    except Exception as exc:
      PrintExc(exc)
  
  def Generate(self, povFilePath, povFileName, groupNum, \
    subGroupNum, outFolder, descFilePath, templateFilePath, inFolder):
    '''
    Generate one dataSet
    Inputs:
      'povFilePath': the full path of the pov file
      'povFileName': the filename of the pov file
      'groupNum': the dataSet group number from the file name
      'subGroupNum': the dataSet subgroup number from the file name
      'outFolder': the output folder where the data set is generated
      'descFilePath': the full path to the output description file
      'templateFilePath': the full path to the template of the 
        description file
      'inFolder': the input folder where the pov and template files are
    '''
    try:
      
      # Inform the user
      print("\n === Generate data set for\n  " + povFilePath + \
        "\nto\n  " + outFolder)

      # Skip a line
      print("")
      
      # If the template doesn't exist, add this dataSet to the failed 
      # data sets, inform the user and give up
      if not os.path.exists(templateFilePath):
        print("The template file\n  " + templateFilePath + \
          "\ndoesn't exist. Give up.")
        self._failedDataSets.append(povFilePath)
        return None
      
      # Load the template file into a DataSet object
      dataSet = DataSet(templateFilePath)
      
      # If we are not in simulation mode
      if not self._simul:

        # Generate the images and masks
        if not dataSet.Render(inFolder, outFolder):
          
          # If the rendering of the data set has failed, inform the user 
          # and give up
          print("The rendering of \n  " + povFilePath + \
            "\nhas failed. Give up.")
          self._failedDataSets.append(povFilePath)
          return None
          
        # Create the description file
        with open(descFilePath, "w") as fp:
          fp.write(dataSet.GetDescFileContent())
      
      # Append this data set to the list of successfull data sets
      self._successDataSets.append(povFilePath)

      # Inform the user
      print("\nGeneration of \n  " + outFolder + \
        "\ncompleted.")

    except Exception as exc:
      PrintExc(exc)
  
  def RunUnitTest(self):
    '''
    Run the unit tests
    '''
    try:
      
      # Variable to memorize if the unit tests succeeded
      flagSuccess = True
      
      # Create temporary folders to run the test
      try:
        shutil.rmtree("UnitTestIn")
        shutil.rmtree("UnitTestOut")
      except:
        pass
      os.mkdir("UnitTestIn")
      os.mkdir("UnitTestOut")

      # Create fake data set
      shutil.copy("dataset.pov", 
        os.path.join("UnitTestIn", "dataset-001-001.pov"))
      shutil.copy("dataset.pov", 
        os.path.join("UnitTestIn", "dataset-001-002.pov"))
      shutil.copy("dataset.pov", 
        os.path.join("UnitTestIn", "dataset-002-001.pov"))
      shutil.copy("dataset.json", 
        os.path.join("UnitTestIn", "dataset-001-001.json"))
      shutil.copy("dataset.json", 
        os.path.join("UnitTestIn", "dataset-001-002.json"))
      shutil.copy("dataset2.json", 
        os.path.join("UnitTestIn", "dataset-002-001.json"))

      # Test listing
      cmd = []
      cmd.append("python")
      cmd.append("generateDataSet.py")
      cmd.append("-in")
      cmd.append("UnitTestIn")
      cmd.append("-out")
      cmd.append("UnitTestOut")
      cmd.append("-list")
      with open("out.txt", "w") as fp:
        subprocess.call(cmd, stdout = fp)
      check = ["[ ]  dataset-001-001: unitTest\n",
        "[ ]  dataset-001-002: unitTest\n",
        "[ ]  dataset-002-001: unitTest\n"]
      with open ("out.txt", "r") as fp:
        data = fp.readlines()
        if not data == check:
          print("[-list] NOK")
          flagSuccess = False
        else:
          print("[-list] OK")
      
      # Test simulation
      cmd = []
      cmd.append("python")
      cmd.append("generateDataSet.py")
      cmd.append("-in")
      cmd.append("UnitTestIn")
      cmd.append("-out")
      cmd.append("UnitTestOut")
      cmd.append("-simul")
      with open("out.txt", "w") as fp:
        subprocess.call(cmd, stdout = fp)
      check = [
        '\n', 
        ' === Generate data set for\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-001-001.pov") + 
        '\n', 
        'to\n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestOut", "001", "001") + 
        '\n', 
        '\n', 
        '\n', 
        'Generation of \n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestOut", "001", "001") + 
        '\n', 
        'completed.\n', 
        '\n', 
        ' === Generate data set for\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-001-002.pov") + 
        '\n', 
        'to\n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestOut", "001", "002") + 
        '\n', 
        '\n', 
        '\n', 
        'Generation of \n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestOut", "001", "002") + 
        '\n', 
        'completed.\n', 
        '\n', 
        ' === Generate data set for\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-002-001.pov") + 
        '\n', 
        'to\n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestOut", "002", "001") + 
        '\n', 
        '\n', 
        '\n', 
        'Generation of \n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestOut", "002", "001") + 
        '\n', 
        'completed.\n', 
        '\n', 
        'The following data sets were generated successfully:\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-001-001.pov") + 
        '\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-001-002.pov") + 
        '\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-002-001.pov") + 
        '\n', 
        '\n']
      with open ("out.txt", "r") as fp:
        data = fp.readlines()
        if not data == check or \
          os.path.exists(os.path.join("UnitTestOut", "001", "001",
          "dataset.json")) or \
          os.path.exists(os.path.join("UnitTestOut", "001", "002",
          "dataset.json")) or \
          os.path.exists(os.path.join("UnitTestOut", "002", "001",
          "dataset.json")):
          print("[-simul] NOK")
          flagSuccess = False
        else:
          print("[-simul] OK")

      # Test generation
      cmd = cmd[:-1]
      with open("out.txt", "w") as fp:
        subprocess.call(cmd, stdout = fp)
      check = [
        '\n', 
        ' === Generate data set for\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-001-001.pov") + 
        '\n', 
        'to\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "001") + '\n', 
        '\n', 
        '000/003 Rendering image ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "001", "") + 
        'img000.png ...\n', 
        '        Rendering mask ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "001", "") + 
        'mask000-000.png ...\n', 
        '001/003 Rendering image ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "001", "") + 
        'img001.png ...\n', 
        '        Rendering mask ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "001", "") + 
        'mask001-000.png ...\n', 
        '002/003 Rendering image ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "001", "") + 
        'img002.png ...\n', 
        '        Rendering mask ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "001", "") + 
        'mask002-000.png ...\n', 
        '\n', 
        'Generation of \n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestOut", "001", "001") + 
        '\n', 'completed.\n', 
        '\n', 
        ' === Generate data set for\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-001-002.pov") + 
        '\n', 
        'to\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "002") + '\n', 
        '\n', 
        '000/003 Rendering image ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "002", "") + 
        'img000.png ...\n', 
        '        Rendering mask ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "002", "") + 
        'mask000-000.png ...\n', 
        '001/003 Rendering image ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "002", "") + 
        'img001.png ...\n', 
        '        Rendering mask ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "002", "") + 
        'mask001-000.png ...\n', 
        '002/003 Rendering image ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "002", "") + 
        'img002.png ...\n', 
        '        Rendering mask ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "002", "") + 
        'mask002-000.png ...\n', 
        '\n', 
        'Generation of \n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "001", "002") + '\n', 
        'completed.\n', 
        '\n', 
        ' === Generate data set for\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-002-001.pov") + 
        '\n', 
        'to\n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestOut", "002", "001") + 
        '\n', 
        '\n', 
        '000/003 Rendering image ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "002", "001", "") + 
        'img000.tga ...\n', 
        '        Rendering mask ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "002", "001", "") + 
        'mask000-000.tga ...\n', 
        '        Rendering mask ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "002", "001", "") + 
        'mask000-001.tga ...\n', 
        '001/003 Rendering image ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "002", "001", "") + 
        'img001.tga ...\n', 
        '        Rendering mask ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "002", "001", "") + 
        'mask001-000.tga ...\n', 
        '        Rendering mask ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "002", "001", "") + 
        'mask001-001.tga ...\n', 
        '002/003 Rendering image ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "002", "001", "") + 
        'img002.tga ...\n', 
        '        Rendering mask ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "002", "001", "") + 
        'mask002-000.tga ...\n', 
        '        Rendering mask ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "002", "001", "") + 
        'mask002-001.tga ...\n', 
        '\n', 
        'Generation of \n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestOut", "002", "001") + '\n', 
        'completed.\n', 
        '\n', 
        'The following data sets were generated successfully:\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-001-001.pov") + 
        '\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-001-002.pov") + 
        '\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-002-001.pov") + 
        '\n', 
        '\n']
      with open ("out.txt", "r") as fp:
        data = fp.readlines()
        if not data == check or \
          not os.path.exists(os.path.join("UnitTestOut", "001", "001",
          "dataset.json")) or \
          not os.path.exists(os.path.join("UnitTestOut", "001", "001",
          "img000.png")) or \
          not os.path.exists(os.path.join("UnitTestOut", "001", "001",
          "img001.png")) or \
          not os.path.exists(os.path.join("UnitTestOut", "001", "001",
          "img002.png")) or \
          not os.path.exists(os.path.join("UnitTestOut", "001", "001",
          "mask000-000.png")) or \
          not os.path.exists(os.path.join("UnitTestOut", "001", "001",
          "mask001-000.png")) or \
          not os.path.exists(os.path.join("UnitTestOut", "001", "001",
          "mask002-000.png")) or \
          not os.path.exists(os.path.join("UnitTestOut", "001", "002",
          "dataset.json")) or \
          not os.path.exists(os.path.join("UnitTestOut", "001", "002",
          "img000.png")) or \
          not os.path.exists(os.path.join("UnitTestOut", "001", "002",
          "img001.png")) or \
          not os.path.exists(os.path.join("UnitTestOut", "001", "002",
          "img002.png")) or \
          not os.path.exists(os.path.join("UnitTestOut", "001", "002",
          "mask000-000.png")) or \
          not os.path.exists(os.path.join("UnitTestOut", "001", "002",
          "mask001-000.png")) or \
          not os.path.exists(os.path.join("UnitTestOut", "001", "002",
          "mask002-000.png")) or \
          not os.path.exists(os.path.join("UnitTestOut", "002", "001",
          "dataset.json")) or \
          not os.path.exists(os.path.join("UnitTestOut", "002", "001",
          "img000.tga")) or \
          not os.path.exists(os.path.join("UnitTestOut", "002", "001",
          "img001.tga")) or \
          not os.path.exists(os.path.join("UnitTestOut", "002", "001",
          "img002.tga")) or \
          not os.path.exists(os.path.join("UnitTestOut", "002", "001",
          "mask000-000.tga")) or \
          not os.path.exists(os.path.join("UnitTestOut", "002", "001",
          "mask001-000.tga")) or \
          not os.path.exists(os.path.join("UnitTestOut", "002", "001",
          "mask002-000.tga")) or \
          not os.path.exists(os.path.join("UnitTestOut", "002", "001",
          "mask002-001.tga")):
          flagSuccess = False
      check = [
        '\n', 
        'The following data sets were skipped:\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-001-001.pov") + 
        '\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-001-002.pov") + 
        '\n', 
        '  ' + 
        os.path.join(BASE_DIR, "UnitTestIn", "dataset-002-001.pov") + 
        '\n', 
        '\n']
      if flagSuccess:
        with open("out.txt", "w") as fp:
          subprocess.call(cmd, stdout = fp)
        with open ("out.txt", "r") as fp:
          data = fp.readlines()
          if not data == check:
            flagSuccess = False
            print("Generation NOK")
          else:
            print("Generation OK")
      else:
        print("Generation NOK")

      # Test [-force]
      dateTest = os.path.getmtime(os.path.join("UnitTestOut", 
        "001", "001", "dataset.json"))
      cmd.append("-force")
      with open("out.txt", "w") as fp:
        subprocess.call(cmd, stdout = fp)
      check = [
        '\n', 
        ' === Generate data set for\n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestIn", \
          "dataset-001-001.pov") + '\n', 
        'to\n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestOut", "001", "001") + \
          '\n', 
        '\n', 
        '000/003 Rendering image ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "001", "001", "") + 'img000.png ...\n', 
        '        Rendering mask ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "001", "001", "") + 'mask000-000.png ...\n', 
        '001/003 Rendering image ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "001", "001", "") + 'img001.png ...\n', 
        '        Rendering mask ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "001", "001", "") + 'mask001-000.png ...\n', 
        '002/003 Rendering image ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "001", "001", "") + 'img002.png ...\n', 
        '        Rendering mask ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "001", "001", "") + 'mask002-000.png ...\n', 
        '\n', 
        'Generation of \n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestOut", "001", "001") + \
          '\n', 
        'completed.\n', 
        '\n', 
        ' === Generate data set for\n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestIn", \
          "dataset-001-002.pov") + '\n', 
        'to\n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestOut", "001", "002") + \
          '\n', 
        '\n', 
        '000/003 Rendering image ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "001", "002", "") + 'img000.png ...\n', 
        '        Rendering mask ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "001", "002", "") + 'mask000-000.png ...\n', 
        '001/003 Rendering image ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "001", "002", "") + 'img001.png ...\n', 
        '        Rendering mask ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "001", "002", "") + 'mask001-000.png ...\n', 
        '002/003 Rendering image ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "001", "002", "") + 'img002.png ...\n', 
        '        Rendering mask ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "001", "002", "") + 'mask002-000.png ...\n', 
        '\n', 
        'Generation of \n',
        '  ' + os.path.join(BASE_DIR, "UnitTestOut", "001", "002") + \
          '\n', 
        'completed.\n', 
        '\n', 
        ' === Generate data set for\n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestIn", \
          "dataset-002-001.pov") + '\n', 
        'to\n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestOut", "002", "001") + \
          '\n', 
        '\n', 
        '000/003 Rendering image ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "002", "001", "") + 'img000.tga ...\n', 
        '        Rendering mask ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "002", "001", "") + 'mask000-000.tga ...\n', 
        '        Rendering mask ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "002", "001", "") + 'mask000-001.tga ...\n', 
        '001/003 Rendering image ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "002", "001", "") + 'img001.tga ...\n', 
        '        Rendering mask ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "002", "001", "") + 'mask001-000.tga ...\n', 
        '        Rendering mask ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "002", "001", "") + 'mask001-001.tga ...\n', 
        '002/003 Rendering image ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "002", "001", "") + 'img002.tga ...\n', 
        '        Rendering mask ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "002", "001", "") + 'mask002-000.tga ...\n', 
        '        Rendering mask ' + os.path.join(BASE_DIR, \
          "UnitTestOut", "002", "001", "") + 'mask002-001.tga ...\n', 
        '\n', 
        'Generation of \n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestOut", "002", "001") + \
          '\n', 
        'completed.\n', 
        '\n', 
        'The following data sets were generated successfully:\n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestIn", \
          "dataset-001-001.pov") + '\n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestIn", \
          "dataset-001-002.pov") + '\n', 
        '  ' + os.path.join(BASE_DIR, "UnitTestIn", \
          "dataset-002-001.pov") + '\n', 
        '\n']
      with open ("out.txt", "r") as fp:
        data = fp.readlines()
        if not data == check or \
          dateTest == os.path.getmtime(os.path.join("UnitTestOut", 
        "001", "001", "dataset.json")):
          flagSuccess = False
          print("[-force] NOK")
        else:
          print("[-force] OK")
      
      # Delete the temporary file
      os.remove("out.txt")

      # Inform the user
      if flagSuccess:
        print("UnitTest of generateDataSet.py succeeded")
        ret = 0
      else:
        print("UnitTest of generateDataSet.py failed")
        ret = 1

      return ret
    except Exception as exc:
      PrintExc(exc)
  
def Main():
  '''
  Main function
  '''
  try:
    
    # Create a DataSetGenerator
    generator = DataSetGenerator(sys.argv)
    
    # Generate the dataSets
    generator.Run()
    
  except Exception as exc:
    PrintExc(exc)

# Hook for the main function    
if __name__ == '__main__':
  Main()
