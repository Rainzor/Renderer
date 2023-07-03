import os
import cv2
import numpy as np
import pandas as pd
from pathlib import Path
from enum import Enum
from typing import Dict, List
import matplotlib.pyplot as plt

class SampleMethod(Enum):
    BRDF = 0
    Light = 1
    Mixture = 2
    NEE = 3
    MIS = 4

def plot_rmse(data_points, output_folder):
    # Plot RMSE vs SPP
    plt.figure()
    for sm, data in data_points.items():
        plt.plot(data['spp'], data['rmse'], label=sm.name)
    plt.xlabel('SPP')
    plt.ylabel('RMSE')
    plt.title('RMSE vs SPP for different methods')
    plt.legend()
    plt.savefig(str(output_folder / 'rmse_vs_spp.png'))

def calculate_rmse(image1, image2):
    assert image1.shape == image2.shape, "Images should have the same shape"
    return np.sqrt(np.mean(np.square(np.subtract(image1, image2))))

def rmse_vs_spp(output_folder: Path, spps: List[int], methods: List[SampleMethod]) -> Dict[SampleMethod, Dict[int, float]]:
    rmse_values = {}
    default_image = cv2.imread(str(output_folder / "default.png"))

    for sm in methods:
        rmse_values[sm] = {}
        for spp in spps:
            img_name = output_folder / f"{sm.value}_{spp}.png"
            img = cv2.imread(str(img_name))
            rmse = calculate_rmse(img, default_image)
            rmse_values[sm][spp] = rmse

    return rmse_values

def generate_data_points(rmse_values):
    data_points = {}
    for sm, rmse_dict in rmse_values.items():
        data_points[sm] = {'spp': list(rmse_dict.keys()), 'rmse': list(rmse_dict.values())}

    return data_points

if __name__ == "__main__":
    output_folder = Path(".")
    spps = [4, 8, 16, 32, 64, 128, 256]
    methods = [SampleMethod.BRDF, SampleMethod.Light, SampleMethod.Mixture, SampleMethod.NEE, SampleMethod.MIS]

    rmse_values = rmse_vs_spp(output_folder, spps, methods)
    data_points = generate_data_points(rmse_values)
    plot_rmse(data_points, output_folder)