/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2012 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and 
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include "Asteroids.hpp"

#include "../RomUtils.hpp"


AsteroidsSettings::AsteroidsSettings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* AsteroidsSettings::clone() const { 
    
    RomSettings* rval = new AsteroidsSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void AsteroidsSettings::step(const System& system) {

    // update the reward
    reward_t score = getDecimalScore(62, 61, &system);
    score *= 10;
    m_reward = score - m_score;
    m_score = score;

    // update terminal status
    int byte = readRam(&system, 60);
    byte = (byte - (byte & 15)) >> 4;
    m_terminal = byte < 1;
}


/* is end of game */
bool AsteroidsSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t AsteroidsSettings::getReward() const { 

    return m_reward; 
}


/* is an action legal */
bool AsteroidsSettings::isLegal(const Action &a) const {

    return true;    
}


/* reset the state of the game */
void AsteroidsSettings::reset() {
    
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
}


        
/* saves the state of the rom settings */
void AsteroidsSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void AsteroidsSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
}

