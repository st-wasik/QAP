import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import statistics
import math

ALGORITHM = "algorithm"
CHECKED_SOLUTIONS = 'checked_solutions'
STEPS = 'steps'
TIME_MS = 'time_msec'
COST = 'cost'
OPTIMAL = 'optimal'
RANDOM_COST = 'initial_cost'

OUT = 'out\\'
FIG_SIZE = (14, 4)
SAMPLES = 10


def point_2(df):
    steps, score, checked, time = get_results(df)

    plot(time, 'Mean time [ms]', "mean_time.png", OUT)
    plot(checked, 'Mean solutions checked', 'checked_solutions.png', OUT, full=False)
    plot_steps(steps, OUT)


def get_results(df):
    steps_results, score_results, checked_solutions_results, time_results = {}, {}, {}, {}

    for instance in df.instance.unique():
        instance_subset = df[df.instance == instance]

        steepest = instance_subset[instance_subset[ALGORITHM] == "STEEPEST"]
        greedy = instance_subset[instance_subset[ALGORITHM] == "GREEDY"]
        steps_results[instance] = (
            greedy[STEPS].head(SAMPLES).mean(),
            greedy[STEPS].head(SAMPLES).std(),
            steepest[STEPS].head(SAMPLES).mean(),
            steepest[STEPS].head(SAMPLES).std())

        algorithm_checked_solutions, algorithm_time, algorithm_score = {}, {}, {}
        for algorithm in df[ALGORITHM].unique():
            algorithm_subset = instance_subset[instance_subset[ALGORITHM] == algorithm]

            algorithm_checked_solutions[algorithm] = (algorithm_subset[CHECKED_SOLUTIONS].head(SAMPLES).mean(), algorithm_subset[CHECKED_SOLUTIONS].head(SAMPLES).std())
            algorithm_time[algorithm] = (algorithm_subset[TIME_MS].head(SAMPLES).mean(), algorithm_subset[TIME_MS].head(SAMPLES).std())

        checked_solutions_results[instance] = algorithm_checked_solutions
        time_results[instance] = algorithm_time
        score_results[instance] = algorithm_score

    return steps_results, score_results, checked_solutions_results, time_results


def plot(results, y_label, name, out_path, full=True, size=(14, 4), log=True):
    x = np.arange(len(results.keys()))

    fig, ax = plt.subplots(figsize=size)
    ax.errorbar(x,
           [item[1]['STEEPEST'][0] for item in results.items()],
           yerr=[item[1]['STEEPEST'][1] for item in results.items()],
           label='Steepest',
            fmt='-o')
    ax.errorbar(x,
           [item[1]['GREEDY'][0] for item in results.items()],
           yerr=[item[1]['GREEDY'][1] for item in results.items()],
           label='Greedy',
                fmt='-o')
    ax.errorbar(x,
           [item[1]['RANDOM'][0] for item in results.items()],
           yerr=[item[1]['RANDOM'][1] for item in results.items()],
           label='Random', fmt='-o')
    ax.errorbar(x,
           [item[1]['RANDOMWALK'][0] for item in results.items()],
           yerr=[item[1]['RANDOMWALK'][1] for item in results.items()],
           label='Randomwalk', fmt='-o')
    if full:
        ax.errorbar(x,
               [item[1]['HEURISTICS'][0] for item in results.items()],
               yerr=[item[1]['HEURISTICS'][1] for item in results.items()],
               label='Heuristics', fmt='-o')

    ax.set_ylabel(y_label)
    if log:
        ax.set_yscale("log")
    ax.set_xticks(x)
    ax.grid(True)
    ax.set_xticklabels(results.keys())
    ax.legend()

    fig.tight_layout()
    fig.savefig(out_path + name)


def plot_steps(results, out_path, size = (14, 4), ):
    x = np.arange(len(results.keys()))

    fig, ax = plt.subplots(figsize=size)
    ax.errorbar(x,
           [item[1][0] for item in results.items()],
           yerr=[item[1][1] for item in results.items()],
           label='Greedy', fmt='-o')
    ax.errorbar(x,
           [item[1][2] for item in results.items()],
           yerr=[item[1][3] for item in results.items()],
           label='Steepest', fmt='-o')
    ax.set_ylabel('Mean steps')
    ax.set_yscale("log")
    ax.set_xticks(x)
    ax.set_xticklabels(results.keys())
    ax.legend()
    ax.grid(True)
    plt.setp(ax.get_xticklabels(), rotation=30, horizontalalignment='right')
    fig.tight_layout()
    fig.savefig(out_path + "mean_steps.png")


