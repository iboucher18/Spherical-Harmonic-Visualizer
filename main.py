import subprocess as sp
import json
import pandas as pd
import matplotlib.pyplot as plt

class InputData:
    def __init__(self, calculator_path, calculator_output_path):
        self.calc_path = calculator_path
        self.calc_output_path = calculator_output_path



def ReadInputFile(input_filepath) -> InputData:
    # TODO: read in input file with spherical harmonic specifications
    #  worth noting: these specifications will go into the c++ code.
    #  Hopefully we can avoid reading json in c++ by doing this, but could limit what arguments we can set
    return InputData("test_output.csv")


def BuildAndRunCalculator(input_data: InputData):
    sp.run("make -f make_spherical_harmonic_generator") # TODO: not hardcode filename?
    sp.run("./SphericalHarmonicGenerator.x") # TODO: correct name? also need to pass in runtime arguments from input file


def VisualizeOutputs():
    # TODO: given fourier transform data of the specified spherical harmonic, plot the data
    #  also plot spherical harmonic
    #  (somehow, idk - maybe matplotlib, maybe something fancier like seaborn, TBD)
    pass




def main():
    # TODO: create optional argument for specifying input filepath location
    input_file_filepath = "sample_input.json"

    input_data = ReadInputFile(input_file_filepath)

    BuildAndRunCalculator(input_data)

    calculator_results = pd.read_csv(input_data.calc_output_path) # TODO: may not be csv, TBD
    VisualizeOutputs(calculator_results)










if __name__ == "__main__":
    main()