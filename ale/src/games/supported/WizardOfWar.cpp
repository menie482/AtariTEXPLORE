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
#include "WizardOfWar.hpp"

#include "../RomUtils.hpp"


WizardOfWarSettings::WizardOfWarSettings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* WizardOfWarSettings::clone() const { 
    
    RomSettings* rval = new WizardOfWarSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void WizardOfWarSettings::step(const System& system) {

    // update the reward
    reward_t score = getDecimalScore(6, 8, &system);
    if (score >= 8000) score -= 8000; // MGB score does not go beyond 999
    score *= 100;
    m_reward = score - m_score;
    m_score = score;

    // update terminal status
    int byte1 = readRam(&system, 0xF4);
    int byte2 = readRam(&system, 0xEE);
    m_terminal = byte1 == 0xF8 && byte2 == 0x2C;
}


/* is end of game */
bool WizardOfWarSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t WizardOfWarSettings::getReward() const { 

    return m_reward; 
}


/* is an action legal */
bool WizardOfWarSettings::isLegal(const Action &a) const {

    switch (a) {
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
        case PLAYER_A_NOOP:
        case PLAYER_A_UP:
        case PLAYER_A_DOWN:
        case PLAYER_A_FIRE:
        case PLAYER_A_RIGHTFIRE:
        case PLAYER_A_LEFTFIRE:
        case PLAYER_A_UPFIRE:
        case PLAYER_A_DOWNFIRE:
            return true;
        default:
            return false;
    }   
}


/* reset the state of the game */
void WizardOfWarSettings::reset() {
    
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
}

        
/* saves the state of the rom settings */
void WizardOfWarSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void WizardOfWarSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
}

