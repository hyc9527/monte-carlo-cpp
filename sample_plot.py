import json
from matplotlib import pyplot as plt
import pandas as pd

with open("sample.json", "r") as fp:
	data = json.load(fp)

df = pd.DataFrame(data=data).reset_index(drop=True)

for it in data.keys():
	df[it].plot()

plt.show()

