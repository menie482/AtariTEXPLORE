import matplotlib.pyplot as plt
import numpy as np
import glob
import os
import re

# find files
files = glob.glob("*.err*")

# parse data
trials = []
for path in files:
  scores = []
  trials.append(scores)
  f = open(path)

  trial = 0
  for line in f:
    match = re.search('(\d+)\t(\d+)\t(\d+)', line)
    if match:
      score = match.group(1)
      steps = match.group(2)
      invld = match.group(3)
      scores.append(score)
      trial = trial + 1
  f.close()

episodesByTrial = []
for i in range(len(trials)):
  episodesByTrial.append(len(trials[i]))
print episodesByTrial

# analyze data
maxEpisodeReached = max(episodesByTrial)
totals = []
for i in range(maxEpisodeReached):
  total = 0
  for j in range(len(trials)):
    trial = trials[j]
    if episodesByTrial[j] > i:
      total = total + int(trial[i])
  totals.append(total)

averages = []
for total in totals:
  averages.append(total/len(trials))
print averages

# draw graph
t = np.array(range(maxEpisodeReached))
avgs = np.array(averages)

plt.plot(t, avgs)
plt.ylabel('Average Reward')
plt.xlabel('Episode #')
plt.axes([0, maxEpisodeReached, 0, 1000])
#plt.show()
plt.savefig('experimentgraph.png')
