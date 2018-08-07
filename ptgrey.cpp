////////////////////////////////////////////////////////////////////////////////
//                      *** 150 FPS Acquisition Script ***                    //
//                            Andreas Genewsky (2018)                         //
//                           agenewsky(snail)gmail.com                        //
//                   RG Sirota, LMU Munich, Bavaria, Germany                  //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// MIT License                                                                //
//                                                                            //   
// Copyright (c) [2018] [Andreas Genewsky]                                    //  
//                                                                            //
// Permission is hereby granted, free of charge, to any person obtaining a    //
// copy of this software and associated documentation files (the "Software"), //
// to deal in the Software without restriction, including without limitation  //
// the rights to use, copy, modify, merge, publish, distribute, sublicense,   //
// and/or sell copies of the Software, and to permit persons to whom the      //
// Software is furnished to do so, subject to the following conditions:       //
//                                                                            //
// The above copyright notice and this permission notice shall be included    //
// in all copies or substantial portions of the Software.                     //
//                                                                            //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    //
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    //
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        //   
// DEALINGS IN THE SOFTWARE.                                                  //
////////////////////////////////////////////////////////////////////////////////

//============================================================================//
// Copyright 2015 Point Grey Research, Inc. All Rights Reserved.			  //
//																			  //	
// This software is the confidential and proprietary information of 		  //
// Point Grey Research, Inc. ("Confidential Information"). You shall not	  //
// disclose such Confidential Information and shall use it only in 			  //
// accordance with the terms of the "License Agreement" that you 			  //
// entered into with PGR in connection with this software.					  //
//																			  //
// UNLESS OTHERWISE SET OUT IN THE LICENSE AGREEMENT, THIS SOFTWARE IS 		  //
// PROVIDED ON AN "AS-IS" BASIS AND POINT GREY RESEARCH INC. MAKES NO 		  //
// REPRESENTATIONS OR WARRANTIES ABOUT THE SOFTWARE, EITHER EXPRESS 		  //
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED WARRANTIES OR 		  //
// CONDITIONS OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR 		  //
// NON-INFRINGEMENT. POINT GREY RESEARCH INC. SHALL NOT BE LIABLE FOR ANY 	  //
// DAMAGES, INCLUDING BUT NOT LIMITED TO ANY DIRECT, INDIRECT, INCIDENTAL,    //
// SPECIAL, PUNITIVE, OR CONSEQUENTIAL DAMAGES, OR ANY LOSS OF PROFITS,       //
// REVENUE, DATA OR DATA USE, ARISING OUT OF OR IN CONNECTION WITH THIS 	  //
// SOFTWARE OR OTHERWISE SUFFERED BY YOU AS A RESULT OF USING, MODIFYING 	  //
// OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.						  //
//============================================================================//


////////////////////////////////////////////////////////////////////////////////
//                   _                       __								  //
//                 /   \                  /      \							  //
//                '      \              /          \						  //
//               |       |Oo          o|            |						  //
//               `    \  |OOOo......oOO|   /        |						  //
//                `    \\OOOOOOOOOOOOOOO\//        /						  //
//                  \ _o\OOOOOOOOOOOOOOOO//. ___ /							  //
//              ______OOOOOOOOOOOOOOOOOOOOOOOo.___							  //
//               --- OO'* `OOOOOOOOOO'*  `OOOOO--							  //
//                   OO.   OOOOOOOOO'    .OOOOO o							  //
//                   `OOOooOOOOOOOOOooooOOOOOO'OOOo							  //
//                 .OO "OOOOOOOOOOOOOOOOOOOO"OOOOOOOo						  //
//             __ OOOOOO`OOOOOOOOOOOOOOOO"OOOOOOOOOOOOo						  //
//            ___OOOOOOOO_"OOOOOOOOOOO"_OOOOOOOOOOOOOOOO					  //
//              OOOOO^OOOO0`(____)/"OOOOOOOOOOOOO^OOOOOO					  //
//             OOOOO OO000/000000\000000OOOOOOOO OOOOOO						  //
//              OOOOO O0000000000000000 ppppoooooOOOOOO					  	  //
//              `OOOOO 0000000000000000 QQQQ "OOOOOOO"						  //
//               o"OOOO 000000000000000oooooOOoooooooO'						  //
//               OOo"OOOO.00000000000000000000OOOOOOOO'						  //
//              OOOOOO QQQQ 0000000000000000000OOOOOOO						  //
//             OOOOOO00eeee00000000000000000000OOOOOOOO.					  //
//            OOOOOOOO000000000000000000000000OOOOOOOOOO					  //
//            OOOOOOOOO00000000000000000000000OOOOOOOOOO					  //
//            `OOOOOOOOO000000000000000000000OOOOOOOOOOO					  //
//              "OOOOOOOO0000000000000000000OOOOOOOOOOO'					  //
//                "OOOOOOO00000000000000000OOOOOOOOOO"						  //
//     .ooooOOOOOOOo"OOOOOOO000000000000OOOOOOOOOOO"						  //
//   .OOO"""""""""".oOOOOOOOOOOOOOOOOOOOOOOOOOOOOo							  //
//   OOO         QQQQO"'                     `"QQQQ							  //
//   OOO																	  //
//   `OOo.																	  //
//     `"OOOOOOOOOOOOoooooooo.												  //
//																			  //
////////////////////////////////////////////////////////////////////////////////


