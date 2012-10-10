/** \file Arcade.hh
    Defines the Arcade Learning Environment domain
    \author Chris Donahue
*/

#include <rl_common/core.hh>
//#include "../ale/src/ale_interface.hpp"
#include "ale_interface.hpp"
#include <stdio.h>
#include <cmath>

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
  virtual void getMinMaxFeatures(std::vector<float> *minFeat, std::vector<float> *maxFeat);
  virtual void getMinMaxReward(float* minR, float* maxR);

  virtual std::vector<experience> getSeedings();

protected:
  ALEInterface ale;
	IntMatrix screen_matrix;
	IntVect ram_content;

	int numActions;
	long totalScore;
	bool display_active;
	bool game_over;
  const int stateSpaceLength;
	
	int numEpisodes;

private:
  std::vector<float> state;
};