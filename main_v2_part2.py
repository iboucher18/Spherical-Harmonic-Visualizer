import pandas as pd
import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm, colors
import sys

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
    ax.set_title('Sphere with Inputted Shapes')

    plt.show()

def VisualizeSpheres(spheres):
    r = 1 # Must match radius value in final_project.cpp
    step_count = 41 # Must match step count in final_project.cpp

    fig, ax = plt.subplots(1, 3, subplot_kw={'projection': '3d'})

    titles = [
        'Original Sphere',
        'FFT (RE)',
        'FFT (IM)'
    ]

    for counter in range(len(spheres)):
        sphereData = spheres[counter]
        title = titles[counter]

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

        ax[counter].plot_surface(x, y, z, cmap=cm.coolwarm, facecolors=cm.coolwarm(norm(values_reshaped)))
        ax[counter].set_title(title)

    plt.show()



def main():
    output_folderpath = sys.argv[1]
    run_fourier = sys.argv[2]

    sphere_data_filepath = os.path.join(output_folderpath, 'SphereData.csv')
    calculator_results = pd.read_csv(sphere_data_filepath)

    if run_fourier.lower() == 'true':
        fourier_sphere_data_filepath_real = os.path.join(output_folderpath, 'SphereData_postFourierTransform_real.csv')
        calculator_results_fourier_real = pd.read_csv(fourier_sphere_data_filepath_real)
        fourier_sphere_data_filepath_imag = os.path.join(output_folderpath, 'SphereData_postFourierTransform_imag.csv')
        calculator_results_fourier_imag = pd.read_csv(fourier_sphere_data_filepath_imag)
        VisualizeSpheres([calculator_results, calculator_results_fourier_real, calculator_results_fourier_imag])
    else:
        VisualizeSphere(calculator_results)



if __name__ == "__main__":
    main()