/*
This little script initializes a PointGrey/FLIR USB3.1 Camera, and stores the 
acquired images within a new folder with a unique frameID_TimeStamp. 
I hope you enjoy this script!

Compile with: 
[LINUX]
g++ -Wall -g -Wno-unknown-pragmas -Wno-conversion-null -I/usr/include/spinnaker/ -o view_stream main.cpp  -lSpinnaker 
[WIN]
g++ -Wall -Wno-unknown-pragmas -Wno-conversion-null -I "C:\Program Files\Point Grey Research\Spinnaker\include" - ptgrey ptgrey.cpp -lSpinnaker
	Cheers,
		Andreas
*/
#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#endif

#ifdef linux
#include <termios.h>
#endif

#define _GLIBCXX_USE_CXX11_ABI 0

#include "Spinnaker.h"
#include "SpinGenApi/SpinnakerGenApi.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h> 
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>




// Use the following enum and global constant to select whether chunk data is 
// displayed from the image or the nodemap.ImagePtr pResultImage = pCam->GetNextImage();
enum chunkDataType
{
        IMAGE,
        NODEMAP
};
const chunkDataType chosenChunkData = IMAGE;

// Use the following enum and global constant to select whether a software or
// hardware trigger is used.
enum triggerType
{
        SOFTWARE,
        HARDWARE
};
const triggerType chosenTrigger = SOFTWARE;


/// FUNCTION DEFINITIONS
#ifdef linux
int kbhit(void)
{
	//###
	//### a function which emulates kbhit fron Windows, a returns 1 once the 
	//### kb has been hit.
	//###
	struct termios oldt, newt;
	int ch;
  	int oldf;

  	tcgetattr(STDIN_FILENO, &oldt);
  	newt = oldt;
  	newt.c_lflag &= ~(ICANON | ECHO);
  	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  	ch = getchar();

  	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF){
    	ungetc(ch, stdin);
    	return 1;
  	}
	return 0;
}
#endif

// Get current date/time, format is YYYY-MM-DD.HH:mm:ssdelay c++
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &tstruct);

    return buf;
}


// check if fiel exists ...
bool does_file_exist(const char *fileName)
{
    std::ifstream ifile(fileName);
    return ifile.good();
}

