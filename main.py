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
    return InputData("test_path.exe", "test_output.csv")


def RunCalculator(input_data: InputData):
    # TODO: use subprocess to call executable at path in input data
    pass


def VisualizeOutputs():
    # TODO: given fourier transform data of the specified spherical harmonic, plot the data
    #  (somehow, idk - maybe matplotlib, maybe something fancier like seaborn, TBD)
    pass




def main():
    # TODO: create optional argument for specifying input filepath location
    input_file_filepath = ""

    input_data = ReadInputFile(input_file_filepath)

    RunCalculator(input_data)

    calculator_results = pd.read_csv(input_data.calc_output_path) # may not be csv, TBD
    VisualizeOutputs(calculator_results)










if __name__ == "__main__":
    main()