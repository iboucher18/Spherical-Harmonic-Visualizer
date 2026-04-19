# Spherical-Harmonic-Visualizer
Final Project for PHYS 5810

Dependencies

Python: 
- matplotlib
- pandas
- numpy
- json

C++:
- GSL

How to setup and run

OPTION 1: Run everything all at once. This requires you having a python environment with all the required dependencies installed, and also has GSL installed

1: Setup your python virtual environment (See dependencies above).

2: Verify you have the GSL C++ library available.

3: Setup your input file (sample_input.json)
- If you desire any different specifications, make those changes

4: Run 'python main.py'

OPTION 2: Run in 2 stages. Necessary when running using OSC
- I coded this program with the idea I would be able to run everything all at once, however OSC does not have the right matplotlib version, so I was unable to do that. I instead was able to run in 2 stages: The first stage is running the calculator on OSC, and the second stage is running the plotting on my own computer.

1: Put the following files onto OSC
- main_v2_part1.py
- all .h and .cpp files

2: Setup your input file (sample_input.json)
- If you desire any different specifications, make those changes

3: run "python main_v2_part1.py"

4: three files will be in your output directory. Pull these files down from OSC and place them in directory with your python environment
- SphereData.csv
- SphereData_postFourierTransform_real.csv
- SphereData_postFourierTransform_imag.csv

5: run 'python main_v2_part2.py PATH_TO_FOLDER_WITH_SPHERE_FILES'

Description
**TODO**: describe when done