// This function configures the camera to use a trigger. First, trigger mode is 
// set to off in order to select the trigger source. Once the trigger source
// has been selected, trigger mode is then enabled, which has the camera 
// capture only a single image upon the execution of the chosen trigger.
int ConfigureTrigger(Spinnaker::GenApi::INodeMap & nodeMap)
{
        int result = 0;
        std::cout << std::endl << std::endl << "*** CONFIGURING TRIGGER ***" << std::endl << std::endl;
        if (chosenTrigger == SOFTWARE)
        {
                std::cout << "Software trigger chosen..." << std::endl;
        }
        else if (chosenTrigger == HARDWARE)
        {
                std::cout << "Hardware trigger chosen..." << std::endl;
        }
        try
        {
                //
                // Ensure trigger mode off
                //
                // *** NOTES ***
                // The trigger must be disabled in order to configure whether the source
                // is software or hardware.
                //
                Spinnaker::GenApi::CEnumerationPtr ptrTriggerMode = nodeMap.GetNode("TriggerMode");
                if (!IsAvailable(ptrTriggerMode) || !IsReadable(ptrTriggerMode))
                {
                        std::cout << "Unable to disable trigger mode (node retrieval). Aborting..." << std::endl;
                        return -1;
                }
                
                Spinnaker::GenApi::CEnumEntryPtr ptrTriggerModeOff = ptrTriggerMode->GetEntryByName("Off");
                if (!IsAvailable(ptrTriggerModeOff) || !IsReadable(ptrTriggerModeOff))
                {
                        std::cout << "Unable to disable trigger mode (enum entry retrieval). Aborting..." << std::endl;
                        return -1;
                }
                
                ptrTriggerMode->SetIntValue(ptrTriggerModeOff->GetValue());
                
                std::cout << "Trigger mode is ..." << ptrTriggerMode->GetCurrentEntry()->GetSymbolic() << std::endl;
                //
                // Select trigger source
                //
                // *** NOTES ***
                // The trigger source must be set to hardware or software while trigger 
                // mode is off.
                //
                Spinnaker::GenApi::CEnumerationPtr ptrTriggerSource = nodeMap.GetNode("TriggerSource");
                if (!IsAvailable(ptrTriggerSource) || !IsWritable(ptrTriggerSource))
                {
                        std::cout << "Unable to set trigger mode (node retrieval). Aborting..." << std::endl;
                        return -1;
                }
                if (chosenTrigger == SOFTWARE)
                {
                        // Set trigger mode to software
                        Spinnaker::GenApi::CEnumEntryPtr ptrTriggerSourceSoftware = ptrTriggerSource->GetEntryByName("Software");
                        if (!IsAvailable(ptrTriggerSourceSoftware) || !IsReadable(ptrTriggerSourceSoftware))
                        {
                                std::cout << "Unable to set trigger mode (enum entry retrieval). Aborting..." << std::endl;
                                return -1;
                        }
                        
                        ptrTriggerSource->SetIntValue(ptrTriggerSourceSoftware->GetValue());
                        
                        std::cout << "Trigger source set to ..." << ptrTriggerSource->GetCurrentEntry()->GetSymbolic() << std::endl;
                }
                else if (chosenTrigger == HARDWARE)
                {
                        // Set trigger mode to hardware ('Line0')
                        Spinnaker::GenApi::CEnumEntryPtr ptrTriggerSourceHardware = ptrTriggerSource->GetEntryByName("Line0");
                        if (!IsAvailable(ptrTriggerSourceHardware) || !IsReadable(ptrTriggerSourceHardware))
                        {
                                std::cout << "Unable to set trigger mode (enum entry retrieval). Aborting..." << std::endl;
                                return -1;
                        }
                        
                        ptrTriggerSource->SetIntValue(ptrTriggerSourceHardware->GetValue());
                        
                        std::cout << "Trigger source set to ..." << ptrTriggerSource->GetCurrentEntry()->GetSymbolic() << std::endl;
                }

				
				//#### Trigger Overlap
                Spinnaker::GenApi::CEnumerationPtr ptrTriggerOverlap = nodeMap.GetNode("TriggerOverlap");
                if (!IsAvailable(ptrTriggerOverlap) || !IsReadable(ptrTriggerOverlap))
                {
                        std::cout << "Unable to access trigger overlap (node retrieval). Aborting..." << std::endl;
                        return -1;
                }
                
                Spinnaker::GenApi::CEnumEntryPtr ptrTriggerOverlapMode = ptrTriggerOverlap->GetEntryByName("ReadOut");//ReadOut
                if (!IsAvailable(ptrTriggerOverlapMode) || !IsReadable(ptrTriggerOverlapMode))
                {
                        std::cout << "Unable to enable trigger overlap (enum entry retrieval). Aborting..." << std::endl;
                        return -1;
                }
				ptrTriggerOverlap->SetIntValue(ptrTriggerOverlapMode->GetValue());
				std::cout << "Trigger Overlap set to ... " << ptrTriggerOverlap->GetCurrentEntry()->GetSymbolic() << std::endl << std::endl;


                //
                // Turn trigger mode on
                //
                // *** LATER ***
                // Once the appropriate trigger source has been set, turn trigger mode 
                // on in order to retrieve images using the trigger.
                //
                Spinnaker::GenApi::CEnumEntryPtr ptrTriggerModeOn = ptrTriggerMode->GetEntryByName("On");
                if (!IsAvailable(ptrTriggerModeOn) || !IsReadable(ptrTriggerModeOn))
                {
                        std::cout << "Unable to enable trigger mode (enum entry retrieval). Aborting..." << std::endl;
                        return -1;
                }
                
                ptrTriggerMode->SetIntValue(ptrTriggerModeOn->GetValue());
                
                std::cout << "Trigger mode is ..." << ptrTriggerMode->GetCurrentEntry()->GetSymbolic() << std::endl;



        }
        catch (Spinnaker::Exception &e)
        {
                std::cout << "Error: " << e.what() << std::endl;
                result = -1;
        }
        return result;
}

