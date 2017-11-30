#!/usr/bin/env python3
"""Generate a list of edges for EU4 map.

Must be called in a directory containing definition.csv and provinces.bmp
Writes output to not-a-toy-graph.txt"""

import csv
import numpy as np
import datetime
import time
from PIL import Image

VERBOSE = True


def verbose(string):
    if VERBOSE:
        print(string)


def get_rgb_to_id_map(filename):
    with open(filename, newline='') as f:
        reader = csv.reader(f)

        # id, red, green, blue, name, is_inhabitable
        # In definitions.csv, the columns are province, red, green, blue, x, x.
        # `province` is an integer id for a province, and the first x is the
        # province name. The last x indicates whether the province is "inhabitable."
        # The problem is, in the file, values in this column are either 'x' or
        # empty. Python gets angry when we try to unpack a value that isn't there.
        inhabitable_provinces = (row for row in reader if len(row) == 6)
        next(reader)  # throw away titles
        return {(int(r), int(g), int(b)): int(id)
                for id, r, g, b, _, _ in inhabitable_provinces}


def get_edges(img, m):
    width, height = img.size
    arr = np.asarray(img)
    edges = set()

    def neq(a, b):
        return any(x != y for x in a for y in b)

    for x in range(1, height - 1):
        for y in range(1, width - 1):
            k = arr[x, y]
            k_up = arr[x, y - 1]
            k_down = arr[x, y + 1]
            k_left = arr[x - 1, y]
            k_right = arr[x + 1, y]
            neighbors = [k_up, k_down, k_left, k_right]
            for n in neighbors:
                if neq(k, n):
                    k_key = tuple(k)
                    n_key = tuple(n)
                    try:
                        a = m[k_key]
                    # Uninhabitable land
                    except KeyError:
                        pass
                    try:
                        b = m[n_key]
                    # Uninhabitable land
                    except KeyError:
                        pass
                    if a < b:
                        edges.add((a, b))
    return edges


if __name__ == '__main__':
    verbose('Generating RGB lookup...')
    rgb_to_id_map = get_rgb_to_id_map('definition.csv')

    verbose('Opening bmp file...')
    img = Image.open('provinces.bmp')

    verbose('Finding neighboring provinces...')
    edges = get_edges(img, rgb_to_id_map)

    verbose('Preparing to write one big string...')
    s = '\n'.join(f'{u} {v}' for u, v in edges)

    verbose('Writing...')
    with open('not-a-toy-graph.txt', 'w') as f:
        f.write(s)
