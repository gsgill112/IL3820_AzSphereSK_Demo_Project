#  Copyright (c) Microsoft Corporation. All rights reserved.
#  Licensed under the MIT License.

cmake_minimum_required (VERSION 3.8)
project (IL3820_AzSphereSK_Demo_Project_HighLevelCore C)

# Create executable
add_executable (${PROJECT_NAME} main.c "../EPD/IL3820_2in9_Driver.c" "../EPD/AzSphere_Interface.c" "ImageData.c" "logo.c")
target_link_libraries (${PROJECT_NAME} applibs pthread gcc_s c)

# Add MakeImage post-build command
include (${AZURE_SPHERE_MAKE_IMAGE_FILE})
