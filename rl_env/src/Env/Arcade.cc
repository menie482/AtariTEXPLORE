/** \file Arcade.cc
    Implements the Fuel World domain, with possible noise.
    From the paper:
    Hester and Stone, "Real Time Targeted Exploration in Large Domains", ICDL 2010.
    \author Todd Hester
*/

#include <rl_env/Arcade.hh>

Arcade::Arcade(char* rom_path) :
	totalScore(0), display_active(true), game_over(false), stateSpaceLength(10), state(stateSpaceLength),
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

	// 0 = selfLoc x
    // 1 = selfLoc y
    // 2 = radar in front
    // 3 = radar behind
    // 4 = radar left
    // 5 = radar right
    // 6 = size in front
    // 7 = size behind
    // 8 = size left
    // 9 = size right
	// 10 = action
    // 11 = reward tree
    // 12 = terminal tree

  // establish dep structure
  modelSpecs[0].modelType = C45TREE;
  modelSpecs[0].dependencies.push_back(0);
  modelSpecs[0].dependencies.push_back(1);
  modelSpecs[0].dependencies.push_back(10);

  modelSpecs[1].modelType = C45TREE;
  modelSpecs[1].dependencies.push_back(1);
  modelSpecs[1].dependencies.push_back(0);
  modelSpecs[1].dependencies.push_back(10);

  modelSpecs[2].modelType = C45TREE;
  modelSpecs[2].dependencies.push_back(2);
  modelSpecs[2].dependencies.push_back(10);

  modelSpecs[3].modelType = C45TREE;
  modelSpecs[3].dependencies.push_back(3);
  modelSpecs[3].dependencies.push_back(10);

  modelSpecs[4].modelType = C45TREE;
  modelSpecs[4].dependencies.push_back(4);
  modelSpecs[4].dependencies.push_back(10);

  modelSpecs[5].modelType = C45TREE;
  modelSpecs[5].dependencies.push_back(5);
  modelSpecs[5].dependencies.push_back(10);

  modelSpecs[6].modelType = CONSTANT;
  modelSpecs[6].dependencies.push_back(6);

  modelSpecs[7].modelType = CONSTANT;
  modelSpecs[7].dependencies.push_back(7);

  modelSpecs[8].modelType = CONSTANT;
  modelSpecs[8].dependencies.push_back(8);

  modelSpecs[9].modelType = CONSTANT;
  modelSpecs[9].dependencies.push_back(9);

  modelSpecs[11].modelType = C45TREE;
  modelSpecs[11].dependencies.push_back(2);
  modelSpecs[11].dependencies.push_back(3);
  modelSpecs[11].dependencies.push_back(4);
  modelSpecs[11].dependencies.push_back(5);
  modelSpecs[11].dependencies.push_back(6);
  modelSpecs[11].dependencies.push_back(7);
  modelSpecs[11].dependencies.push_back(8);
  modelSpecs[11].dependencies.push_back(9);
  modelSpecs[11].dependencies.push_back(10);

  modelSpecs[12].modelType = C45TREE;
  modelSpecs[12].dependencies.push_back(2);
  modelSpecs[12].dependencies.push_back(3);
  modelSpecs[12].dependencies.push_back(4);
  modelSpecs[12].dependencies.push_back(5);
  modelSpecs[12].dependencies.push_back(6);
  modelSpecs[12].dependencies.push_back(7);
  modelSpecs[12].dependencies.push_back(8);
  modelSpecs[12].dependencies.push_back(9);
  modelSpecs[12].dependencies.push_back(10);

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

	int framesPerAction = 1;
	float reward = 0;
	for (int i = 0; i < framesPerAction; i++) {
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

    // do self state
    point selfLoc = ale.getSelfLocation();
	state[0] = selfLoc.x;
    state[1] = selfLoc.y;

	// do radar state
	vector<pair<CompositeObject,long> > objs = ale.getNonSelfObjs();
	//for (int i = 0; i < objs.size(); i++) {
	//	CompositeObject obj = objs[i];
    for (vector<pair<CompositeObject,long> >::iterator it=objs.begin(); it != objs.end(); it++) {
        pair<CompositeObject,long> pair = *it;
        CompositeObject obj = pair.first;
		long objID = pair.second;
		point objLoc = obj.get_centroid();
		int xdist = selfLoc.x - objLoc.x;
		int ydist = selfLoc.y - objLoc.y;
		
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
	}
	
	printf("STATE: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", state[0], state[1], state[2], state[3], state[4], state[5], state[6], state[7], state[8], state[9]);
	return reward;
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

  state.clear();
  for (int i = 0; i < stateSpaceLength; i++) {
    state.push_back(-1);
  }
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
  maxFeat->resize(stateSpaceLength, 192);
}

void Arcade::getMinMaxReward(float *minR,
                               float *maxR){
  
  *minR = -50;
  *maxR = 50;
}

bool Arcade::isEpisodic() {
    return true;
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
