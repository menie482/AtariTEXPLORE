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
        "uses 10 features:\n"
        "0: self x position\n"
        "1: self row number\n"
        "2: imminent left ID\n"
        "3: imminent right ID\n"
        "4: imminent up ID\n"
        "5: imminent down ID\n"
        "6: collision imminent left\n"
        "7: collision imminent right\n"
        "8: collision imminent up\n"
        "9: collision imminent down\n"
        ;
}

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
  modelSpecs[11].modelType = C45TREE;
  modelSpecs[12].modelType = C45TREE;

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
    state[2] = 0;
    state[3] = 0;
    state[4] = 0;
    state[5] = 0;

    // do self state
    point selfLoc = ale.getSelfLocation();
    state[0] = selfLoc.x;
    state[1] = (int) ((selfLoc.y - 31) / 16);

    // do radar state
    vector<pair<CompositeObject,long> > objs = ale.getNonSelfObjs();
    int collisionRadius = 20;
    for (vector<pair<CompositeObject,long> >::iterator it=objs.begin(); it != objs.end(); it++) {
        pair<CompositeObject,long> pair = *it;
        CompositeObject obj = pair.first;
        long objID = pair.second;
        point objLoc = obj.get_centroid();
        int xdist = selfLoc.x - objLoc.x;
        int ydist = selfLoc.y - objLoc.y;

        // if xdist is in collision radius
        if (abs(xdist) < collisionRadius) {
            // if obj is in same row
            if (abs(ydist) < 10) {
                // collision imminent left
                if (xdist > 0) {
                    state[2] = objID;
                    state[6] = 1;
                }
                // collision imminent right
                else {
                    state[3] = objID;
                    state[7] = 1;
                }
            }
            // if obj is in row above or below
            else if (abs(ydist) < 25) {
                // collision imminent up
                if (ydist > 0) {
                    state[4] = objID;
                    state[8] = 1;
                }
                // collision imminent down
                if (ydist < 0) {
                    state[5] = objID;
                    state[9] = 1;
                }
            }
        }
    }
    printf("STATE: ");
    for (int i = 0; i < state.size() - 1; i++) {
        printf("%f, ", state[i]);
    }
    printf("%f\n", state[state.size() - 1]);
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

          "Specialized environment for Asterix, "
        "uses 10 features:\n"
        "0: self x position\n"
        "1: self row number\n"
        "2: imminent left ID\n"
        "3: imminent right ID\n"
        "4: imminent up ID\n"
        "5: imminent down ID\n"
        "6: collision imminent left\n"
        "7: collision imminent right\n"
        "8: collision imminent up\n"
        "9: collision imminent down\n"
 
  
  minFeat->resize(stateSpaceLength, -1);
  minFeat->at(6) = 0;
  minFeat->at(7) = 0;
  minFeat->at(8) = 0;
  minFeat->at(9) = 0;
  maxFeat->resize(stateSpaceLength, 0);
  maxFeat->at(0) = 160;
  maxFeat->at(1) = 7;
  maxFeat->at(2) = 2;
  maxFeat->at(3) = 2;
  maxFeat->at(4) = 2;
  maxFeat->at(5) = 2;
  maxFeat->at(6) = 1;
  maxFeat->at(7) = 1;
  maxFeat->at(8) = 1;
  maxFeat->at(9) = 1;
}

void Arcade::getDiscretization(std::vector<int> *statesPerDim) {
    statesPerDim->resize(stateSpaceLength,0);
    statesPerDim->at(0) = 10;
    statesPerDim->at(1) = 9;
    statesPerDim->at(2) = 4;
    statesPerDim->at(3) = 4;
    statesPerDim->at(4) = 4;
    statesPerDim->at(5) = 4;
    statesPerDim->at(6) = 2;
    statesPerDim->at(7) = 2;
    statesPerDim->at(8) = 2;
    statesPerDim->at(9) = 2;
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
