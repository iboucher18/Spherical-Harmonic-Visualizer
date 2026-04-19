# Spherical-Harmonic-Visualizer
# Final Project for PHYS 5810

## Dependencies

Python: 
- matplotlib
- pandas
- numpy
- json

C++:
- GSL

How to setup and run

## OPTION 1: Run everything all at once. This requires you having a python environment with all the required dependencies installed, and also has GSL installed

1: Setup your python virtual environment (See dependencies above).

2: Verify you have the GSL C++ library available.

3: Setup your input file (sample_input.json)
- If you desire any different specifications, make those changes

4: Run 'python main.py' - this will execute Parts 1-4 from the description section (see below)

## OPTION 2: Run in 2 stages. Necessary when running using OSC
- I coded this program with the idea I would be able to run everything all at once, however OSC does not have the right matplotlib version, so I was unable to do that. I instead was able to run in 2 stages: The first stage is running the calculator on OSC, and the second stage is running the plotting on my own computer.

1: Put the following files onto OSC
- main_v2_part1.py
- all .h and .cpp files

2: Setup your input file (sample_input.json)
- If you desire any different specifications, make those changes

3: run "python main_v2_part1.py" - this will run Parts 1-3 in the description (see below)

4: Three files will be in your output directory. Pull these files down from OSC and place them in directory with your python environment. If you don't have a way to do this, you can use the download_folder.py script (edit the username, password, folder, remoteFolder, and localFolder fields to your OSC login information and desired filepaths). 
- SphereData.csv
- SphereData_postFourierTransform_real.csv
- SphereData_postFourierTransform_imag.csv

5: run 'python main_v2_part2.py PATH_TO_FOLDER_WITH_SPHERE_FILES true' - this will run Part 4 in the Description (see below)

## Additional Notes on Running

The user is not intended to interact with the cpp executable when running the code normally: They should run one of the python scripts and modify the json input file to run it. Technically, the user can run it this way, however it involves a lot of runtime arguments that are a little unwieldy

To run the tests, run "./final_project.x ExecuteTests".

# Description:

This program takes a set of user-defined shapes (either squares or rectangles), projects them onto a sphere, and plots it. Additionally, if the user sets the appropriate input, a fourier transform will be executed onto the resulting sphere, and both the real and imaginary components of that transform are saved and plotted (in addition to the original sphere's plot).

## Part 1: Input File

In sample_input.json, there are a few inputs that the user can modify. 
- output-path: this defines where the resulting csv files are placed relative to the location of the cpp executable. This defaults to a folder called Outputs/
- seed: This is the seed used for any random draws in the cpp executable. The cpp executable only does random draws for the center points of shapes that do not have a center point specified by the user (see below)
- do-fourier: Either true or false. Dictates whether the user wants to execute the fourier transform on the created sphere.
- l: If the user wants to do a fourier transform on their created sphere, this l value is the l value that will be used. This value must be set, even if a fourier transform is not being done. If the fourier transform is not being executed for a particular run, the l value is not used.
- Shapes: These are the shapes that can be projected onto the unit sphere. For each shape, the required inputs are a type (either 'square' or 'rectangle') and the corresponding dimensions for that shape (square: sideLength, rectangle: length+width). Optionally, the user may specify a center location for the shape to be placed before being projected onto the sphere. If the specified center would place any part of the shape within the sphere, the code will error out, stating that your center location is not valid (note that the sphere has a hardcoded radius of 1, and cannot be changed without changing the code). If a center is not specified for any given shape, the code randomly draws one.

Python is used to parse the input file, build the executable using a makefile, and run the executable. This executable is coded in C++, and does all of the calculations described in Parts 2 and 3. 

## Part 2: Generating Sphere

When all of the desired inputs are given correctly, the code will generate a csv file that contains the resulting sphere. The csv file has 3 columns: theta, phi, and a value. This value will be 0 when no shape is present, and greater than 0 when a shape is present. The closer a point is to the center of a shape, the higher the value will be.

## Part 3: Fourier Transform (Optional)

If the do-fourier parameter is set to true, then a fourier transform is done on the sphere from Part 2. This fourier transform is done in spherical coordinates, which means a specific L value is needed in order to be done. The user specifies the L value using the 'l' input in their input file. Both the real and imaginary components of the resulting transform are saved to csv files, whose layout is identical to the sphere from Part 2.

## Part 4: Plotting

After Parts 2 and 3 are complete, python is used to plot the resulting spheres. If the user decides to run with a fourier transform, then 3 plots are created at once: A plot containing the original sphere, a plot containing the real component of the fourier transformed sphere, and a plot containing the imaginary component of the fourier transformed sphere. If the user runs without the fourier transform, then only 1 plot is created, which contains the original sphere.