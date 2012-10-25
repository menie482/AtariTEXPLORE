#include <vector>
#include <map>

/** Specification for models */
class ModelSpecification {
public:
    int modelType;
    std::vector<unsigned int> dependencies;
    std::vector<unsigned int> noPrediction;
    std::vector<unsigned int> relativeSplits;
    std::vector<unsigned int> absoluteSplits;
//  map<unsigned int, vector<unsigned int> > suggestedSplits;
};
