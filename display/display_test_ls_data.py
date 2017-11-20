# coding: utf8

import sys
from os.path import abspath
from os.path import dirname
from os.path import exists
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import style

style.use('seaborn-paper')

def display(filename, strategie):
    """Display data from script run.sh"""

    if not exists(filename):
        raise Exception(filename + " doesn't exist.")

    data = np.genfromtxt(filename, delimiter=",", skip_header=True, dtype=int)

    plt.plot(data[:, 0], data[:, 1],
             'r--',
             label="Mémoire demandée",
             color='black')

    plt.plot(data[:, 0],
             data[:, 2],
             '-',
             label="Taille du tas",
             color='black')

    plt.legend()
    plt.grid(True)
    plt.suptitle("Execution de ls avec la stratégie %s" % strategie, fontsize=20)

    plt.xlabel('Temps', fontsize=18)
    plt.ylabel('Taille du tas', fontsize=18)

    plt.subplots_adjust(top=0.95, bottom=0.05, left=0.05, right=0.95)
    plt.show()


SCRIPT_PATH = abspath(dirname(sys.argv[0]))
OUT_DIR = SCRIPT_PATH + "/out/"

display(OUT_DIR + "test_ls_0112_191716_first_fit.csv", "first fit")
display(OUT_DIR + "test_ls_0112_192332_fit_worst.csv", "fit worst")
display(OUT_DIR + "test_ls_0112_192717_fit_best.csv", "fit best")

