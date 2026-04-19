import subprocess as sp
import json
import pandas as pd
import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm, colors

# DEFAULT VALUES
default_makefile_filepath = "make_final_project"
default_output_path = "Outputs/"

class InputData:
    def __init__(self,
         makefile_filepath: str,
         output_path: str,
         shapes,
         do_fourier: bool,
         l: int,
         seed: int
     ):
        self.makefile_filepath = makefile_filepath
        self.output_path = output_path
        self.shapes = shapes
        self.do_fourier = do_fourier
        self.l = l
        self.seed = seed



def ReadInputFile(input_filepath) -> InputData:
    with open (input_filepath, 'r') as file:
        inputfile = json.load(file)

    # required fields in input file
    do_fourier = inputfile['do-fourier']
    l=inputfile['l']
    shapes=inputfile['shapes']
    seed=inputfile['seed']

    # optional fields in input file
    if "makefile-filepath" in inputfile.keys():
        makefile_filepath = inputfile['makefile-filepath']
    else:
        makefile_filepath = default_makefile_filepath

    if "output-path" in inputfile.keys():
        output_path = inputfile["output-path"]
    else:
        output_path = default_output_path

    if not os.path.isdir(output_path):
        os.mkdir(output_path)

    return InputData(makefile_filepath, output_path, shapes, do_fourier, l, seed)


def BuildAndRunCalculator(input_data: InputData):
    print("Clean Calculator")
    sp.run(f"make -f {input_data.makefile_filepath} clean", shell=True, executable="/bin/bash")\

    # Build cpp
    print("Build Calculator")
    build_output = sp.run(
        f"make -f {input_data.makefile_filepath}",
        shell=True,
        executable="/bin/bash",
        capture_output=True,
        text=True
    )
    if len(build_output.stderr) != 0:
        print(build_output.stderr)
        raise RuntimeError("Failed to build final_project.cpp!")

    # parse input data to create runtime arguments for cpp, then run
    calculator_command = f"./final_project.x {input_data.do_fourier} {input_data.l} {input_data.output_path} {input_data.seed}"
    for shape in input_data.shapes:
        type = shape['type'].lower()
        if type == 'circle':
            calculator_command += ' ' + type + ' ' + str(shape['radius'])
        elif type == 'square':
            calculator_command += ' ' + type + ' ' + str(shape['side_length'])
        elif type == 'rectangle':
            calculator_command += ' ' + type + ' ' + str(shape['length']) + ' ' + str(shape['width'])

        # If the user defined a specific location for this shape, put it into the executable call
        if 'x' in shape.keys() and 'y' in shape.keys() and 'z' in shape.keys():
            calculator_command += ' ' + str(shape['x']) + ' ' + str(shape['y']) + ' ' + str(shape['z'])

    print(f"Run Calculator: {calculator_command}")

    run_output = sp.run(
        calculator_command,
        shell=True,
        executable="/bin/bash",
        capture_output=True,
        text=True
    )
    if len(run_output.stderr) != 0:
        print(run_output.stderr)
        raise RuntimeError("Failed to run final_project.cpp!")


def RunCalculator_Windows(input_data: InputData):
    # parse input data to create runtime arguments for cpp, then run
    calculator_command = f"./final_project.exe {input_data.do_fourier} {input_data.l} {input_data.output_path} {input_data.seed}"
    for shape in input_data.shapes:
        type = shape['type'].lower()
        if type == 'circle':
            calculator_command += ' ' + type + ' ' + str(shape['radius'])
        elif type == 'square':
            calculator_command += ' ' + type + ' ' + str(shape['side_length'])
        elif type == 'rectangle':
            calculator_command += ' ' + type + ' ' + str(shape['length']) + ' ' + str(shape['width'])

        # If the user defined a specific location for this shape, put it into the executable call
        if 'x' in shape.keys() and 'y' in shape.keys() and 'z' in shape.keys():
            calculator_command += ' ' + str(shape['x']) + ' ' + str(shape['y']) + ' ' + str(shape['z'])

    print(f"Run Calculator: {calculator_command}")

    run_output = sp.run(
        calculator_command,
        capture_output=True,
        text=True
    )
    if len(run_output.stderr) != 0:
        print(run_output.stderr)
        raise RuntimeError("Failed to run final_project.cpp!")



def VisualizeSphere(sphereData):
    r = 1 # Must match radius value in final_project.cpp
    step_count = 41 # Must match step count in final_project.cpp

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    theta = sphereData["Theta"].to_numpy()
    phi = sphereData["Phi"].to_numpy()
    values = sphereData["Value"].to_numpy()

    theta_reshaped = np.reshape(theta, [step_count, -1])
    phi_reshaped = np.reshape(phi, [step_count, -1])
    values_reshaped = np.reshape(values, [step_count, -1])

    norm = colors.Normalize(vmin=np.min(values), vmax=np.max(values))

    x = r * np.sin(phi_reshaped) * np.cos(theta_reshaped)
    y = r * np.sin(phi_reshaped) * np.sin(theta_reshaped)
    z = r * np.cos(phi_reshaped)

    ax.plot_surface(x, y, z, cmap=cm.coolwarm, facecolors=cm.coolwarm(norm(values_reshaped)))

    plt.show()




def main():
    # TODO: create optional argument for specifying input filepath location
    input_file_filepath = "sample_input.json"

    input_data = ReadInputFile(input_file_filepath)

    # BuildAndRunCalculator(input_data)
    # RunCalculator_Windows(input_data) # Does not build calculator for you

    sphere_data_filepath = os.path.join(input_data.output_path, 'SphereData.csv')
    calculator_results = pd.read_csv(sphere_data_filepath)
    VisualizeSphere(calculator_results)

    if input_data.do_fourier:
        fourier_sphere_data_filepath = os.path.join(input_data.output_path, 'SphereData_postFourierTransform.csv')
        calculator_results_fourier = pd.read_csv(fourier_sphere_data_filepath)
        VisualizeSphere(calculator_results_fourier)



if __name__ == "__main__":
    main()