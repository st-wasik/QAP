import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from sklearn.metrics.pairwise import cosine_similarity as cs
import statistics

OUT = 'out\\'

ALGORITHM = "algorithm"
INSTANCE = 'instance'
COST = 'cost'
OPTIMAL = 'optimal'
PERMUTATION = 'permutation'


def compare_solutions(df):
    results = {}
    for instance in df[INSTANCE].unique():
        instance_subset = df[df[INSTANCE] == instance]

        algorithm_results = {}
        for algorithm in instance_subset[ALGORITHM].unique():
            algorithm_subset = instance_subset[instance_subset[ALGORITHM] == algorithm]
            permutations = [[int(h) for h in x.split(' ')] for x in algorithm_subset[PERMUTATION].to_list()]

            temp_results = []
            for i in range(len(permutations)):
                permutation_1 = np.asarray(permutations[i]).reshape(1, -1)
                for j in range(len(permutations)):
                    if i != j:
                        permutation_2 = np.asarray(permutations[j]).reshape(1, -1)
                        temp_results.append(cs(permutation_1, permutation_2))

            algorithm_results[algorithm] = [x[0][0] for x in temp_results]
            # print(temp_results)
        results[instance] = algorithm_results

    results_df = pd.DataFrame.from_dict(results)
    results_df.to_csv('solution_similarities.csv')

    plot(results, 'Similarity [%]', 'similarities.png')
    # plot_best(results, 'Similarity [%]', 'similarity_to_optimal_best.png')


def compare_to_optimum(df, optimum_dic):
    results = {}
    for instance in df[INSTANCE].unique():
        instance_subset = df[df[INSTANCE] == instance]
        optimal_value = optimum_dic[instance]

        algorithm_results = {}
        for algorithm in instance_subset[ALGORITHM].unique():
            algorithm_subset = instance_subset[instance_subset[ALGORITHM] == algorithm]
            permutations = [[int(h) for h in x.split(' ')] for x in algorithm_subset[PERMUTATION].to_list()]

            temp_results = []
            for permutation in permutations:
                temp_results.append(cs(np.asarray(permutation).reshape(1, -1), np.asarray(optimal_value).reshape(1, -1)))

            algorithm_results[algorithm] = [x[0][0] for x in temp_results]
        results[instance] = algorithm_results

    pd.DataFrame.from_dict(results).to_csv('similarities_to_optimal.csv')
    plot(results, 'Similarity [%]', 'similarity_to_optimal.png')
    plot_best(results, 'Similarity [%]', 'similarity_to_optimal_best.png')


def plot(results, y_label, name):
    x = np.arange(len(results.keys()))
    width = 0.35
    size = (4, 7)

    fig, ax = plt.subplots(figsize=size)
    ax.bar(x - width/2,
           [statistics.mean(item[1]['STEEPEST']) * 100 for item in results.items()],
           yerr=[statistics.stdev(item[1]['STEEPEST']) * 100 for item in results.items()],
           width=width,
           label='Steepest')
    ax.bar(x + width/2,
           [statistics.mean(item[1]['GREEDY']) * 100 for item in results.items()],
           yerr=[statistics.stdev(item[1]['GREEDY']) * 100 for item in results.items()],
           width=width,
           label='Greedy')

    ax.set_ylabel(y_label)
    ax.set_yscale("log")
    ax.set_xticks(x)
    ax.set_xticklabels(results.keys())
    ax.legend()
    ax.grid(True)
    # plt.setp(ax.get_xticklabels(), rotation=30, horizontalalignment='right')
    fig.tight_layout()
    fig.savefig(OUT + name)
    plt.close(fig)


def plot_best(results, y_label, name):
    x = np.arange(len(results.keys()))
    width = 0.35
    size = (4, 7)

    fig, ax = plt.subplots(figsize=size)
    ax.bar(x - width / 2,
           [max(item[1]['STEEPEST']) * 100 for item in results.items()],
           width=width,
           label='Steepest')
    ax.bar(x + width / 2,
           [max(item[1]['GREEDY']) * 100 for item in results.items()],
           width=width,
           label='Greedy')

    ax.set_ylabel(y_label)
    ax.set_yscale("log")
    ax.set_xticks(x)
    ax.set_xticklabels(results.keys())
    ax.legend()
    ax.grid(True)
    # ax.set_ylim(60, 90)
    # plt.setp(ax.get_xticklabels(), rotation=30, horizontalalignment='right')
    fig.tight_layout()
    fig.savefig(OUT + name)
    plt.close(fig)

if __name__ == '__main__':
    data = pd.read_csv('point_5.csv')
    compare_solutions(data)

    optimal = {
        'bur26f': [7, 2, 13, 17, 16, 26, 23, 1, 10, 15, 19, 20, 18, 12, 14, 25, 21, 5, 9, 3, 6, 24, 22, 4, 11, 8],
        'tai64c': [1, 3, 5, 15, 17, 20, 30, 35, 40, 45, 49, 51, 55, 2, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 18, 19,
                   21, 22, 23, 24, 25, 26, 27, 28, 29, 31, 32, 33, 34, 36, 37, 38, 39, 41, 42, 43, 44, 46, 47, 48, 50,
                   52, 53, 54, 56, 57, 58, 59, 60, 61, 62, 63, 64]}
    compare_to_optimum(data, optimal)
    # test = pd.read_csv('test.csv')
    # print(test.columns)
