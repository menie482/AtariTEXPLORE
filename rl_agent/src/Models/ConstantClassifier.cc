/** \file ConstantClassifier.cc
    Implements a classifier that predicts no change for a value
    \author Chris Donahue
*/

#include "ConstantClassifier.hh"

ConstantClassifier::ConstantClassifier(int id):
  id(id)
{
    cout << "Created constant classifier " << id << endl;
}

ConstantClassifier::ConstantClassifier(const ConstantClassifier& cc):
  id(cc.id)
{
    cout << " Constant classifier copy constructor id " << id << endl;
}

ConstantClassifier* ConstantClassifier::getCopy(){
  ConstantClassifier* copy = new ConstantClassifier(*this);
  return copy;
}

ConstantClassifier::~ConstantClassifier() {
}

// here the target output will be a single value
bool ConstantClassifier::trainInstance(classPair &instance){
}


// here the target output will be a single value
bool ConstantClassifier::trainInstances(std::vector<classPair> &instances){
}

// Here we want to return the probability of the output value being each of the possible values, in the stochastic case
void ConstantClassifier::testInstance(const std::vector<float> &input, std::map<float, float>* retval){
  //cout << "Input: " << input[0] << ", output: " << input[0] << " = 1.0" << endl;
  (*retval)[input[0]] = 1.0;
  return;
}

float ConstantClassifier::getConf(const std::vector<float> &input){
  return 1.0;
}