// This function returns the camera to a normal state by turning off trigger 
// mode.
int ResetTrigger(Spinnaker::GenApi::INodeMap & nodeMap)
{
        int result = 0;
        try
        {
                //
                // Turn trigger mode back off
                //
                // *** NOTES ***
                // Once all images have been captured, turn trigger mode back off to
                // restore the camera to a clean state.
                //
                Spinnaker::GenApi::CEnumerationPtr ptrTriggerMode = nodeMap.GetNode("TriggerMode");
                if (!IsAvailable(ptrTriggerMode) || !IsReadable(ptrTriggerMode))
                {
                        std::cout << "Unable to disable trigger mode (node retrieval). Non-fatal error..." << std::endl;
                        return -1;
                }
                Spinnaker::GenApi::CEnumEntryPtr ptrTriggerModeOff = ptrTriggerMode->GetEntryByName("Off");
                if (!IsAvailable(ptrTriggerModeOff) || !IsReadable(ptrTriggerModeOff))
                {
                        std::cout << "Unable to disable trigger mode (enum entry retrieval). Non-fatal error..." << std::endl;
                        return -1;
                }
                ptrTriggerMode->SetIntValue(ptrTriggerModeOff->GetValue());
                std::cout << "Trigger mode is ..." << ptrTriggerMode->GetCurrentEntry()->GetSymbolic() << std::endl;
        }
        catch (Spinnaker::Exception &e)
        {
                std::cout << "Error: " << e.what() << std::endl;
                result = -1;
        }
        return result;
}

// This function configures the camera to add chunk data to each image. It does 
// this by enabling each type of chunk data before enabling chunk data mode. 
// When chunk data is turned on, the data is made available in both the nodemap 
// and each image.
int ConfigureChunkData(Spinnaker::GenApi::INodeMap & nodeMap)
{
        int result = 0;
        std::cout << std::endl << std::endl << "*** CONFIGURING CHUNK DATA ***" << std::endl << std::endl;
        try
        {
                //
                // Activate chunk mode
                //
                // *** NOTES ***
                // Once enabled, chunk data will be available at the end of the payload
                // of every image captured until it is disabled. Chunk data can also be 
                // retrieved from the nodemap.
                //
                Spinnaker::GenApi::CBooleanPtr ptrChunkModeActive = nodeMap.GetNode("ChunkModeActive");
                if (!Spinnaker::GenApi::IsAvailable(ptrChunkModeActive) || !Spinnaker::GenApi::IsWritable(ptrChunkModeActive))
                {
                        std::cout << "Unable to activate chunk mode. Aborting..." << std::endl << std::endl;
                        return -1;
                }
                ptrChunkModeActive->SetValue(true);
                std::cout << "Chunk mode activated..." << std::endl;
                //
                // Enable all types of chunk data
                //
                // *** NOTES ***
                // Enabling chunk data requires working with nodes: "ChunkSelector"
                // is an enumeration selector node and "ChunkEnable" is a boolean. It
                // requires retrieving the selector node (which is of enumeration node 
                // type), selecting the entry of the chunk data to be enabled, retrieving 
                // the corresponding boolean, and setting it to true. 
                //
                // In this example, all chunk data is enabled, so these steps are 
                // performed in a loop. Once this is complete, chunk mode still needs to
                // be activated.
                //
                Spinnaker::GenApi::NodeList_t entries;
                // Retrieve the selector node
                Spinnaker::GenApi::CEnumerationPtr ptrChunkSelector = nodeMap.GetNode("ChunkSelector");
                if (!Spinnaker::GenApi::IsAvailable(ptrChunkSelector) || !Spinnaker::GenApi::IsReadable(ptrChunkSelector))
                {
                        std::cout << "Unable to retrieve chunk selector. Aborting..." << std::endl << std::endl;
                        return -1;
                }
                // Retrieve entries
                ptrChunkSelector->GetEntries(entries);
                std::cout << "Enabling entries..." << std::endl;
				for(std::vector<float>::size_type i = 0; i < entries.size(); i++)
                //for (int i = 0; i < entries.size(); i++)
                {
                        // Select entry to be enabled
                        Spinnaker::GenApi::CEnumEntryPtr ptrChunkSelectorEntry = entries.at(i);
                        // Go to next node if problem occurs
                        if (!Spinnaker::GenApi::IsAvailable(ptrChunkSelectorEntry) || !Spinnaker::GenApi::IsReadable(ptrChunkSelectorEntry))
                        {
                                continue;
                        }
                        ptrChunkSelector->SetIntValue(ptrChunkSelectorEntry->GetValue());
                       std::cout << "\t" << ptrChunkSelectorEntry->GetSymbolic() << ": ";
                        // Retrieve corresponding boolean
                        Spinnaker::GenApi::CBooleanPtr ptrChunkEnable = nodeMap.GetNode("ChunkEnable");
                        // Enable the boolean, thus enabling the corresponding chunk data
                        if (!IsAvailable(ptrChunkEnable))
                        {
                                std::cout << "Node not available" << std::endl;
                        }
                        else if (ptrChunkEnable->GetValue())
                        {
                                std::cout << "Enabled" << std::endl;
                        }
                        else if (Spinnaker::GenApi::IsWritable(ptrChunkEnable))
                        {
                                ptrChunkEnable->SetValue(true);
                                std::cout << "Enabled" << std::endl;
                        }
                        else
                        {
                                std::cout << "Node not writable" << std::endl;
                        }
                }
        }
        catch (Spinnaker::Exception &e)
        {
                std::cout << "Error: " << e.what() << std::endl;
                result = -1;
        }
        return result;
}

