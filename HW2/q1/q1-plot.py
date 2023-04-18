import numpy as np 
import pandas as pd
import matplotlib.pyplot as plt 

# Read data to plot from CSV file
data = pd.read_csv("performance.csv", sep=',', header=0, names=["n", "Elapsed Time", "MFLOPs"], dtype=np.float64)

def plot_data(data):
	# Plot data
	plt.plot(data["n"], data["MFLOPs"], label="MFLOPs")
	plt.xlabel("n")
	plt.ylabel("MFLOPs")
	plt.title(r"Performance of $y = \alpha x + y$")
	plt.legend()
	plt.show()
