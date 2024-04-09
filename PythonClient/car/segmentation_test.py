# ====================================================================================================

# --- imports from airsim ---
import setup_path
import airsim

import csv
import random
import numpy as np
from PIL import Image

import os
import time
from datetime import datetime

import tempfile
import cv2

# --- imports from our Python->UE script ---
from pynput import keyboard
import socket
from threading import Thread
import threading

# --- import for Ascii art ---
# --- import for Ascii art --- 
from termcolor import colored
import pyfiglet


# ====================================================================================================

MIN_DEPTH_METERS = 0
MAX_DEPTH_METERS = 100

outputfolder = ""

# ====================================================================================================

# Function to send a message to the Unreal project
def send_to_unreal(message):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:  # SOCK_DGRAM for UDP
            s.sendto(message.encode(), ("127.0.0.1", 9999))
    except:
        print("Unable to send message to Unreal. Ensure Unreal is listening.")

# Function to receive a message from the Unreal project
def receive_from_unreal(event):
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as server_socket:
        server_socket.bind(("127.0.0.1", 9988))  # Binding to another port for receiving
        while True:
            message, address = server_socket.recvfrom(1024)
            decodedMessage = message.decode() # decode byte string
            if decodedMessage == "LOAD_SUCCESS":
                event.set()
            else:
                print(f"Received message from UE: {decodedMessage}")

# Seperate thread to detect and send input from Python -> UE
def on_key_press(key):
    try:
        # Check if the key is a character key
        if key.char == 'w':
            #print("Forward")
            send_to_unreal('w')
        elif key.char == 's':
            #print("Back")
            send_to_unreal('s')
    except AttributeError:
        # Non-character key was pressed
        pass

# Process and save images responses from AirSim
def saveImage(response, filename):
    if response.pixels_as_float:
        # airsim.write_pfm(os.path.normpath(filename + '.pfm'), airsim.get_pfm_array(response))
        depth = np.array(response.image_data_float, dtype=np.float64)
        depth = depth.reshape((response.height, response.width, -1))
        depth = np.array(depth * 255, dtype=np.uint8)
        # save pic
        cv2.imwrite(os.path.normpath(filename + '.png'), depth)

    elif response.compress: #png format
        airsim.write_file(os.path.normpath(filename + '.png'), response.image_data_uint8)

    else: #uncompressed array
        img1d = np.frombuffer(response.image_data_uint8, dtype=np.uint8) # get numpy array
        img_rgb = img1d.reshape(response.height, response.width, 3) # reshape array to 3 channel image array H X W X 3
        cv2.imwrite(os.path.normpath(filename + '.png'), img_rgb) # write to png

# Process and save depth image responses from AirSim
def processAndSaveDepthImage(response, filename):
    # Reshape to a 2d array with correct width and height
    depth_img_in_meters = airsim.list_to_2d_float_array(response.image_data_float, response.width, response.height)
    depth_img_in_meters = depth_img_in_meters.reshape(response.height, response.width, 1)

    # Lerp 0..100m to 0..255 gray values
    depth_8bit_lerped = np.interp(depth_img_in_meters, (MIN_DEPTH_METERS, MAX_DEPTH_METERS), (0, 255))
    cv2.imwrite(( filename + ".png"), depth_8bit_lerped.astype('uint8'))

# Update the instance segmentation in the scene
def UpdateInstanceSegmentation(AirsimClient):
    AirsimClient.simInitializeSegmentation()
    currentObjectList = AirsimClient.simListInstanceSegmentationObjects()

    # Sort the objects from the list by class defined in the CSV and keep them in a dictionary with classname as key
    #print("Sorting objects based on segmentation.csv into classes...")
    with open('segmentation.csv', encoding='utf-8-sig') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        index = 0
        classObjectMap = {}
        for idx, row in enumerate(csv_reader, start=1):
            classItems = [i for i in currentObjectList if i.startswith(row[0])]
            #print("Found " + str(len(classItems)) + " objects starting with object-name '" + str(row[0]) + "'")
            classObjectMap[row[0]] = classItems
    #print("Sorted objects based on segmentation.csv into classes\n")

    # Set the colors for all walls to a chosen color with color index 15
    InstanceMap =   { 
                    'BP_ParkedCar_veh': 2,
                    'SM_Frame_Interior_veh': 2
                    }
    instanceID = 1
    
    for instance in InstanceMap:
      className = instance
      classID   = InstanceMap[instance]
      for item in classObjectMap[className]:
        success = AirsimClient.simSetSegmentationObjectID(item, classID, False, instanceID, True)
        if success:
            instanceID = instanceID + 1
            #print("Found object matching object-name '" + item + "'! Set segmentation value " + str(classID))
        #else:
            #print("No object found matching object-name '" + item + "'")
    print("Instance segmentation updated\n")

