import os
import re
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import ScalarFormatter

DIRECTORY = 'initial3/'
BIN_WIDTH = 25

def extract_first_number(filename):
    match = re.match(r'(\d+)', filename)
    return int(match.group(1)) if match else None

filenames = [f for f in os.listdir(DIRECTORY) if os.path.isfile(os.path.join(DIRECTORY, f))]
numbers = [extract_first_number(name) for name in filenames]
numbers = list(filter(None, numbers))

min_val = min(numbers)
max_val = max(numbers)
bins = np.arange(min_val, max_val + BIN_WIDTH, BIN_WIDTH)

fig, ax = plt.subplots()
ax.hist(numbers, bins=bins, edgecolor='black')
ax.set_title('')
ax.set_xlabel('Довжина маршруту')
ax.set_ylabel('Кількість')
ax.grid(True)

formatter = ScalarFormatter(useMathText=False)
formatter.set_scientific(False)
formatter.set_useOffset(False)
ax.xaxis.set_major_formatter(formatter)

plt.tight_layout()
plt.show()

mean_val = np.mean(numbers)
median_val = np.median(numbers)
count_val = len(numbers)

print(f'Mean: {mean_val}')
print(f'Median: {median_val}')
print(f'Count: {count_val}')
