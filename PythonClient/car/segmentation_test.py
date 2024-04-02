#!/usr/bin/env python

import setup_path
import airsim
import csv
import random
import numpy as np
from PIL import Image
from datetime import datetime

# Demonstration script to show how the instance segmentation can be accessed and updated through the API
# See docs/instance_segmentation.md and docs/image_apis.md#segmentation for more information.
if __name__ == '__main__':

    # Make connection to AirSim API
    client = airsim.CarClient()
    client.confirmConnection()

    # Generate list of all colors available for segmentation
    print("Generating segmentation colormap, this takes a while...")
    colorMap = client.simGetSegmentationColorMap()
    print("Generated segmentation colormap\n")

    # Get names of all objects in simulation world in the instance segmentation format
    # and store in list together with the associated RGB value
    # In a dynamic world, this is never the same!!
    currentObjectList = client.simListInstanceSegmentationObjects()

    # Sort the objects from the list by class defined in the CSV and keep them in a dictionary with classname as key
    print("Sorting objects based on segmentation.csv into classes...")
    with open('segmentation.csv', encoding='utf-8-sig') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        index = 0
        classObjectMap = {}
        for idx, row in enumerate(csv_reader, start=1):
            classItems = [i for i in currentObjectList if i.startswith(row[0])]
            print("Found " + str(len(classItems)) + " objects starting with object-name '" + str(row[0]) + "'")
            classObjectMap[row[0]] = classItems
    print("Sorted objects based on segmentation.csv into classes\n")

    # Set the colors for all walls to a chosen color with color index 15
    className = 'Wall'
    classColorIndex = 2
    instanceID = 1

    # a) this version we set it based on the gathered objects in the list
    print("Setting all objects in world matching class-name '" + className + "' a certain color, based on object list...")
    for item in classObjectMap[className]:
        success = client.simSetSegmentationObjectID(item, classColorIndex, False, instanceID, True)
        if success:
            instanceID = instanceID + 1
            print("Found object matching object-name '" + item + "'! Set segmentation value " + str(classColorIndex))
        else:
            print("No object found matching object-name '" + item + "'")
    print("Found and changed color on all objects in world matching class-name '"  + className + "' based on object list\n")


responses = client.simGetImages([airsim.ImageRequest( "front_center", airsim.ImageType.Segmentation, False, False)])
img_rgb_string = responses[0].image_data_uint8
rgbarray = np.frombuffer(img_rgb_string, np.uint8)
rgbarray_shaped = rgbarray.reshape(responses[0].height, responses[0].width, 3)
rgbarray_shaped = rgbarray_shaped
img = Image.fromarray(rgbarray_shaped, 'RGB')
img.show()
img.save("segmentation_sample.png", "PNG")