# Get the output folder for the images
def GetOutputFolder(imageType = -1, timestamp = 0):
    global outputfolder

    # Create a folder with the timestamp
    if outputfolder == "":
        outputfolder = f"Output_{ timestamp }"
        
        #imageOutputFolder = f"Output_{ timestamp }"
        os.makedirs(outputfolder)

        # Generate scene, segmentation and depth folder in this folder
        os.makedirs(os.path.join(outputfolder, "Scene"))
        os.makedirs(os.path.join(outputfolder, "Segmentation"))
        os.makedirs(os.path.join(outputfolder, "Depth"))

    if imageType == 0:
        return os.path.join(outputfolder, "Scene")
    elif imageType == 2:
        return os.path.join(outputfolder, "Depth")
    elif imageType == 5:
        return os.path.join(outputfolder, "Segmentation")
    

    return outputfolder

# Generate images
def GenerateImages(AirsimClient, loopCount):
    imageIndex = 0

    # Segmentation image
    #segmentationResponses = client.simGetImages([
    #    airsim.ImageRequest("CameraMiddle", airsim.ImageType.Segmentation)
    #    ])
    #print('Retrieved segmentation images: %d' % len(segmentationResponses))

    #for response_idx, response in enumerate(segmentationResponses):
    #   imageIndex = imageIndex + 1
    #    filename = os.path.join(GetOutputFolder(5, response.time_stamp), f"CameraMiddle_5_{loopCount}")
    #    saveImage(response, filename)


    # Scene image
    #sceneResponses = client.simGetImages([
    #    airsim.ImageRequest("CameraMiddle", airsim.ImageType.Scene, False, False)
    #    ])
    #print('Retrieved images: %d' % len(sceneResponses))

    #for response_idx, response in enumerate(sceneResponses):
    #   imageIndex = imageIndex + 1
    #    filename = os.path.join(GetOutputFolder(0, response.time_stamp), f"CameraMiddle_0_{loopCount}")
    #    saveImage(response, filename)


    # Depth image
    depthResponses = client.simGetImages([
        airsim.ImageRequest("CameraMiddle", airsim.ImageType.DepthPerspective, True, False)
        ])
    print('Retrieved depth images: %d' % len(depthResponses))
    
    for response_idx, response in enumerate(depthResponses):
        imageIndex = imageIndex + 1
        filename = os.path.join(GetOutputFolder(2, response.time_stamp), f"CameraMiddle_2_{loopCount}")
        saveImage(response, filename)

# Setup all the listeners and threads
def Startup():
    # Set up the keyboard listener in a separate thread
    keyboard_thread = Thread(target=lambda: keyboard.Listener(on_press=on_key_press).start())
    keyboard_thread.start()

    # Set up the UDP server listener in a separate thread
    loadEvent = threading.Event()
    udp_receive_thread = Thread(target=receive_from_unreal, args=(loadEvent,))
    udp_receive_thread.start()

    # Print OTIV logo
    otivLogo = pyfiglet.figlet_format("   OTIV Tram Sim")
    coloredOtivLogo = colored(otivLogo, 'cyan')
    howest = "   Made in collaboration with Howest Digital Arts & Entertainment"
    print(coloredOtivLogo)
    print(colored(howest, 'cyan'))

    # Wait for Unreal Scene to load before creating AirSim connection and starting data collection
    print("\nWaiting for Unreal Scene to load..")
    loadEvent.wait()

    print(colored("LOAD_SUCCESS received, continuing execution...", 'green'))

# ====================================================================================================

if __name__ == '__main__':
    Startup()    

    # Setup connection to AirSim API
    client = airsim.CarClient()
    client.confirmConnection()
    client.enableApiControl(True)

    keepRunning = True
    loopCounter = 0

    imageInterval = 0.1

    while(keepRunning):
        # Pause the simulation
        client.simPause(True)
    
        # Update the instance segmentation
        #UpdateInstanceSegmentation(client)

        # Generate images
        GenerateImages(client, loopCounter)

        # Unpause the simulation
        client.simPause(False)

        #Wait for a set amount of time
        time.sleep(imageInterval)

        # Increase the loop counter
        loopCounter = loopCounter + 1