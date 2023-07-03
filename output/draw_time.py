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


def generate_data_points(rmse_values, timing_results):
    data_points = {}
    for sm, rmse_dict in rmse_values.items():
        data_points[sm] = {'spp': list(rmse_dict.keys()), 'rmse': list(rmse_dict.values())}

    for sm in SampleMethod:
        data_points[sm]['time'] = timing_results[timing_results['SampleMethod'] == sm.value]['Time'].tolist()

    return data_points

def plot_rmse(data_points, output_folder):
    # # Plot RMSE vs SPP
    # plt.figure()
    # for sm, data in data_points.items():
    #     plt.plot(data['spp'], data['rmse'], label=sm.name)
    # plt.xlabel('SPP')
    # plt.ylabel('RMSE')
    # plt.title('RMSE vs SPP for different methods')
    # plt.legend()
    # plt.savefig(str(output_folder/'rmse_vs_spp.png'))

    # Plot RMSE vs Time
    plt.figure()
    for sm, data in data_points.items():
        plt.plot(data['time'], data['rmse'], label=sm.name)
    plt.xlabel('Time')
    plt.ylabel('RMSE')
    plt.title('RMSE vs Time for different methods')
    plt.legend()
    plt.savefig(str(output_folder/'rmse_vs_time.png'))

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


def rmse_vs_time(timing_results_path: Path, rmse_values: Dict[SampleMethod, Dict[int, float]]):
    timing_results = pd.read_csv(timing_results_path)
    timing_results["RMSE"] = 0.0

    for index, row in timing_results.iterrows():
        sm = SampleMethod(int(row["SampleMethod"]))
        spp = int(row["SPP"])
        timing_results.at[index, "RMSE"] = rmse_values[sm][spp]

    timing_results.to_csv(timing_results_path, index=False)

def plot_time_vs_spp(data_points, output_folder):
    mis_data = data_points[SampleMethod.MIS]
    plt.figure()
    plt.plot(mis_data['spp'], mis_data['time'])
    plt.xlabel('SPP')
    plt.ylabel('Time (s)')
    plt.title('Time vs SPP for MIS algorithm')
    plt.savefig(str(output_folder/'mis_time_vs_spp.png'))

if __name__ == "__main__":
    output_folder = Path(".")
    spps = [2, 4, 8, 16, 32, 64, 128]
    methods = [SampleMethod.BRDF, SampleMethod.Light, SampleMethod.Mixture, SampleMethod.NEE, SampleMethod.MIS]

    rmse_values = rmse_vs_spp(output_folder, spps, methods)
    timing_results_path = output_folder / "timing_results.csv"
    rmse_vs_time(timing_results_path, rmse_values)

    timing_results = pd.read_csv(timing_results_path)
    data_points = generate_data_points(rmse_values, timing_results)
    plot_rmse(data_points,output_folder)
    plot_time_vs_spp(data_points, output_folder)