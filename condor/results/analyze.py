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
    match = re.search('(\-?\d+)\t(\d+)\t(\d+)', line)
    if match:
      score = match.group(1)
      steps = match.group(2)
      invld = match.group(3)
      scores.append(score)
      trial = trial + 1
  f.close()

numEpisodesPerTrial = []
for i in range(len(trials)):
  numEpisodesPerTrial.append(len(trials[i]))
print 'NUMBER OF EPISODES PER TRIAL:'
print numEpisodesPerTrial

# analyze data
maxEpisodeReached = max(numEpisodesPerTrial)
totals = []
for i in range(maxEpisodeReached):
  total = 0
  numEpisodesThatReachedThisTrial = 0
  for j in range(len(trials)):
    trial = trials[j]
    if numEpisodesPerTrial[j] > i:
      total = total + int(trial[i])
      numEpisodesThatReachedThisTrial = numEpisodesThatReachedThisTrial + 1
  totals.append((total, numEpisodesThatReachedThisTrial))

print 'PER EPISODE: (TOTAL REWARD, NUM TRIALS THAT REACHED THIS EPISODE)'
print totals

averages = []
for (total, epCount) in totals:
  averages.append(float(total)/epCount)
print 'AVERAGE EPISODE REWARD ACROSS ALL TRIALS:'
print averages

# draw graph
t = np.array(range(maxEpisodeReached))
avgs = np.array(averages)

plt.plot(t, avgs)
plt.ylabel('Average Reward')
plt.xlabel('Episode #')
plt.axes([0, maxEpisodeReached, 0, 1000])
plt.show()
plt.savefig('experimentgraph.png')
