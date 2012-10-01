/** \file Arcade.cc
    Implements the Fuel World domain, with possible noise.
    From the paper:
    Hester and Stone, "Real Time Targeted Exploration in Large Domains", ICDL 2010.
    \author Todd Hester
*/

#include <rl_env/Arcade.hh>

Arcade::Arcade(string _rom_file) :
	rom_file(""), numActions(0), display_active(true), game_over(false), state(RAM_LENGTH)
{
	rom_file = _rom_file;
  // Check that rom exists and is readable
  ifstream file(rom_file.c_str());
  if (!file.good()) {
      cerr << "Unable to find or open rom file: \"" << rom_file << "\"" << endl;
      exit(-1);
  }

  // Initialize Atari Stuff
  if (!ale.loadROM(rom_file.c_str(), display_active, false)) {
      cerr << "Ale had problem loading rom..." << endl;
      exit(-1);
  }
  numActions = ale.allowed_actions.size();

  // init state
	state.clear();
  for(int i = 0; i<RAM_LENGTH/8; i++) {
  	state.push_back((float) ale.ram_content.at(i));
  }
}

Arcade::~Arcade() {
}

const std::vector<float> &Arcade::sensation() const {
	return state;
}

float Arcade::apply(int action) {
	int framesPerAction = 10;
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

	if (reward != 0)
			printf("reward: %f\n", reward);

	state.clear();
  for(int i = 0; i<RAM_LENGTH/8; i++) {
  	state.push_back((float) ale.ram_content.at(i));
  }

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

int Arcade::getNumActions(){
  return numActions;
}

std::vector<experience> Arcade::getSeedings() {
  // return empty seedings
  std::vector<experience> seeds;
  return seeds;
}

void Arcade::getMinMaxFeatures(std::vector<float> *minFeat,
                                    std::vector<float> *maxFeat){
  minFeat->resize(RAM_LENGTH/8, 0.0);
  maxFeat->resize(RAM_LENGTH/8, 255.0);
}

void Arcade::getMinMaxReward(float *minR,
                               float *maxR){
  
  *minR = -1000.0;
  *maxR = 1000.0;
}
