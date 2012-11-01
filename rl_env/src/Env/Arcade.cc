/** \file Arcade.cc
    Implements the Fuel World domain, with possible noise.
    From the paper:
    Hester and Stone, "Real Time Targeted Exploration in Large Domains", ICDL 2010.
    \author Todd Hester
*/

#include <rl_env/Arcade.hh>

Arcade::Arcade(char* rom_path) :
	totalScore(0), display_active(true), game_over(false), stateSpaceLength(1), state(stateSpaceLength),
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

	// 0 = selfLoc y
	// 1 = action
    // 2 = reward tree
    // 3 = terminal tree

  // establish dependency structure
  modelSpecs[0].modelType = C45TREE;
  //modelSpecs[0].modelType = C45TREE;
  //modelSpecs[0].dependencies.push_back(0);
  modelSpecs[0].dependencies.push_back(1);
  modelSpecs[2].modelType = C45TREE;
  modelSpecs[2].dependencies.push_back(1);
  modelSpecs[3].modelType = C45TREE;
  modelSpecs[3].dependencies.push_back(0);
  modelSpecs[3].dependencies.push_back(1);

  reset();
}

Arcade::~Arcade() {
}

const std::vector<float> &Arcade::sensation() const {
    return state;
}

float Arcade::apply(int action) {
	Action a = ale.allowed_actions[action];

    //printf("Texplore: %d, Atari: %d\n", action, a);

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
        return 0;
    }

    point selfLoc = ale.getSelfLocation();
	
    if (reward != 0)
        printf("reward: %f\n", reward);
    else if (selfLoc.y != -1 && prevY != -1) {
        reward = prevY - selfLoc.y;
        if (reward > 1 || reward < -1)
            reward = 0;
    }

    // do self state
	state[0] = selfLoc.y;
    //state[0] = 0;
    //printf("Reward: %f\n", reward);
/*
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
*/
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
  while (selfLoc.y == -1 || selfLoc.y == 0) {
    ale.act((Action) 2);
    ale.act((Action) 5);
    selfLoc = ale.getSelfLocation();
  }

  //init state
  state.clear();
  state.push_back(selfLoc.y);
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
  
  *minR = -1;
  *maxR = 1;
}

bool Arcade::isEpisodic() {
    return true;
}

const std::vector<ModelSpecification>& Arcade::getModelSpecs() {
    return modelSpecs;
}
