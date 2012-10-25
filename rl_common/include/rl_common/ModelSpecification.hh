#include <rl_common/core.hh>
#include <vector>
#include <map>

/** Specification for models */
class ModelSpecification {
public:
    ModelSpecification(int modelType);
    ~ModelSpecification();

    int modelType;
    vector<unsigned int> dependencies;
    vector<unsigned int> noPrediction;
    vector<unsigned int> relativeSplits;
    vector<unsigned int> absoluteSplits;
//    map<unsigned int, vector<unsigned int> > suggestedSplits;
};
