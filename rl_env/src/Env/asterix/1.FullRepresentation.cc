/** \file Arcade.cc
    Implements the Fuel World domain, with possible noise.
    From the paper:
    Hester and Stone, "Real Time Targeted Exploration in Large Domains", ICDL 2010.
    \author Todd Hester
*/

#include <rl_env/Arcade.hh>
#include <cmath>

char* Arcade::getEnvironmentDescription() {
    return 
        "Specialized environment for Asterix, "
        "uses 18 features:\n"
        "00: self x position\n"
        "01: self row number\n"
        "02: row 0 obj position\n"
        "03: row 0 obj ID\n"
        "04: row 1 obj position\n"
        "05: row 1 obj ID\n"
        "06: row 2 obj position\n"
        "07: row 2 obj ID\n"
        "08: row 3 obj position\n"
        "09: row 3 obj ID\n"
        "10: row 4 obj position\n"
        "11: row 4 obj ID\n"
        "12: row 5 obj position\n"
        "13: row 5 obj ID\n"
        "14: row 6 obj position\n"
        "15: row 6 obj ID\n"
        "16: row 7 obj position\n"
        "17: row 7 obj ID\n"
        ;
}

Arcade::Arcade(char* rom_path) :
	totalScore(0), display_active(false), game_over(false), stateSpaceLength(18), state(stateSpaceLength),
    modelSpecs(stateSpaceLength + 3)
{
  // save the path
  romPath = rom_path;

  // Check that rom exists and is readable
  ifstream file(romPath);
  if (!file.good()) {
      cerr << "Unable to find or open rom file: \"" << romPath << "\"" << endl;
      exit(-1);
  }

  modelSpecs[0].modelType = C45TREE;
  modelSpecs[1].modelType = C45TREE;
  modelSpecs[2].modelType = C45TREE;
  modelSpecs[3].modelType = C45TREE;
  modelSpecs[4].modelType = C45TREE;
  modelSpecs[5].modelType = C45TREE;
  modelSpecs[6].modelType = C45TREE;
  modelSpecs[7].modelType = C45TREE;
  modelSpecs[8].modelType = C45TREE;
  modelSpecs[9].modelType = C45TREE;
  modelSpecs[10].modelType = C45TREE;
  modelSpecs[11].modelType = C45TREE;
  modelSpecs[12].modelType = C45TREE;
  modelSpecs[13].modelType = C45TREE;
  modelSpecs[14].modelType = C45TREE;
  modelSpecs[15].modelType = C45TREE;
  modelSpecs[16].modelType = C45TREE;
  modelSpecs[17].modelType = C45TREE;
  modelSpecs[19].modelType = C45TREE;
  modelSpecs[20].modelType = C45TREE;

  totalScore = 0;
  game_over = false;
  // Initialize Atari Stuff
  if (!ale.loadROM(romPath, display_active, true)) {
      cerr << "Ale had problem loading rom..." << endl;
      exit(-1);
  }

  reset();
}

Arcade::~Arcade() {
}

const std::vector<float> &Arcade::sensation() const {
    return state;
}

float Arcade::apply(int action) {
    Action a = ale.allowed_actions[action];

    float prevY = state[0];

    int idleFrames = 3;
    float reward = 0;
    reward += ale.act(a);
    for (int i = 0; i < idleFrames; i++) {
            reward += ale.act(a);
            game_over = ale.game_over();
            if (game_over) {
                    break;
            }
    }
    totalScore += reward;
    if (game_over) {
      printf("Game over! Total score was %ld.\n", totalScore);
      reward = minReward;
    }

    if (reward != 0)
      printf("reward: %f\n", reward);

    updateState();

    return reward;
}