// This function is not used, however it is taken straight from the the PTgrey
// examples an shows the many different image variables one can get.
// So simply take this as a reference.
//
// This function displays a select amount of chunk data from the image. Unlike
// accessing chunk data via the nodemap, there is no way to loop through all 
// available data.
int DisplayChunkData(Spinnaker::ImagePtr pImage)
{
        int result = 0;
        std::cout << "Printing chunk data from image..." << std::endl;
        try
        {
                //
                // Retrieve chunk data from image
                //
                // *** NOTES ***
                // When retrieving chunk data from an image, the data is stored in a
                // a ChunkData object and accessed with getter functions.
                //
                Spinnaker::ChunkData chunkData = pImage->GetChunkData();
        
                //
                // Retrieve exposure time; exposure time recorded in microseconds
                //
                // *** NOTES ***
                // Floating point numbers are returned as a float64_t. This can safely
                // and easily be statically cast to a double.
                //
                double exposureTime = static_cast<double>(chunkData.GetExposureTime());
                std::cout << "\tExposure time: " << exposureTime << std::endl;
                //
                // Retrieve frame ID
                //
                // *** NOTES ***
                // Integers are returned as an int64_t. As this is the typical integer
                // data type used in the Spinnaker SDK, there is no need to cast it.
                //
                int64_t frameID = chunkData.GetFrameID();
                std::cout << "\tFrame ID: " << frameID << std::endl;
                // Retrieve gain; gain recorded in decibels
                double gain = chunkData.GetGain();
                std::cout << "\tGain: " << gain << std::endl;
                // Retrieve height; height recorded in pixels
                int64_t height = chunkData.GetHeight();
                std::cout << "\tHeight: " << height << std::endl;
                // Retrieve offset X; offset X recorded in pixels
                int64_t offsetX = chunkData.GetOffsetX();
                std::cout << "\tOffset X: " << offsetX << std::endl;
                // Retrieve offset Y; offset Y recorded in pixels
                int64_t offsetY = chunkData.GetOffsetY();
                std::cout << "\tOffset Y: " << offsetY << std::endl;
                // Retrieve sequencer set active
                int64_t sequencerSetActive = chunkData.GetSequencerSetActive();
                std::cout << "\tSequencer set active: " << sequencerSetActive << std::endl;
                // Retrieve timestamp
                int64_t timestamp = chunkData.GetTimestamp();
                std::cout << "\tTimestamp: " << timestamp << std::endl;
                // Retrieve width; width recorded in pixels
                int64_t width = chunkData.GetWidth();
                std::cout << "\tWidth: " << width << std::endl;
        }
        catch (Spinnaker::Exception &e)
        {
                std::cout << "Error: " << e.what() << std::endl;
                result = -1;
        }
        
        return result;
}


int PrintDeviceInfo(Spinnaker::GenApi::INodeMap & nodeMap)
{
        int result = 0;

        std::cout << std::endl << "*** DEVICE INFORMATION ***" << std::endl << std::endl;
        try{
            Spinnaker::GenApi::FeatureList_t features;
            Spinnaker::GenApi::CCategoryPtr category = nodeMap.GetNode("DeviceInformation");
            if (Spinnaker::GenApi::IsAvailable(category) && Spinnaker::GenApi::IsReadable(category)){
                category->GetFeatures(features);
                Spinnaker::GenApi::FeatureList_t::const_iterator it;
                for (it = features.begin(); it != features.end(); ++it){
                    Spinnaker::GenApi::CNodePtr pfeatureNode = *it;
                    std::cout << pfeatureNode->GetName() << " : ";
                    Spinnaker::GenApi::CValuePtr pValue = (Spinnaker::GenApi::CValuePtr)pfeatureNode;
                    std::cout << (IsReadable(pValue) ? pValue->ToString() : "Node not readable");
                    std::cout << std::endl;
                }
            }
            else{
                std::cout << "Device control information not available." << std::endl;
            }
        }
        catch (Spinnaker::Exception &e){
            std::cout << "Error: " << e.what() << std::endl;
            result = -1;
        }
        return result;
}