def get_efficiency(df, optimal_solution):
    MSE_score, best_opt, app_err, best = {}, {}, {}, {}
    for instance in df.instance.unique():
        instance_subset = df[df.instance == instance]

        algorithm_MSE, algorithm_best_opt, algorithm_app_err, algorithm_best = {}, {}, {}, {}
        for algorithm in df[ALGORITHM].unique():
            algorithm_subset = instance_subset[instance_subset[ALGORITHM] == algorithm]

            costs = algorithm_subset[COST].to_list()[:SAMPLES]

            # MSE
            # mse = [(optimal_solution.loc[instance, OPTIMAL] - result) ** 2 for result in costs]
            # algorithm_MSE[algorithm] = (statistics.mean(mse), statistics.stdev(mse))

            # best / opt
            # best_opt_val = [result / optimal_solution.loc[instance, OPTIMAL] for result in costs]
            # algorithm_best_opt[algorithm] = (statistics.mean(best_opt_val), statistics.stdev(best_opt_val))

            # (best - opt) / opt
            approx = [((result - optimal_solution.loc[instance, OPTIMAL]) / optimal_solution.loc[instance, OPTIMAL]) for result in costs]
            algorithm_app_err[algorithm] = (statistics.mean(approx), statistics.stdev(approx))
            algorithm_best[algorithm] = min(approx)

        MSE_score[instance] = algorithm_MSE
        best_opt[instance] = algorithm_best_opt
        app_err[instance] = algorithm_app_err
        best[instance] = algorithm_best

    # plot(MSE_score, 'MSE', 'MSE_score.png', OUT)
    # plot(best_opt, 'Best / Opt', 'BestOpt.png', OUT)
    plot(app_err, 'Błąd względny', 'approx_error.png', OUT, log=True)
    plot_without_std(best, 'Najlepszy przypadek', 'best.png', OUT)

    return app_err


def quality_in_time(df, eff_dict):
    results = {}
    for instance in df.instance.unique():
        instance_subset = df[df.instance == instance]

        algorithm_results = {}
        for algorithm in df[ALGORITHM].unique():
            algorithm_subset = instance_subset[instance_subset[ALGORITHM] == algorithm]

            time = algorithm_subset[TIME_MS].head(SAMPLES).to_list()
            eff = eff_dict[instance][algorithm][0]

            algorithm_results[algorithm] = (eff / math.log(statistics.mean(time), 10)) ** -1
        results[instance] = algorithm_results

    plot_without_std(results, 'Efficiency', 'efficiency.png', OUT)


def plot_without_std(results, y_label, name, out_path, full=True, size=(14, 4), log=True):
    x = np.arange(len(results.keys()))
    fig, ax = plt.subplots(figsize=FIG_SIZE)
    ax.plot(x,
           [item[1]['STEEPEST'] for item in results.items()],
            '-o',
           label='Steepest')
    ax.plot(x,
           [item[1]['GREEDY'] for item in results.items()],
            '-o',
           label='Greedy')
    ax.plot(x,
           [item[1]['RANDOM'] for item in results.items()],
            '-o',
           label='Random')
    ax.plot(x,
           [item[1]['RANDOMWALK'] for item in results.items()],
            '-o',
           label='Randomwalk')
    ax.plot(x,
           [item[1]['HEURISTICS'] for item in results.items()],
            '-o',
           label='Heuristics')

    ax.set_ylabel(y_label)
    if log:
        ax.set_yscale("log")
    ax.grid(True)
    ax.set_xticks(x)
    ax.set_xticklabels(results.keys())
    ax.legend()

    fig.tight_layout()
    fig.savefig(OUT + name)


if __name__ == '__main__':
    data = pd.read_csv('point_2.csv')
    data[TIME_MS] = data[TIME_MS].replace(0, 1.2)

    point_2(data)

    optimal = pd.DataFrame({'optimal': [578, 51765268, 3782044, 168, 240516, 1855928, 66256, 360630, 498896643]},
                           index=['nug12', 'tai15b', 'bur26f', 'esc32b', 'tho40', 'tai64c', 'sko72', 'lipa90a', 'tai150b'])
    efficiency = get_efficiency(data, optimal)

    quality_in_time(data, efficiency)
