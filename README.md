# ASE_Animator
Load an ASE file and add an animation 
[![Watch the video](https://drive.google.com/file/d/1OhnoJpqAuY9rjQn9OzrdAcd3SSRnyTUs/view?usp=sharing)](https://drive.google.com/file/d/1WRW8tD48aMaTyyoweVMHK5dsM3r6FCcx/view)

# Requirements 
1. Visual Studio 2017, C++ Development Kit 
2. CMake https://cmake.org/download/

# Build Steps
1. Install Visual Studio 2017
   -Make sure to install C++ Development Kit
2. Install CMake and start CMake

3. Build with CMake
   1) Browse source code path : "YOUR_PATH/ASE_Animator"
   2) Browse build path : "YOUR_PATH/ASE_Animator/build"
   3) Click "Configure" button at left bottom side
      - Select "Visual Studio 15 2017" as your generator
      - An error will be occurred! It's fine
   4) Set "GLUT_ROOT_PATH" to "YOUR_PATH/ASE_Animator/glut3.6_win32"
   5) Click "Configure" again
   6) Click "Generate"
      -CMake will generate build file at your build path
      
4. Go to your build path and open "CG_Animation.sln" file
   -If you need to install additional packages then install them too