int ConfigureCamera(Spinnaker::CameraPtr pCam, Spinnaker::GenApi::INodeMap & nodeMap, Spinnaker::GenApi::INodeMap & nodeMapTLDevice){

	///Configuring the Camera
    std::cout << std::endl <<"###   CAMERA CONFIGURATION   ###" << std::endl << std::endl;

	// #
	// # Set Buffer Handling Mode to Oldest First
	// #
	Spinnaker::GenApi::INodeMap & snodeMap = pCam->GetTLStreamNodeMap();
    Spinnaker::GenApi::CEnumerationPtr ptrBufferHandlingMode = snodeMap.GetNode("StreamBufferHandlingMode");
    if (!Spinnaker::GenApi::IsAvailable(ptrBufferHandlingMode)
         || !Spinnaker::GenApi::IsWritable(ptrBufferHandlingMode))
         std::cout << std::endl << "Unable to change buffer handling mode." << std::endl << std::endl;

    Spinnaker::GenApi::CEnumEntryPtr ptrStreamBufferHandlingMode_OldestFirst  = ptrBufferHandlingMode->GetEntryByName(
         "OldestFirst");
    if (!Spinnaker::GenApi::IsAvailable(ptrStreamBufferHandlingMode_OldestFirst)
         || !Spinnaker::GenApi::IsReadable(ptrStreamBufferHandlingMode_OldestFirst))
         std::cout << std::endl <<"Unable to set buffer handling mode." << std::endl << std::endl;
    
	int64_t bufferHandlingModeOldest_First = ptrStreamBufferHandlingMode_OldestFirst->GetValue();
	ptrBufferHandlingMode->SetIntValue(bufferHandlingModeOldest_First);
	std::cout << "Buffer Handling Mode set to " << ptrBufferHandlingMode->GetCurrentEntry()->GetSymbolic() << "." << std::endl << std::endl;

	// #
	// # Set Pixel Format to Mono8
	// #
    Spinnaker::GenApi::CEnumerationPtr ptrPixelFormat = nodeMap.GetNode("PixelFormat");
    if (Spinnaker::GenApi::IsAvailable(ptrPixelFormat) && Spinnaker::GenApi::IsWritable(ptrPixelFormat))
    {
    	// Retrieve the desired entry node from the enumeration node
        Spinnaker::GenApi::CEnumEntryPtr ptrPixelFormatMono8 = ptrPixelFormat->GetEntryByName("Mono8");
        if (Spinnaker::GenApi::IsAvailable(ptrPixelFormatMono8) && Spinnaker::GenApi::IsReadable(ptrPixelFormatMono8))
        {
        // Retrieve the integer value from the entry node
    	    int64_t pixelFormatMono8 = ptrPixelFormatMono8->GetValue();
            // Set integer as new value for enumeration node
            ptrPixelFormat->SetIntValue(pixelFormatMono8);
            std::cout << "Pixel format set to " << ptrPixelFormat->GetCurrentEntry()->GetSymbolic() << "..." << std::endl;
    	}
        else
        {
        	std::cout << "Pixel format mono 8 not available..." << std::endl;
        }
        }
        	else
        {
        	std::cout << "Pixel format not available..." << std::endl;
        }


	// #
	// # Set Image Fiel Format to RAW
	// #
    Spinnaker::GenApi::CEnumerationPtr ptrImageFileFormat = nodeMap.GetNode("ImageFileFormat");
    if (Spinnaker::GenApi::IsAvailable(ptrImageFileFormat) && Spinnaker::GenApi::IsWritable(ptrImageFileFormat)){
    	// Retrieve the desired entry node from the enumeration node
        Spinnaker::GenApi::CEnumEntryPtr ptrImageFileFormatMode = ptrImageFileFormat->GetEntryByName("RAW");
    	if (Spinnaker::GenApi::IsAvailable(ptrImageFileFormatMode) && Spinnaker::GenApi::IsReadable(ptrImageFileFormatMode)){
        	// Retrieve the integer value from the entry node
            int64_t ImageFileFormatMode = ptrImageFileFormatMode->GetValue();
        	// Set integer as new value for enumeration node
        	ptrImageFileFormat->SetIntValue(ImageFileFormatMode);
            std::cout << "Image File Format set to " << ptrImageFileFormat->GetCurrentEntry()->GetSymbolic() << "." << std::endl << std::endl;
      	}
        else{
       		std::cout << "Image File Format not available..." << std::endl << std::endl;
       	}
  	}
    else{
  		std::cout << "Image File Format not available..." << std::endl << std::endl;
   	}

	// #
	// # Set ADC Bit Depth to Bit8
	// #
    Spinnaker::GenApi::CEnumerationPtr ptrAdcBitDepth = nodeMap.GetNode("AdcBitDepth");
    if (Spinnaker::GenApi::IsAvailable(ptrAdcBitDepth) && Spinnaker::GenApi::IsWritable(ptrAdcBitDepth)){
    	// Retrieve the desired entry node from the enumeration node
        Spinnaker::GenApi::CEnumEntryPtr ptrAdcBitDepthBit8 = ptrAdcBitDepth->GetEntryByName("Bit8");
    	if (Spinnaker::GenApi::IsAvailable(ptrAdcBitDepthBit8) && Spinnaker::GenApi::IsReadable(ptrAdcBitDepthBit8)){
        	// Retrieve the integer value from the entry node
            int64_t AdcBitDepthBit8 = ptrAdcBitDepthBit8->GetValue();
        	// Set integer as new value for enumeration node
        	ptrAdcBitDepth->SetIntValue(AdcBitDepthBit8);
            std::cout << "ADC Bit Depth set to " << ptrAdcBitDepth->GetCurrentEntry()->GetSymbolic() << "." << std::endl << std::endl;
      	}
        else{
       		std::cout << "ADC Bit Depth Bit8 not available..." << std::endl << std::endl;
       	}
  	}
    else{
  		std::cout << "ADC Bit Depth not available..." << std::endl << std::endl;
   	}

	// #
	// # Set Acquisition Mode to Continuous 
	// #
	Spinnaker::GenApi::CEnumerationPtr ptrAcquisitionMode = nodeMap.GetNode("AcquisitionMode");
    if (!Spinnaker::GenApi::IsAvailable(ptrAcquisitionMode) || !Spinnaker::GenApi::IsWritable(ptrAcquisitionMode)){
        std::cout << "Unable to set acquisition mode to continuous (enum retrieval). Aborting..." << std::endl << std::endl;
    }
    // Retrieve entry node from enumeration node
    Spinnaker::GenApi::CEnumEntryPtr ptrAcquisitionModeContinuous = ptrAcquisitionMode->GetEntryByName("Continuous");
    if (!Spinnaker::GenApi::IsAvailable(ptrAcquisitionModeContinuous) || !Spinnaker::GenApi::IsReadable(ptrAcquisitionModeContinuous)){
        std::cout << "Unable to set acquisition mode to continuous (entry retrieval). Aborting..." << std::endl << std::endl;
    }
    // Retrieve integer value from entry node
    int64_t acquisitionModeContinuous = ptrAcquisitionModeContinuous->GetValue();
    // Set integer value from entry node as new value of enumeration node
    ptrAcquisitionMode->SetIntValue(acquisitionModeContinuous);
    std::cout <<"Acquisition mode set to continuous..." << std::endl << std::endl;

	
	// #
	// # Disable Auto-Exposure & Set Exposure to fixed value to allow maximum FrameRate
	// #
    Spinnaker::GenApi::CEnumerationPtr ptrExposureAuto = nodeMap.GetNode("ExposureAuto");
    if (!Spinnaker::GenApi::IsAvailable(ptrExposureAuto) || !Spinnaker::GenApi::IsWritable(ptrExposureAuto)){
        std::cout << "Unable to disable automatic exposure (node retrieval). Aborting..." << std::endl << std::endl;
    }
    Spinnaker::GenApi::CEnumEntryPtr ptrExposureAutoOff = ptrExposureAuto->GetEntryByName("Off");
    if (!Spinnaker::GenApi::IsAvailable(ptrExposureAutoOff) || !Spinnaker::GenApi::IsReadable(ptrExposureAutoOff)){
        std::cout << "Unable to disable automatic exposure (enum entry retrieval). Aborting..." << std::endl << std::endl;
    }
    ptrExposureAuto->SetIntValue(ptrExposureAutoOff->GetValue());
    std::cout << "Automatic exposure disabled..." << std::endl;
    Spinnaker::GenApi::CFloatPtr ptrExposureTime = nodeMap.GetNode("ExposureTime");
    if (!Spinnaker::GenApi::IsAvailable(ptrExposureTime) || !Spinnaker::GenApi::IsWritable(ptrExposureTime)){
        std::cout << "Unable to set exposure time. Aborting..." << std::endl << std::endl;
    }
    const double exposureTimeMax = ptrExposureTime->GetMax();
    double exposureTimeToSet = 1790.0;
    if (exposureTimeToSet > exposureTimeMax){
        exposureTimeToSet = exposureTimeMax;
    }
    ptrExposureTime->SetValue(exposureTimeToSet);
    std::cout << "Exposure time set to " << exposureTimeToSet << " us." << std::endl << std::endl;
	
	
	// #
	// # Disable Auto-Gain & Set Gain to fixed value
	// #
    Spinnaker::GenApi::CEnumerationPtr ptrGainAuto = nodeMap.GetNode("GainAuto");
    if (!Spinnaker::GenApi::IsAvailable(ptrGainAuto) || !Spinnaker::GenApi::IsWritable(ptrGainAuto)){
        std::cout << "Unable to disable automatic gain (node retrieval). Aborting..." << std::endl << std::endl;
    }
   Spinnaker::GenApi::CEnumEntryPtr ptrGainAutoOff = ptrGainAuto->GetEntryByName("Off");
    if (!Spinnaker::GenApi::IsAvailable(ptrGainAutoOff) || !Spinnaker::GenApi::IsReadable(ptrGainAutoOff)){
        std::cout << "Unable to disable automatic gain (enum entry retrieval). Aborting..." << std::endl << std::endl;
    }
    ptrGainAuto->SetIntValue(ptrGainAutoOff->GetValue());
    std::cout << "Automatic gain disabled..." << std::endl;
    Spinnaker::GenApi::CFloatPtr ptrGain = nodeMap.GetNode("Gain");
    if (!Spinnaker::GenApi::IsAvailable(ptrGain) || !Spinnaker::GenApi::IsWritable(ptrGain)){
        std::cout << "Unable to set gain value. Aborting..." << std::endl << std::endl;
    }
    const double GainMax = ptrGain->GetMax();
    double GainToSet = 18.0;
    if (GainToSet > GainMax){
        GainToSet = GainMax;
    }
    ptrGain->SetValue(GainToSet);
    std::cout << "Gain set to " << GainToSet << "." << std::endl << std::endl;

	return 0;
}


