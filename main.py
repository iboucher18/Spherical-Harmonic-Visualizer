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
         seed: int
     ):
        self.makefile_filepath = makefile_filepath
        self.output_path = output_path
        self.shapes = shapes
        self.do_fourier = do_fourier
        self.seed = seed



def ReadInputFile(input_filepath) -> InputData:
    with open (input_filepath, 'r') as file:
        inputfile = json.load(file)

    # required fields in input file
    do_fourier = inputfile['do-fourier']
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

    return InputData(makefile_filepath, output_path, shapes, do_fourier, seed)


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
    calculator_command = f"./final_project.x {input_data.do_fourier} {input_data.output_path} {input_data.seed}"
    for shape in input_data.shapes:
        type = shape['type'].lower()
        if type == 'circle':
            calculator_command += ' ' + type + ' ' + str(shape['radius'])
        elif type == 'square':
            calculator_command += ' ' + type + ' ' + str(shape['side_length'])
        elif type == 'rectangle':
            calculator_command += ' ' + type + ' ' + str(shape['length']) + ' ' + str(shape['width'])

        # If the user defined a specific location for this shape, put it into the executable call
        if 'theta-location' in shape.keys() and 'phi-location' in shape.keys(): # TODO: adjust this to match whatever the c++ code becomes
            calculator_command += ' ' + str(shape['theta-location']) + ' ' + str(shape['phi-location'])

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
    calculator_command = f"./final_project.exe {input_data.do_fourier} {input_data.output_path} {input_data.seed}"
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
    r = 3 # TODO: i guess make this an optional var in the input file. make sure c++ uses this var as well
    step_size = 41 # TODO: i guess make this an optional var in the input file. make sure c++ uses this var as well

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    theta = sphereData["Theta"].to_numpy()
    phi = sphereData["Phi"].to_numpy()
    values = sphereData["Value"].to_numpy()

    theta_reshaped = np.reshape(theta, [step_size, -1])
    phi_reshaped = np.reshape(phi, [step_size, -1])
    values_reshaped = np.reshape(values, [step_size, -1])

    norm = colors.Normalize(vmin=np.min(values), vmax=np.max(values))

    x = r * np.sin(phi_reshaped) * np.cos(theta_reshaped)
    y = r * np.sin(phi_reshaped) * np.sin(theta_reshaped)
    z = r * np.cos(phi_reshaped)

    # TODO: remove
    # with open("test.csv", 'w') as file:
    #     file.write(','.join(['x', 'y', 'z', 'value']))
    #     file.write('\n')
    #     for counter1 in range(len(x)):
    #         for counter2 in range(len(x[0])):
    #             # if counter1 == len(x)-1:
    #                 # print(x[counter1][counter2], y[counter1][counter2], z[counter1][counter2], values_reshaped[counter1][counter2])
    #             file.write(','.join([
    #                 str(x[counter1][counter2]),
    #                 str(y[counter1][counter2]),
    #                 str(z[counter1][counter2]),
    #                 str(values_reshaped[counter1][counter2])]))
    #             file.write('\n')

    # TODO: remove
    # with open("test2.csv", 'w') as file:
    #     file.write(','.join(['theta', 'phi', 'value']))
    #     file.write('\n')
    #     for counter1 in range(len(x)):
    #         for counter2 in range(len(x[0])):
    #             # if counter1 == len(x)-1:
    #             # print(x[counter1][counter2], y[counter1][counter2], z[counter1][counter2], values_reshaped[counter1][counter2])
    #             file.write(','.join([
    #                 str(theta_reshaped[counter1][counter2]),
    #                 str(phi_reshaped[counter1][counter2]),
    #                 str(values_reshaped[counter1][counter2])]))
    #             file.write('\n')


    ax.plot_surface(x, y, z, cmap=cm.coolwarm, facecolors=cm.coolwarm(norm(values_reshaped)))

    plt.show()




def main():
    # TODO: create optional argument for specifying input filepath location
    input_file_filepath = "sample_input.json"

    input_data = ReadInputFile(input_file_filepath)

    # BuildAndRunCalculator(input_data)
    RunCalculator_Windows(input_data) # Does not build calculator for you

    sphere_data_filepath = os.path.join(input_data.output_path, 'SphereData.csv')
    calculator_results = pd.read_csv(sphere_data_filepath)
    VisualizeSphere(calculator_results)

    # TODO: visualize fourier results



#
# from mpl_toolkits.mplot3d import Axes3D
# from matplotlib import cm, colors
# import matplotlib.pyplot as plt
# import numpy as np
#
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
#
# v, u = np.mgrid[0:2*np.pi:50j, 0:np.pi:50j]
#
# print(len(u))
# print(len(u[0]))
#
# strength = u
# norm=colors.Normalize(vmin = np.min(strength),
#                       vmax = np.max(strength), clip = False)
#
# x = 10 * np.sin(u) * np.cos(v)
# y = 10 * np.sin(u) * np.sin(v)
# z = 10 * np.cos(u)
#
# print(u[0])
# print(v[0])
#
# ax.plot_surface(x, y, z, rstride=1, cstride=1, cmap=cm.coolwarm,
#                        linewidth=0, antialiased=False,
#                        facecolors=cm.coolwarm(norm(strength)))
#
# plt.show()



#
# # 1. Create a flat square grid (2D plane at z=1)
# n = 50
# x_range = np.linspace(-1, 1, n)
# y_range = np.linspace(-1, 1, n)
# X, Y = np.meshgrid(x_range, y_range)
# Z = np.ones_like(X)  # Plane positioned above the origin
#
# # 2. Normalize vectors to project them onto a unit sphere surface
# # Magnitude of each vector from origin to grid point
# mag = np.sqrt(X**2 + Y**2 + Z**2)
#
# # New coordinates on the sphere
# X_sphere = X / mag
# Y_sphere = Y / mag
# Z_sphere = Z / mag
#
# # 3. Visualization
# fig = plt.figure(figsize=(8, 8))
# ax = fig.add_subplot(111, projection='3d')
#
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
#
# u, v = np.mgrid[0:np.pi:50j, 0:2*np.pi:50j]
#
# strength = u
# norm=colors.Normalize(vmin = np.min(strength),
#                       vmax = np.max(strength), clip = False)
#
# x = 0.99* np.sin(u) * np.cos(v)
# y = 0.99* np.sin(u) * np.sin(v)
# z = 0.99* np.cos(u)
#
# print(norm(strength))
#
# ax.plot_surface(x, y, z, rstride=1, cstride=1, cmap=cm.coolwarm,
#                        linewidth=0, antialiased=False,
#                        facecolors=cm.coolwarm(norm(strength)))
#
# ax.plot_surface(X_sphere, Y_sphere, Z_sphere, cmap='viridis', edgecolors='k', lw=0.1)
#
# ax.set_title("Square Projected onto Sphere Surface")
# plt.show()




if __name__ == "__main__":
    main()