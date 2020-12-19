import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

ALGORITHM = " algorithm"
CHECKED_SOLUTIONS = ' checked_solutions'
STEPS = ' steps'
TIME_MS = ' time_msec'
COST = ' cost'
OPTIMAL = 'optimal'


def mean_steps(df):
    results = {}
    for instance in df.instance.unique():
        subset = df[df.instance == instance]

        steepest = subset[subset[ALGORITHM] == "STEEPEST"]
        greedy = subset[subset[ALGORITHM] == "GREEDY"]

        results[instance] = (greedy[STEPS].mean(), greedy[STEPS].std(), steepest[STEPS].mean(), steepest[STEPS].std())

    x = np.arange(len(results.keys()))
    width = 0.35
    size = (14, 4)

    fig, ax = plt.subplots(figsize=size)
    ax.bar(x - width / 2,
           [item[1][0] for item in results.items()],
           yerr=[item[1][1] for item in results.items()],
           width=width,
           label='Greedy')
    ax.bar(x + width / 2,
           [item[1][2] for item in results.items()],
           yerr=[item[1][3] for item in results.items()],
           width=width,
           label='Steepest')
    ax.set_ylabel('Mean steps')
    ax.set_yscale("log")
    ax.set_xticks(x)
    ax.set_xticklabels(results.keys())
    ax.legend()
    plt.setp(ax.get_xticklabels(), rotation=30, horizontalalignment='right')
    fig.tight_layout()
    fig.savefig(OUTPUT_PATH + "mean_steps.png")


def graded_solutions(df):
    results = {}
    for instance in df.instance.unique():
        instance_subset = df[df.instance == instance]

        algorithm_dict = {}
        for algorithm in df[ALGORITHM].unique():
            algorithm_subset = instance_subset[instance_subset[ALGORITHM] == algorithm]
            algorithm_dict[algorithm] = (
                algorithm_subset[CHECKED_SOLUTIONS].mean(), algorithm_subset[CHECKED_SOLUTIONS].std())

        results[instance] = algorithm_dict

    x = np.arange(len(results.keys()))
    width = 0.20
    size = (14, 4)

    fig, ax = plt.subplots(figsize=size)
    ax.bar(x - 2 * width,
           [item[1]['STEEPEST'][0] for item in results.items()],
           yerr=[item[1]['STEEPEST'][1] for item in results.items()],
           width=width,
           label='Steepest')
    ax.bar(x - width,
           [item[1]['GREEDY'][0] for item in results.items()],
           yerr=[item[1]['GREEDY'][1] for item in results.items()],
           width=width,
           label='Greedy')
    ax.bar(x,
           [item[1]['RANDOM'][0] for item in results.items()],
           yerr=[item[1]['RANDOM'][1] for item in results.items()],
           width=width,
           label='Random')
    ax.bar(x + width,
           [item[1]['RANDOMWALK'][0] for item in results.items()],
           yerr=[item[1]['RANDOMWALK'][1] for item in results.items()],
           width=width,
           label='Randomwalk')

    ax.set_ylabel('Mean solutions checked')
    ax.set_yscale("log")
    ax.set_xticks(x)
    ax.set_xticklabels(results.keys())
    ax.legend()

    fig.tight_layout()
    fig.savefig(OUTPUT_PATH + "checked_solutions.png")


def mean_time(df):
    results = {}
    for instance in df.instance.unique():
        instance_subset = df[df.instance == instance]

        algorithm_dict = {}
        for algorithm in df[ALGORITHM].unique():
            algorithm_subset = instance_subset[instance_subset[ALGORITHM] == algorithm]
            algorithm_dict[algorithm] = (algorithm_subset[TIME_MS].mean(), algorithm_subset[TIME_MS].std())

        results[instance] = algorithm_dict

    print_full(results, 'Mean time [ms]', "mean_time.png")


def mean_score(df, ground_truth):
    results = {}
    for instance in df.instance.unique():
        instance_subset = df[df.instance == instance]

        algorithm_dict = {}
        for algorithm in df[ALGORITHM].unique():
            algorithm_subset = instance_subset[instance_subset[ALGORITHM] == algorithm]

            costs = algorithm_subset[COST].to_list()
            algorithm_subset['MSE'] = [(ground_truth.loc[algorithm, OPTIMAL] - result) ** 2 for result in costs]
            algorithm_dict[algorithm] = (algorithm_subset['MSE'].mean(), algorithm_subset['MSE'].std())

        results[instance] = algorithm_dict

    print_full(results, 'MSE', 'mean_score')


def print_full(results, y_label, name):
    x = np.arange(len(results.keys()))
    width = 0.18
    size = (14, 4)

    fig, ax = plt.subplots(figsize=size)
    ax.bar(x - 2 * width,
           [item[1]['STEEPEST'][0] for item in results.items()],
           yerr=[item[1]['STEEPEST'][1] for item in results.items()],
           width=width,
           label='Steepest')
    ax.bar(x - width,
           [item[1]['GREEDY'][0] for item in results.items()],
           yerr=[item[1]['GREEDY'][1] for item in results.items()],
           width=width,
           label='Greedy')
    ax.bar(x,
           [item[1]['RANDOM'][0] for item in results.items()],
           yerr=[item[1]['RANDOM'][1] for item in results.items()],
           width=width,
           label='Random')
    ax.bar(x + width,
           [item[1]['RANDOMWALK'][0] for item in results.items()],
           yerr=[item[1]['RANDOMWALK'][1] for item in results.items()],
           width=width,
           label='Randomwalk')
    ax.bar(x + 2 * width,
           [item[1]['HEURISTICS'][0] for item in results.items()],
           yerr=[item[1]['HEURISTICS'][1] for item in results.items()],
           width=width,
           label='Heuristics')

    ax.set_ylabel(y_label)
    ax.set_yscale("log")
    ax.set_xticks(x)
    ax.set_xticklabels(results.keys())
    ax.legend()

    fig.tight_layout()
    fig.savefig(OUTPUT_PATH + name)


if __name__ == '__main__':
    INPUT_PATH = sys.argv[0]
    OUTPUT_PATH = sys.argv[1]

    data = pd.read_csv("first_test.csv")

    mean_steps(data)
    graded_solutions(data)
    mean_time(data)
