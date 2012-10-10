/** \file Arcade.cc
    Implements the Fuel World domain, with possible noise.
    From the paper:
    Hester and Stone, "Real Time Targeted Exploration in Large Domains", ICDL 2010.
    \author Todd Hester
*/

#include <rl_env/Arcade.hh>

Arcade::Arcade(char* romPath) :
	numActions(0), totalScore(0), display_active(true), game_over(false), stateSpaceLength(6), state(stateSpaceLength)
{
  // Check that rom exists and is readable
  ifstream file(romPath);
  if (!file.good()) {
      cerr << "Unable to find or open rom file: \"" << romPath << "\"" << endl;
      exit(-1);
  }

  // Initialize Atari Stuff
  if (!ale.loadROM(romPath, display_active, true)) {
      cerr << "Ale had problem loading rom..." << endl;
      exit(-1);
  }
  numActions = ale.allowed_actions.size();

  // init state
	state.clear();
  for(int i = 0; i<stateSpaceLength; i++) {
  	state.push_back(-1);
  }
}

Arcade::~Arcade() {
}

const std::vector<float> &Arcade::sensation() const {
	return state;
}

float Arcade::apply(int action) {
	int framesPerAction = 3;
	Action a;

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
	}
	
	float reward = 0;
	for (int i = 0; i < framesPerAction; i++) {
		reward += ale.act(a);
		game_over = ale.game_over();
		if (game_over) {
			break;
		}
	}
	totalScore += reward;
	if (game_over)
		printf("Game over! Total score was %d.\n", totalScore);

	if (reward != 0)
		printf("reward: %f\n", reward);

	// do self state
	state.clear();
	point selfLoc = ale.getSelfLocation();
	state.push_back(selfLoc.x);
	state.push_back(selfLoc.y);

	// do radar state
	state.push_back(0); // sense in front
	state.push_back(0); // sense behind
	state.push_back(0); // sense to the left
	state.push_back(0); // sense to the right
	
	// negative reward for lack of info
	if (state[0] == -1)
		reward = -1;
	
	vector<point> objLocations = ale.getNonSelfObjLocations();
	for (int i = 0; i < objLocations.size(); i++) {
		point objLoc = objLocations[i];
		int xdist = selfLoc.x - objLoc.x;
		int ydist = selfLoc.y - objLoc.y;
		
		if (abs(xdist) <= 10) {
			if (ydist >= 0 && (abs(ydist) < state[2] || state[2] == 0)) {
				state[2] = abs(ydist);
			}
			else if (ydist < 0 && (abs(ydist) < state[3] || state[3] == 0)) {
				state[3] = abs(ydist);
			}
		}
		if (abs(ydist) <= 10) {
			if (xdist >= 0 && (abs(xdist) < state[4] || state[4] == 0)) {
				state[4] = abs(xdist);
			}
			else if (xdist < 0 && (abs(xdist) < state[5] || state[5] == 0)) {
				state[5] = abs(xdist);
			}
		}
	}
	if (objLocations.size() == 0) {
		state[2] = -1;
		state[3] = -1;
		state[4] = -1;
		state[5] = -1;
	}
	
	printf("STATE: %f, %f, %f, %f, %f, %f\n", state[0], state[1], state[2], state[3], state[4], state[5]);

	if (game_over)
		return 0.0;
	
	return reward;
}

bool Arcade::terminal() const {
	return game_over;
}


void Arcade::reset() {
	ale.reset_game();
}

int Arcade::getNumActions() {
  return numActions;
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
  
  *minR = -1000.0;
  *maxR = 1000.0;
}
