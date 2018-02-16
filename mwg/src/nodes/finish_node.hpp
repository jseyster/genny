#include "node.hpp"

#pragma once
using namespace std;

namespace mwg {

class finishNode : public node {
public:
    finishNode() {
        name = "Finish";
    };
    finishNode(YAML::Node&) {
        name = "Finish";
    };
    virtual ~finishNode() = default;
    finishNode(const finishNode&) = default;
    finishNode(finishNode&&) = default;
    // Execute the finishNode
    virtual void executeNode(shared_ptr<threadState>) override;
    const string getName() {
        return "Finish";
    };

    // The finish node never has a next pointer
    virtual void setNextNode(unordered_map<string, node*>&, vector<shared_ptr<node>>&) override{};
    virtual std::pair<std::string, std::string> generateDotGraph() override;
};
}