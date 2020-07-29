# %%
import numpy as np
import pandas as pd
import csv
from math import radians, cos, sin, asin ,sqrt

# %%
def calc_distance(longitude1, latitude1, longitude2, latitude2):

    #convert degrees to radians
    longitude1, latitude1, longitude2, latitude2 = \
        map(radians, [longitude1, latitude1, longitude2, latitude2])

    longitude_difference = longitude2 - longitude1
    latitude1_difference = latitude2 - latitude1

    #haversine formula
    distance = sin(latitude1_difference / 2) ** 2 + \
        cos(latitude1) * cos(latitude2) * sin(longitude_difference / 2) ** 2
    distance = 2 * asin(sqrt(distance)) * 6371

    return distance

# %%
taipei = []
taipei_coordinate = []

with open('taiwan.csv', newline = '') as dataset:
    rows = csv.reader(dataset, delimiter = ' ')
    for row in rows:
        if float(row[0]) > 121.522628 and float(row[0]) < 121.592658 and float(row[1]) > 25.008748 and float(row[1]) < 25.052120:
            taipei.append(row)

# %%
for each_location in taipei:
    temp = [0, 0]
    temp[0] = calc_distance(float(each_location[0]), float(each_location[1]), float(each_location[0]), 25.008774)
    temp[1] = calc_distance(float(each_location[0]), float(each_location[1]), 121.522634, float(each_location[1]))
    taipei_coordinate.append(temp)

# %%
# for each in taipei_coordinate:
#     print(each[0] * 1000, each[1] * 1000)
for each in taipei:
    print(each[0], each[1])