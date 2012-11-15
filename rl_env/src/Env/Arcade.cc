/** \file Arcade.cc
    Implements the Fuel World domain, with possible noise.
    From the paper:
    Hester and Stone, "Real Time Targeted Exploration in Large Domains", ICDL 2010.
    \author Todd Hester
*/

#include <rl_env/Arcade.hh>
#include <cmath>

Arcade::Arcade(char* rom_path) :
	totalScore(0), display_active(true), game_over(false), stateSpaceLength(6), state(stateSpaceLength),
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

    // 0 = self x
    // 1 = self y
    // 2 = nearest obj dist x
    // 3 = nearest obj dist y
    // 4 = nearest obj id
    // 5 = collision imminent
    // 6 = action
    // 7 = reward tree
    // 8 = terminal tree

  modelSpecs[0].modelType = C45TREE;
  //modelSpecs[0].dependencies.push_back(0);
  //modelSpecs[0].dependencies.push_back(1);
  modelSpecs[0].dependencies.push_back(6);
  
  modelSpecs[1].modelType = C45TREE;
  //modelSpecs[1].dependencies.push_back(0);
  //modelSpecs[1].dependencies.push_back(1);
  modelSpecs[1].dependencies.push_back(6);

  modelSpecs[2].modelType = C45TREE;
  //modelSpecs[2].dependencies.push_back(2);
  modelSpecs[2].dependencies.push_back(4);

  modelSpecs[3].modelType = C45TREE;
  //modelSpecs[3].dependencies.push_back(3);
  modelSpecs[3].dependencies.push_back(4);

  modelSpecs[4].modelType = CONSTANT;
  modelSpecs[4].dependencies.push_back(4);

  modelSpecs[5].modelType = C45TREE;
  //modelSpecs[5].dependencies.push_back(0);
  //modelSpecs[5].dependencies.push_back(1);
  modelSpecs[5].dependencies.push_back(2);
  modelSpecs[5].dependencies.push_back(3);
  modelSpecs[5].dependencies.push_back(4);
  modelSpecs[5].dependencies.push_back(5);
  modelSpecs[5].dependencies.push_back(6);

  modelSpecs[7].modelType = C45TREE;
  modelSpecs[7].dependencies.push_back(0);
  modelSpecs[7].dependencies.push_back(1);
  //modelSpecs[7].dependencies.push_back(2);
  //modelSpecs[7].dependencies.push_back(3);
  modelSpecs[7].dependencies.push_back(4);
  modelSpecs[7].dependencies.push_back(5);
  modelSpecs[7].dependencies.push_back(6);
  
  modelSpecs[8].modelType = C45TREE;
  //modelSpecs[8].dependencies.push_back(0);
  //modelSpecs[8].dependencies.push_back(1);
  //modelSpecs[8].dependencies.push_back(2);
  //modelSpecs[8].dependencies.push_back(3);
  modelSpecs[8].dependencies.push_back(4);
  modelSpecs[8].dependencies.push_back(5);
  modelSpecs[8].dependencies.push_back(6);

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
        return -50.0;
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
    state[5] = 0;

    // do self state
    point selfLoc = ale.getSelfLocation();
	state[0] = selfLoc.x;
    state[1] = selfLoc.y;

	// do radar state
	vector<pair<CompositeObject,long> > objs = ale.getNonSelfObjs();
	//for (int i = 0; i < objs.size(); i++) {
	//	CompositeObject obj = objs[i];
    float distToNearest = -1;
    for (vector<pair<CompositeObject,long> >::iterator it=objs.begin(); it != objs.end(); it++) {
        pair<CompositeObject,long> pair = *it;
        CompositeObject obj = pair.first;
		long objID = pair.second;
		point objLoc = obj.get_centroid();
		int xdist = selfLoc.x - objLoc.x;
		int ydist = selfLoc.y - objLoc.y;
        float objDist = sqrt(pow(xdist, 2) + pow(ydist, 2));
        if (distToNearest == -1 || objDist < distToNearest) {
            distToNearest = objDist;
            state[2] = xdist;
            state[3] = ydist;
            state[4] = objID;
            if (distToNearest < 18)
                state[5] = 1;
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
  minFeat->resize(stateSpaceLength, 0);
  minFeat->at(0) = -1;
  minFeat->at(1) = -1;
  minFeat->at(2) = -160;
  minFeat->at(3) = -192;
  minFeat->at(4) = -1;
  minFeat->at(5) = -1;
  maxFeat->resize(stateSpaceLength, 0);
  maxFeat->at(0) = 160;
  maxFeat->at(1) = 192;
  maxFeat->at(2) = 160;
  maxFeat->at(3) = 192;
  maxFeat->at(4) = 3;
  maxFeat->at(5) = 1;
}

void Arcade::getMinMaxReward(float *minR,
                               float *maxR){
  
  *minR = -50;
  *maxR = 50;
}

bool Arcade::isEpisodic() {
    return true;
}

bool Arcade::lostLocation() {
    return ale.getSelfLocation().x == -1;
}

std::vector<ModelSpecification>& Arcade::getModelSpecs() {
    // encode actions as binary representation
    for (unsigned j = 0; j < modelSpecs.size(); j++) {
        std::vector<unsigned> dep = modelSpecs[j].dependencies;
        if (std::find(dep.begin(), dep.end(), stateSpaceLength) != dep.end()) {
            for (unsigned k = stateSpaceLength + 1; k < stateSpaceLength + ale.allowed_actions.size(); k++) {
                modelSpecs[j].dependencies.push_back(k);
            }
        }
    }

    return modelSpecs;
}
