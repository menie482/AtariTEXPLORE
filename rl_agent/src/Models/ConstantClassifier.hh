/** \file ConstantClassifier.hh
    Defines a classifier that predicts no change for a value
    \author Chris Donahue
*/

#ifndef _CONSTANTCLASSIFIER_HH_
#define _CONSTANTCLASSIFIER_HH_

#include <rl_common/core.hh>
#include <vector>
#include <map>

/** Constant Classifier class */
class ConstantClassifier: public Classifier {

public:
  /** Default constructor 
      \param id id of the tree for debug
  */
  ConstantClassifier(int id);

  /** Copy constructor. */
  ConstantClassifier(const ConstantClassifier&);

  virtual ConstantClassifier* getCopy();

  ~ConstantClassifier();

  virtual bool trainInstance(classPair &instance);
  virtual bool trainInstances(std::vector<classPair> &instances);
  virtual void testInstance(const std::vector<float> &input, std::map<float, float>* retval);
  virtual float getConf(const std::vector<float> &input);

private:
  const int id;
};

#endif
