/** \file Arcade.hh
    Defines the Arcade Learning Environment domain
    \author Chris Donahue
*/

#include <rl_common/core.hh>
//#include "../ale/src/ale_interface.hpp"
#include "ale_interface.hpp"
#include <stdio.h>

/** This class defines the Atari Arcade domain */
class Arcade: public Environment {
public:
  /** Creates an Arcade Learning environment
  */
  Arcade(string _rom_file);

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

	string rom_file;
	int numActions;
	bool display_active;
	bool game_over;
	
	int numEpisodes;
	

private:
  std::vector<float> state;
};
