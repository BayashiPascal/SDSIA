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
