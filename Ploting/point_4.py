import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

ALGORITHM = "algorithm"
INSTANCE = 'instance'
COST = 'cost'
OPTIMAL = 'optimal'

OUT = 'out\\'


def plot(df, optimal):
    for instance in df.instance.unique():
        instance_subset = df[df.instance == instance]

        greedy = instance_subset[instance_subset[ALGORITHM] == 'GREEDY']
        steepest = instance_subset[instance_subset[ALGORITHM] == 'STEEPEST']

        greedy_results = get_data(greedy, optimal)
        steepest_results = get_data(steepest, optimal)
        x = np.arange(1, 501)

        try:
            fig, ax = plt.subplots(figsize=(14, 4))
            ax.plot(x, greedy_results[0], 'ro', label='Greedy best', markersize=1)
            ax.plot(x, greedy_results[1], label='Greedy mean')

            ax.plot(x, steepest_results[0], 'bo', label='Steepest best', markersize=1)
            ax.plot(x, steepest_results[1], label='Steepest mean')

            ax.set_ylabel('Błąd względny')
            ax.set_xlabel('n')
            ax.legend()
            fig.tight_layout()
            fig.savefig(OUT + 'point_4_' + instance)
            plt.close(fig)
        except:
            pass


def get_data(df, optimal):
    costs = df[COST].to_list()
    optimal_value = optimal.loc[df.iloc[0, 0], OPTIMAL]
    quality = [(cost - optimal_value) / optimal_value for cost in costs]

    best, mean = [], []
    for i in range(1, 501):
        best.append(min(quality[:i]))
        mean.append(sum(quality[:i]) / i)

    return best, mean


if __name__ == '__main__':
    data = pd.read_csv('point_4.csv')
    optimal_df = pd.DataFrame({'optimal': [578, 51765268, 3782044, 168, 240516, 1855928, 66256, 360630, 498896643]},
                           index=['nug12', 'tai15b', 'bur26f', 'esc32b', 'tho40', 'tai64c', 'sko72', 'lipa90a',
                                  'tai150b'])
    plot(data, optimal_df)
