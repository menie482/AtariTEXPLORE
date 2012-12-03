/** \file Arcade.hh
    Defines the Arcade Learning Environment domain
    \author Chris Donahue
*/

#include <rl_common/core.hh>
#include "ale_interface.hpp"
#include <stdio.h>
#include <cmath>
#include <string>

/** This class defines the Atari Arcade domain */
class Arcade: public Environment {
public:
  /** Creates an Arcade Learning environment
  */
  Arcade(char* romPath);

  virtual ~Arcade();

  virtual const std::vector<float> &sensation() const;
  virtual float apply(int action);

  virtual bool terminal() const;
  virtual void reset();

  virtual int getNumActions();
  virtual void getDiscretization(std::vector<int> *statesPerDim);
  virtual void getMinMaxFeatures(std::vector<float> *minFeat, std::vector<float> *maxFeat);
  virtual void getMinMaxReward(float* minR, float* maxR);
  virtual bool isEpisodic();
  virtual std::vector<ModelSpecification>& getModelSpecs();
  virtual bool invalidStateChange(int lastAction);
  virtual char* getEnvironmentDescription();
  void updateState();

  virtual std::vector<experience> getSeedings();

  long totalScore;

protected:
  ALEInterface ale;
	IntMatrix screen_matrix;
	IntVect ram_content;

    char* romPath;
	bool display_active;
	bool game_over;
  const int stateSpaceLength;
	
private:
  float minReward;
  float maxReward;
  std::vector<float> state;
  std::vector<ModelSpecification> modelSpecs;
};