int main()
{
	using namespace std::chrono;
    using Framerate = duration<steady_clock::rep, std::ratio<6667, 1000000>>;
    auto next = steady_clock::now() + Framerate{1};
	

	bool GetImage = true;		
	std::stringstream filename;

    std::cout << "BEGIN VIEW STREAM" << std::endl;
    Spinnaker::SystemPtr system = Spinnaker::System::GetInstance();
    Spinnaker::CameraList camList = system->GetCameras();
    unsigned int numCameras = camList.GetSize();
    if(numCameras>0){
        std::cout << "SUCCESS ... Camera detected!" << std::endl;
    }
    Spinnaker::CameraPtr pCam = camList.GetByIndex(0);
    Spinnaker::GenApi::INodeMap & nodeMapTLDevice = pCam->GetTLDeviceNodeMap();
    PrintDeviceInfo(nodeMapTLDevice);

    // Initialize camera
    pCam->Init();

    // Retrieve GenICam nodemap
    Spinnaker::GenApi::INodeMap & nodeMap = pCam->GetNodeMap();
	ConfigureCamera(pCam, nodeMap, nodeMapTLDevice);
	// Configure chunk data
    ConfigureChunkData(nodeMap);

 	// Configure trigger
 	ConfigureTrigger(nodeMap);

	/// Begin Acquisition
   	pCam->BeginAcquisition();
	std::cout << std::endl <<"SUCCESS: Acquiring images..." << std::endl;

	Spinnaker::GenApi::CCommandPtr ptrSoftwareTriggerCommand = nodeMap.GetNode("TriggerSoftware");
   	
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

   	std::stringstream ss;
	ss << cwd << "/" << currentDateTime().c_str();
	std::string outdir = ss.str(); 
	std::cout << outdir << std::endl;
	mkdir(outdir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	filename << outdir << "/" << "log.txt";
	const std::string& tmp = filename.str();
	const char* path = tmp.c_str();
	std::ofstream log;
	log.open(path);

	

	do{	
		while (steady_clock::now() < next)		
		if(GetImage){
			GetImage = false;
			ptrSoftwareTriggerCommand->Execute();
        	Spinnaker::ImagePtr pResultImage = pCam->GetNextImage();
			// Create a unique filename
           	std::stringstream filename;
			Spinnaker::ChunkData chunkData = pResultImage->GetChunkData();
			int64_t frameID = chunkData.GetFrameID();
			int64_t timestamp = chunkData.GetTimestamp();
			filename << outdir << "/" << frameID << "_" << timestamp <<	".raw";
			const std::string& tmp = filename.str();
			const char* path = tmp.c_str();
			log << frameID << "\t" << timestamp << std::endl;
			pResultImage->Save(path);
		}   	
   		next += Framerate{1};
		GetImage = true;
	}while(!kbhit());

	log.close();
  

/// How to turn off the thing: stop acquizition, de-initialize Camera, destroy 
/// Ptr, clear camList, release System:
	ResetTrigger(nodeMap);
    pCam->EndAcquisition();
    std::cout << std::endl <<"SUCCESS: Stopped acquisition..." << std::endl;
    pCam->DeInit();
    pCam = NULL;
    camList.Clear();
    system->ReleaseInstance();

}


