/** \file Arcade.cc
    Implements the Fuel World domain, with possible noise.
    From the paper:
    Hester and Stone, "Real Time Targeted Exploration in Large Domains", ICDL 2010.
    \author Todd Hester
*/

#include <rl_env/Arcade.hh>
#include <cmath>

Arcade::Arcade(char* rom_path) :
	totalScore(0), display_active(true), game_over(false), stateSpaceLength(8), state(stateSpaceLength),
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
    // 0 = self x position
    // 1 = self row
    // 2 = row above obj relative x distance
    // 3 = row above obj ID
    // 4 = row below obj relative x distance
    // 5 = row below obj ID
    // 6 = row current obj relative x distance
    // 7 = row obj ID
    // 8 = action
    // 9 = reward
    // 10 = terminal

  modelSpecs[0].modelType = M5MULTI;
  modelSpecs[1].modelType = C45TREE;
  modelSpecs[2].modelType = M5MULTI;
  modelSpecs[3].modelType = C45TREE;
  modelSpecs[4].modelType = M5MULTI;
  modelSpecs[5].modelType = C45TREE;
  modelSpecs[6].modelType = M5MULTI;
  modelSpecs[7].modelType = C45TREE;
  modelSpecs[8].modelType = C45TREE;
  modelSpecs[10].modelType = C45TREE;
  modelSpecs[11].modelType = C45TREE;

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
    state[2] = 0;
    state[4] = 0;
    state[6] = 0;
    state[7] = 0;

    // 0 = self x position
    // 1 = self row
    // 2 = row above obj collision imminent
    // 3 = row above obj ID
    // 4 = row below obj collision imminent
    // 5 = row below obj ID
    // 6 = row obj collision imminent left
    // 7 = row obj collision imminent right
    // 8 = row obj ID
    // 9 = action
    // 10 = reward
    // 11 = terminal

    // do self state
    point selfLoc = ale.getSelfLocation();
	state[0] = selfLoc.x;
    state[1] = (int) ((selfLoc.y - 31) / 16);

    // do radar state
	vector<pair<CompositeObject,long> > objs = ale.getNonSelfObjs();
	//for (int i = 0; i < objs.size(); i++) {
	//	CompositeObject obj = objs[i];
    float distToNearest = -1;
    long radius = 20;
    long alignmentRadius = 10;
    for (vector<pair<CompositeObject,long> >::iterator it=objs.begin(); it != objs.end(); it++) {
        pair<CompositeObject,long> pair = *it;
        CompositeObject obj = pair.first;
        long objID = pair.second;
        point objLoc = obj.get_centroid();
        int xdist = selfLoc.x - objLoc.x;
        int ydist = selfLoc.y - objLoc.y;
            float objDist = sqrt(pow(xdist, 2) + pow(ydist, 2));
     // 0 = self x position
    // 1 = self row
    // 2 = row above obj relative x distance
    // 3 = row above obj ID
    // 4 = row below obj relative x distance
    // 5 = row below obj ID
    // 6 = row current obj relative x distance
    // 7 = row obj ID
    // 8 = action
    // 9 = reward
    // 10 = terminal
        if (abs(ydist) < 5) {
		state[6] = xdist;
            state[7] = objID;
        }
        else if (ydist > 0 && ydist < 20) {
            state[2] = xdist;
            state[3] = objID;
        }
        else if (ydist < 0 && ydist > -20) {
            state[4] = xdist;
            state[5] = objID;
        }
 
/*        
        if (abs(ydist) < 5 && abs(xdist) < 10) {
            if (xdist < 0) {
                state[7] = 1;
            }
            else if (xdist > 0) {
                state[6] = 1;
            }
            state[8] = objID;
        }
        else if (ydist > 0 && ydist < 20 && abs(xdist) < 10) {
            state[2] = 1;
            state[3] = objID;
        }
        else if (ydist < 0 && ydist > -20 && abs(xdist) < 10) {
            state[4] = 1;
            state[5] = objID;
        }
  */      
	/*
        if (abs(ydist) < 5) {
            if (xdist < 0) {
                state[7] = 1;
            }
            else if (xdist > 0) {
                state[6] = 1;
            }
            state[8] = objID;
        }
        else if (abs(xdist) < 13) {
            if (ydist < 0) {
                state[4] = 1;
                state[5] = objID;
            }
            else if (ydist > 0) {
                state[2] = 1;
                state[3] = objID;
            }
       }
	*/
 
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
  // Initialize Atari Stuff
  if (!ale.loadROM(romPath, display_active, true)) {
      cerr << "Ale had problem loading rom..." << endl;
      exit(-1);
  }

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
    // 0 = self x position
    // 1 = self row
    // 2 = row above obj collision imminent
    // 3 = row above obj ID
    // 4 = row below obj collision imminent
    // 5 = row below obj ID
    // 6 = row obj collision imminent left
    // 7 = row obj collision imminent right
    // 8 = row obj ID

/*
  minFeat->resize(stateSpaceLength, 0);
  minFeat->at(0) = -1;
  minFeat->at(1) = -2;
  minFeat->at(2) = 0;
  minFeat->at(3) = -1;
  minFeat->at(4) = 0;
  minFeat->at(5) = -1;
  minFeat->at(6) = 0;
  minFeat->at(7) = 0;
  minFeat->at(8) = -1;
  maxFeat->resize(stateSpaceLength, 0);
  maxFeat->at(0) = 162;
  maxFeat->at(1) = 7;
  maxFeat->at(2) = 1;
  maxFeat->at(3) = 2;
  maxFeat->at(4) = 1;
  maxFeat->at(5) = 2;
  maxFeat->at(6) = 1;
  maxFeat->at(7) = 1;
  maxFeat->at(8) = 2;
*/



    // 0 = self x position
    // 1 = self row
    // 2 = row above obj relative x distance
    // 3 = row above obj ID
    // 4 = row below obj relative x distance
    // 5 = row below obj ID
    // 6 = row current obj relative x distance
    // 7 = row obj ID
    // 8 = action
    // 9 = reward
    // 10 = terminal


  minFeat->resize(stateSpaceLength, 0);
  minFeat->at(0) = -1;
  minFeat->at(1) = -2;
  minFeat->at(2) = -162;
  minFeat->at(3) = -1;
  minFeat->at(4) = -162;
  minFeat->at(5) = -1;
  minFeat->at(6) = -162;
  minFeat->at(7) = -1;
  maxFeat->resize(stateSpaceLength, 0);
  maxFeat->at(0) = 162;
  maxFeat->at(1) = 7;
  maxFeat->at(2) = 162;
  maxFeat->at(3) = 2;
  maxFeat->at(4) = 162;
  maxFeat->at(5) = 2;
  maxFeat->at(6) = 162;
  maxFeat->at(7) = 2;


}

void Arcade::getMinMaxReward(float *minR,
                               float *maxR){
  string romStr (romPath);
  if (romStr.find("asterix") != string::npos) {
    minReward = -50;
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

bool Arcade::lostLocation() {
    return ale.getSelfLocation().x == -1;
}

bool Arcade::invalidStateChange() {
    return false;
}

std::vector<ModelSpecification>& Arcade::getModelSpecs() {
    // encode actions as binary representation
    /*
    for (unsigned j = 0; j < modelSpecs.size(); j++) {
        std::vector<unsigned> dep = modelSpecs[j].dependencies;
        if (std::find(dep.begin(), dep.end(), stateSpaceLength) != dep.end()) {
            for (unsigned k = stateSpaceLength + 1; k < stateSpaceLength + ale.allowed_actions.size(); k++) {
                modelSpecs[j].dependencies.push_back(k);
            }
        }
    }
    */

    return modelSpecs;
}