void Arcade::updateState() {
    for (int i = 0; i < state.size(); i++) {
        state[i] = -1;
    }

    // do self state
    point selfLoc = ale.getSelfLocation();
    state[0] = selfLoc.x;
    state[1] = (int) ((selfLoc.y - 31) / 16);

    // do radar state
    vector<pair<CompositeObject,long> > objs = ale.getNonSelfObjs();
    for (vector<pair<CompositeObject,long> >::iterator it=objs.begin(); it != objs.end(); it++) {
        pair<CompositeObject,long> pair = *it;
        CompositeObject obj = pair.first;
        long objID = pair.second;
        point objLoc = obj.get_centroid();
        //int xdist = selfLoc.x - objLoc.x;
        //int ydist = selfLoc.y - objLoc.y;
        if (objLoc.y < 47) {
          state[2] = objLoc.x;
          state[3] = objID;
        }
        else if (objLoc.y < 63) {
          state[4] = objLoc.x;
          state[5] = objID;
        }
        else if (objLoc.y < 79) {
          state[6] = objLoc.x;
          state[7] = objID;
        }
        else if (objLoc.y < 95) {
          state[8] = objLoc.x;
          state[9] = objID;
        }
        else if (objLoc.y < 111) {
          state[10] = objLoc.x;
          state[11] = objID;
        }
        else if (objLoc.y < 127) {
          state[12] = objLoc.x;
          state[13] = objID;
        }
        else if (objLoc.y < 143) {
          state[14] = objLoc.x;
          state[15] = objID;
        }
        else if (objLoc.y < 159) {
          state[16] = objLoc.x;
          state[17] = objID;
        }
    }
    printf("STATE: ");
    for (int i = 0; i < state.size() - 1; i++) {
        printf("%f, ", state[i]);
    }
    printf("%f\n", state[state.size() - 1]);

        /*
        printf("objID: %ld, xdist: %d, ydist: %d\n", objID, xdist, ydist);
		if (abs(xdist) <= 10) {
			if (ydist >= 0 && (abs(ydist) < state[2] || state[2] == -1)) {
				state[2] = abs(ydist);
				state[6] = objID;
			}
			else if (ydist < 0 && (abs(ydist) < state[3] || state[3] == -1)) {
				state[3] = abs(ydist);
				state[7] = objID;
			}
		}
		if (abs(ydist) <= 10) {
			if (xdist >= 0 && (abs(xdist) < state[4] || state[4] == -1)) {
				state[4] = abs(xdist);
				state[8] = objID;
			}
			else if (xdist < 0 && (abs(xdist) < state[5] || state[5] == -1)) {
				state[5] = abs(xdist);
				state[9] = objID;
			}
		}
        */
}

bool Arcade::terminal() const {
	return game_over;
}

void Arcade::reset() {
  printf("---------------RESET WAS CALLED!!!!----------------\n");
  totalScore = 0;
  game_over = false;
  ale.reset_game();

  point selfLoc = ale.getSelfLocation();
  while (selfLoc.x == -1) {
    ale.act((Action) 1);
    ale.act((Action) 2);
    selfLoc = ale.getSelfLocation();
  }

  updateState();
}

int Arcade::getNumActions() {
  return ale.allowed_actions.size();
}

std::vector<experience> Arcade::getSeedings() {
  // return empty seedings
  std::vector<experience> seeds;
  return seeds;
}

void Arcade::getMinMaxFeatures(std::vector<float> *minFeat,
                                    std::vector<float> *maxFeat){
  minFeat->resize(stateSpaceLength, -1);
  minFeat->at(2) = -2;
  maxFeat->resize(stateSpaceLength, 0);
  maxFeat->at(0) = 160;
  maxFeat->at(1) = 7;
  maxFeat->at(2) = 160;
  maxFeat->at(3) = 2;
  maxFeat->at(4) = 160;
  maxFeat->at(5) = 2;
  maxFeat->at(6) = 160;
  maxFeat->at(7) = 2;
  maxFeat->at(8) = 160;
  maxFeat->at(9) = 2;
  maxFeat->at(10) = 160;
  maxFeat->at(11) = 2;
  maxFeat->at(12) = 160;
  maxFeat->at(13) = 2;
  maxFeat->at(14) = 160;
  maxFeat->at(15) = 2;
  maxFeat->at(16) = 160;
  maxFeat->at(17) = 2;
}

void Arcade::getDiscretization(std::vector<int> *statesPerDim) {
    statesPerDim->resize(stateSpaceLength,0);
    statesPerDim->at(0) = 10;
    statesPerDim->at(1) = 10;
    statesPerDim->at(2) = 3;
    statesPerDim->at(3) = 2;
    statesPerDim->at(4) = 3;
    statesPerDim->at(5) = 2;
    statesPerDim->at(6) = 3;
    statesPerDim->at(7) = 2;
    statesPerDim->at(8) = 3;
    statesPerDim->at(9) = 2;
    statesPerDim->at(10) = 3;
    statesPerDim->at(11) = 2;
    statesPerDim->at(12) = 3;
    statesPerDim->at(13) = 2;
    statesPerDim->at(14) = 3;
    statesPerDim->at(15) = 2;
    statesPerDim->at(16) = 3;
    statesPerDim->at(17) = 2;
}

void Arcade::getMinMaxReward(float *minR,
                               float *maxR){
  string romStr (romPath);
  if (romStr.find("asterix") != string::npos) {
    minReward = 0;
    maxReward = 50;
  }
  else if (romStr.find("boxing") != string::npos) {
    minReward = -1;
    maxReward = 1;
  }
  else if (romStr.find("freeway") != string::npos) {
    minReward = 0;
    maxReward = 1;
  }
  else {
    minReward = 0;
    maxReward = 1;
  }
  *minR = minReward;
  *maxR = maxReward;
}

bool Arcade::isEpisodic() {
    return true;
}

bool Arcade::invalidStateChange(int lastAction) {
    return ale.getSelfLocation().x == -1;
}

std::vector<ModelSpecification>& Arcade::getModelSpecs() {
    // encode actions as binary representation
    return modelSpecs;
}
