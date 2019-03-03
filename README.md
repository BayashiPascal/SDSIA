# SDSIA (Synthetic Data Set for Image Analysis)

SDSIA is a Python module to generate synthetic data sets for image analysis.

The goal of the Python module is to provide a way to easily generate data sets of images used by image analysis softwares or algorithms, in a context of learning or development.

The generation of the images is made using the 3D rendering software POV-Ray. This software uses text scripts to defined the scene to render, so it's very easy to parameterize a script to create variations of a given scene. The Python module then render as many samples as needed in the set, each time using different parameters' value.

By creating its own POV-Ray script, the user can create any needed data set, selecting precisely which aspect of the scene is constant and which is variable. The level of difficulty of the data set, from the point of view of the software or algorithm used for analysis, can then be controlled at will. By choosing the appropriate variable in the scene, the user could also study a particular property of a given software or algorithm (e.g, is it more sensitive to variation in shape or variation in color).

Once the user has written the script to render images, the python module takes care of the tedious tasks of rendering (only needed data sets), files and folders management, creation of a description file in JSON format for later use. It also generates perfect one or several masks (black and white image) by modifying the texture properties of the objects in the scene.

The number of images per data set, the dimensions and format of each image are also defined by the user. Then, one can create sets corresponding to its needs, in particular memory, disk storage, processing time limits.

If the output format is supported by cv2.imread, the relative coordinates (as expected by Yolo) of the bounding box for each mask are also generated.

The current version of SDSIA is designed for image segmentation (localization of pixels corresponding to an object in a scene). However it has been developped with the view to be extended to other kind of data sets.

## How to install this repository

The Python module doesn't require any particular operation, but you will need to have the following Python modules installed on your machine:
os, sys, json, glob, subprocess, re, shutil, cv2, numpy
You will also need to have POV-Ray installed on your machien:
http://www.povray.org/
If you need assistance feel free to contact me with my gmail address: at bayashipascal.

## How to use this repository

Detailed explanation are available in the documentation:
https://github.com/BayashiPascal/SDSIA/blob/master/Doc/doc.pdf
A simple example is given below as an illustration of the process.

1) Create the description file for the data set and save it as dataset-000-000.json
```
{
"dataSetType": "1",
"desc": "unitTest",
"dim": {
"_dim":"2",
"_val":["10", "20"]
},
"format": "tga",
"nbSample": "3",
"nbMask": "2"
}
```

2) Create the POV-Ray script and save it as dataset-000-000.pov in the same folder as dataset.json
```
#include "colors.inc"
#include "textures.inc"

// Black texture used to create the mask of the target
#declare _texMaskTarget = texture {
  pigment { color Black }
}

// White texture used to create the mask of the non-target
#declare _texMaskNonTarget = texture {
  pigment { color White }
}

// Random generator seed, based on the clock variable set by the 
// SDSIA generator
#declare RndSeed = seed(clock);

// Macro to get a random value in [A, B]
#macro rnd(A,B)
  (A+(B-A)*rand(RndSeed))
#end

// Camera
camera {
  location    <0.0,10.0,0.0>
  look_at     <0.0,0.0,0.0>
  right x
}

// Light source
light_source {
  <0.0,10.0,0.0>
}

// Background
background { color rgb <1.0, 1.0, 1.0> }

// Target A definition
#declare TargetA = box {
  -1, 1

  // Transformation of the target to create various samples
  translate x * rnd(0, 1)

  // Apply the real texture or the mask texture according to the 
  // Mask variable set by the SDSIA generator 
  #if (Mask = 0)
    pigment { color Red }
  #else 
    #if (Mask = 1)
      texture {_texMaskTarget}
    #else 
      #if (Mask = 2)
        texture {_texMaskNonTarget}
      #end
    #end
  #end

}

// Target B definition
#declare TargetB = cylinder { 
  -y, y, 0.5
  
  // Transformation of the non-target to create various samples
  translate x * rnd(0, 1)

  // Apply the real texture or the mask texture according to the 
  // Mask variable set by the SDSIA generator 
  #if (Mask = 0)
    pigment { color Blue }
  #else 
    #if (Mask = 1)
      texture {_texMaskNonTarget}
    #else 
      #if (Mask = 2)
        texture {_texMaskTarget}
      #end
    #end
  #end

}

// Create the scene
object { TargetA }
object { TargetB }
```

3) Run the script 
```
python generateDataSet.py -in <path to dataset.json> -out <output folder>
```

4) According to the script, images and their black/white masks will be generated in the folder <output folder>/000/000/. A json file is also generated with information about the generated dataset.
Examples of images and masks are given below:
![example](https://github.com/BayashiPascal/SDSIA/blob/master/Doc/sdsia.jpg)

5) An example of script to use the generated dataset would be:
```
# Import necessary modules
import json
import os
import sys

# Base directory
BASE_DIR = os.path.dirname(os.path.abspath(__file__))


# Function to print exceptions
def print_exc(exc):
    exc_type, exc_obj, exc_tb = sys.exc_info()
    fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
    print(exc_type, fname, exc_tb.tb_lineno, str(exc))


def main(data_set_folder_path):
    """
    Main function
    Inputs:
      'data_set_folder_path': full path to the folder containing the data set
    """
    try:

        # Check if the folder exists
        if not os.path.exists(data_set_folder_path):
            print("The folder {} doesn't exists".format(data_set_folder_path))
            quit()

        # Check if the folder contains the data set description file
        desc_file_path = os.path.join(data_set_folder_path, "dataset.json")
        if not os.path.exists(desc_file_path):
            print("The description file {} doesn't exists".format(desc_file_path))
            quit()

        # Load and decode the content of the description file
        with open(desc_file_path, "r") as fp:
            data_set_desc = json.load(fp)

        # Display info about the data set
        print("Description: {}".format(data_set_desc["desc"]))
        print("Nb image: {}".format(data_set_desc["nbSample"]))
        print("Nb mask: {}".format(data_set_desc["nbMask"]))
        print("Dimension image (width, height): {}".format(data_set_desc["dim"]))
        print("Format image: {}".format(data_set_desc["format"]))

        # Loop on the data set
        for iSample in range(int(data_set_desc["nbSample"])):

            # Get the path to the image and mask
            img_file_name = data_set_desc["samples"][iSample]["img"]
            mask_file_names = data_set_desc["samples"][iSample]["mask"]
            img_file_path = os.path.join(data_set_folder_path, img_file_name)

            # Check the full paths are valid
            if not os.path.exists(img_file_path):
                print("Description file corrupted. The image {} doesn't exists"
                      "".format(img_file_path))
                quit()

            # Loop on masks
            for maskFileName in mask_file_names:

                # Check for the existence of the mask
                mask_file_path = os.path.join(data_set_folder_path, maskFileName)
                if not os.path.exists(mask_file_path):
                    print("Description file corrupted. The mask {} doesn't exists"
                          "".format(mask_file_path))
                    quit()

                # Train on the pair image-mask
                # In the mask, the black pixels match the target and the white
                # pixels match the non-target
                print("Train on ({}, {})".format(img_file_path, mask_file_path))

    except Exception as exc:
        print_exc(exc)


# Hook for the main function
if __name__ == '__main__':
    try:

        # Get the data set folder path from the argument line
        if not len(sys.argv) == 2:
            print("Usage: python exampleUse.py <dataSetFolderPath>")
            quit()
        dataSetFolderPath = os.path.abspath(sys.argv[1])

        # Call the main function with the data set folder
        main(dataSetFolderPath)

    except Exception as exc:
        print_exc(exc)

```
