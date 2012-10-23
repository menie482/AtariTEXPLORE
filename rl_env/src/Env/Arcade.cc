/** \file Arcade.cc
    Implements the Fuel World domain, with possible noise.
    From the paper:
    Hester and Stone, "Real Time Targeted Exploration in Large Domains", ICDL 2010.
    \author Todd Hester
*/

#include <rl_env/Arcade.hh>

Arcade::Arcade(char* rom_path) :
	totalScore(0), display_active(true), game_over(false), stateSpaceLength(10), state(stateSpaceLength), dependencies(stateSpaceLength)
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

  // store dependency structure
  dependencies[0].push_back(0);
  dependencies[0].push_back(1);
  dependencies[0].push_back(10);

  dependencies[1].push_back(1);
  dependencies[1].push_back(0);
  dependencies[1].push_back(10);

  dependencies[2].push_back(2);
  dependencies[2].push_back(10);

  dependencies[3].push_back(3);
  dependencies[3].push_back(10);

  dependencies[4].push_back(4);
  dependencies[4].push_back(10);

  dependencies[5].push_back(5);
  dependencies[5].push_back(10);

  dependencies[6].push_back(6);
  dependencies[6].push_back(10);

  dependencies[7].push_back(7);
  dependencies[7].push_back(10);

  dependencies[8].push_back(8);
  dependencies[8].push_back(10);

  dependencies[9].push_back(9);
  dependencies[9].push_back(10);

  reset();
}

Arcade::~Arcade() {
}

const std::vector<float> &Arcade::sensation() const {
	return state;
}

float Arcade::apply(int action) {
	Action a = ale.allowed_actions[action];
/*
	if (action == 0) {
		a = PLAYER_A_NOOP;
	}
	else if (action == 1) {
		a = PLAYER_A_FIRE;
	}
	else if (action == 2) {
		a = PLAYER_A_UP;
	}
	else if (action == 3) {
		a = PLAYER_A_RIGHT;
	}
	else if (action == 4) {
		a = PLAYER_A_LEFT;
	}
	else if (action == 5) {
		a = PLAYER_A_DOWN;
	}
	else if (action == 6) {
		a = PLAYER_A_UPRIGHT;
	}
	else if (action == 7) {
		a = PLAYER_A_UPLEFT;
	}
	else if (action == 8) {
		a = PLAYER_A_DOWNRIGHT;
	}
	else if (action == 9) {
		a = PLAYER_A_DOWNLEFT;
	}
	else if (action == 10) {
		a = PLAYER_A_UPFIRE;
	}
	else if (action == 11) {
		a = PLAYER_A_RIGHTFIRE;
	}
	else if (action == 12) {
		a = PLAYER_A_LEFTFIRE;
	}
	else if (action == 13) {
		a = PLAYER_A_DOWNFIRE;
	}
	else if (action == 14) {
		a = PLAYER_A_UPRIGHTFIRE;
	}
	else if (action == 15) {
		a = PLAYER_A_UPLEFTFIRE;
	}
	else if (action == 16) {
		a = PLAYER_A_DOWNRIGHTFIRE;
	}
	else if (action == 17) {
		a = PLAYER_A_DOWNLEFTFIRE;
	}*/

    // init state
    for(int i = 0; i<stateSpaceLength; i++) {
  	    state[i] = -1;
    }

	int framesPerAction = 3;
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

  //init state
  state.clear();
  for(int i = 0; i<stateSpaceLength; i++) {
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
  
  *minR = -50.0;
  *maxR = 50.0;
}

bool Arcade::isEpisodic() {
    return true;
}

const std::vector<std::vector<int> >& Arcade::getDependencyStructure() {
    return dependencies;
}
