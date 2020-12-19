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


def get_results(df):
    for instance in df.instance.unique():
        optimal_df = pd.DataFrame({'optimal': [578, 51765268, 3782044, 168, 240516, 1855928, 66256, 360630, 498896643]},
                               index=['nug12', 'tai15b', 'bur26f', 'esc32b', 'tho40', 'tai64c', 'sko72', 'lipa90a',
                                      'tai150b'])
        optimal = optimal_df.loc[instance, 'optimal']
        instance_subset = df[df.instance == instance]

        steepest = instance_subset[instance_subset[ALGORITHM] == "STEEPEST"]
        greedy = instance_subset[instance_subset[ALGORITHM] == "GREEDY"]

        n_samples = 300
        dot_size = 8

        greedy_random = (greedy[RANDOM_COST].to_list()[:n_samples] - optimal) / optimal
        greedy_best = (greedy[COST].to_list()[:n_samples] - optimal) / optimal
        greedy_corrcoef = np.corrcoef(greedy_random, greedy_best)

        steepest_random = (steepest[RANDOM_COST].to_list()[:n_samples] - optimal) / optimal
        steepest_best = (steepest[COST].to_list()[:n_samples] - optimal) / optimal
        steepest_corrcoef = np.corrcoef(steepest_random, steepest_best)

        try:
            fig, ax = plt.subplots()
            ax.scatter(greedy_random,
                       greedy_best,
                       s=dot_size,
                       label='Greedy, p = ' + str(greedy_corrcoef[0][1])[:6])
            ax.plot(np.unique(greedy_random),
                    np.poly1d(np.polyfit(greedy_random, greedy_best, 1))(np.unique(greedy_random)), color='green')
            ax.scatter(steepest_random,
                       steepest_best,
                       s=dot_size,
                       label='Steepest, p = ' + str(steepest_corrcoef[0][1])[:6])
            ax.plot(np.unique(steepest_random),
                    np.poly1d(np.polyfit(steepest_random, steepest_best, 1))(np.unique(steepest_random)), color='red')

            ax.legend()
            ax.grid(True)
            plt.title(instance)
            ax.set_ylabel('Final quality')
            ax.set_xlabel('Random start quality')
            fig.tight_layout()
            fig.savefig(OUT + instance + "_random_to_best_solution.png")
            plt.close()
        except:
            pass


if __name__ == '__main__':
    data = pd.read_csv('point_3.csv')
    get_results(data)