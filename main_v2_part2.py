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

    plt.show()




def main():
    output_folderpath = sys.argv[1]
    run_fourier = sys.argv[2]

    if run_fourier:
        # TODO: update to have all 3 plots on 1 plot
        fourier_sphere_data_filepath = os.path.join(output_folderpath, 'SphereData_postFourierTransform.csv')
        calculator_results_fourier = pd.read_csv(fourier_sphere_data_filepath)
        VisualizeSphere(calculator_results_fourier)
    else:
        sphere_data_filepath = os.path.join(output_folderpath, 'SphereData.csv')
        calculator_results = pd.read_csv(sphere_data_filepath)
        VisualizeSphere(calculator_results)



if __name__ == "__main__":
    main()