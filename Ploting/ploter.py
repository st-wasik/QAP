import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

ALGORITHM = "algorithm"
CHECKED_SOLUTIONS = 'checked_solutions'
STEPS = 'steps'
TIME_MS = 'time_msec'
COST = 'cost'
OPTIMAL = 'optimal'
RANDOM_COST = 'initial_cost'

OUT = 'out\\'


def plot_point_2(df, optimal_solution):
    steps, score, checked, time = get_results(df, optimal_solution)

    plot_full(score, 'MSE', 'mean_score.png', OUT)
    plot_full(time, 'Mean time [ms]', "mean_time.png", OUT)
    plot_checked_solutions(checked, OUT)
    plot_steps(steps, OUT)


def get_results(df, optimal_solution):
    steps_results, score_results, graded_solutions_results, time_results = {}, {}, {}, {}

    for instance in df.instance.unique():
        instance_subset = df[df.instance == instance]

        steepest = instance_subset[instance_subset[ALGORITHM] == "STEEPEST"]
        greedy = instance_subset[instance_subset[ALGORITHM] == "GREEDY"]
        steps_results[instance] = (greedy[STEPS].mean(), greedy[STEPS].std(), steepest[STEPS].mean(), steepest[STEPS].std())

        plot_random_vs_best_solution(greedy, steepest, instance, OUT)

        algorithm_graded_solutions, algorithm_time, algorithm_score = {}, {}, {}
        # for algorithm in df[ALGORITHM].unique():
        #     algorithm_subset = instance_subset[instance_subset[ALGORITHM] == algorithm]
        #
        #     algorithm_graded_solutions[algorithm] = (algorithm_subset[CHECKED_SOLUTIONS].mean(), algorithm_subset[CHECKED_SOLUTIONS].std())
        #     algorithm_time[algorithm] = (algorithm_subset[TIME_MS].mean(), algorithm_subset[TIME_MS].std())
        #
        #     costs = algorithm_subset[COST].to_list()
        #     algorithm_subset['MSE'] = [(optimal_solution.loc[instance, OPTIMAL] - result) ** 2 for result in costs]
        #     algorithm_score[algorithm] = (algorithm_subset['MSE'].mean(), algorithm_subset['MSE'].std())
        #
        # graded_solutions_results[instance] = algorithm_graded_solutions
        # time_results[instance] = algorithm_time
        # score_results[instance] = algorithm_score

    return steps_results, score_results, graded_solutions_results, time_results


def get_efficiency(score, time):
    result = {}


def plot_random_vs_best_solution(greedy, steepest, instance, out_path):
    n_samples = 200
    dot_size = 8

    greedy_random = greedy[RANDOM_COST].to_list()[:n_samples]
    greedy_best = greedy[COST].to_list()[:n_samples]
    greedy_corrcoef = np.corrcoef(greedy_random, greedy_best)

    steepest_random = steepest[RANDOM_COST].to_list()[:n_samples]
    steepest_best = steepest[COST].to_list()[:n_samples]
    steepest_corrcoef = np.corrcoef(steepest_random, steepest_best)

    fig, ax = plt.subplots()
    ax.scatter(greedy_random,
               greedy_best,
               s=dot_size,
               label='Greedy, p = ' + str(greedy_corrcoef[0][1])[:6])
    ax.plot(np.unique(greedy_random), np.poly1d(np.polyfit(greedy_random, greedy_best, 1))(np.unique(greedy_random)), color='green')
    ax.scatter(steepest_random,
               steepest_best,
               s=dot_size,
               label='Steepest, p = ' + str(steepest_corrcoef[0][1])[:6])
    ax.plot(np.unique(steepest_random), np.poly1d(np.polyfit(steepest_random, steepest_best, 1))(np.unique(steepest_random)),color='red')

    ax.legend()
    ax.grid(True)
    ax.set_ylabel('Final cost')
    ax.set_xlabel('Random start cost')
    fig.tight_layout()
    fig.savefig(out_path + instance + "_random_to_best_solution.png")
    plt.close()


def plot_full(results, y_label, name, out_path):
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
    fig.savefig(out_path + name)


def plot_checked_solutions(results, out_path):
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
    fig.savefig(out_path + "checked_solutions.png")


def plot_steps(results, out_path):
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
    fig.savefig(out_path + "mean_steps.png")


def point_4_plot(df):
    for instance in df.instance.unique():
        instance_subset = df[df.instance == instance]

        greedy = instance_subset[instance_subset[ALGORITHM] == 'GREEDY']
        steepest = instance_subset[instance_subset[ALGORITHM] == 'STEEPEST']

        greedy_results = point_4_get_data(greedy)
        steepest_results = point_4_get_data(steepest)
        x = np.arange(1, 501)

        fig, ax = plt.subplots(figsize=(14, 4))
        ax.plot(x, greedy_results[0], 'ro', labels='Greedy best')
        ax.plot(x, greedy_results[1], labels='Greedy mean')

        ax.plot(x, steepest_results[0], 'bo', labels='Steepest best')
        ax.plot(x, steepest_results[1], labels='Steepest mean')

        ax.set_ylabel('Score')
        ax.set_xlabel('')




def point_4_get_data(df):
    cost = df[COST].to_list()
    best, mean = [], []
    for i in range(1, 501):
        best.append(min(cost[:i]))
        mean.append(sum(cost[:i]) / i)

    return best, mean


if __name__ == '__main__':

    dummy_optimal = pd.DataFrame({'optimal': [100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000, 11000000, 12000000, 12323322]},
                                 index=['bur26d', 'lipa30b', 'esc32a', 'lipa40a', 'wil50', 'lipa60a', 'sko72', 'sko81', 'lipa90b', 'tai100b', 'sko100d', 'tai100a', 'tai150b'])

    data = pd.read_csv('cost_to_init_2.csv')
    print(len(data.instance.unique()))
    plot_point_2(data, dummy_optimal)