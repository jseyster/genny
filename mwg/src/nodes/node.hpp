#include <atomic>
#include <memory>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <random>
#include <string>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

#include "stats.hpp"
#include "threadState.hpp"

#pragma once
using namespace std;

namespace mwg {

class node {
public:
    node() : stopped(false){};
    node(YAML::Node&);
    virtual ~node() = default;
    // do we use the copy constructor at all?
    node(const node& other) : nextNode(other.nextNode), stopped(false), text(other.text) {
        myStats.reset();
    };
    node(node&&) = default;
    // Execute the node
    virtual void executeNode(shared_ptr<threadState>);
    virtual void executeNextNode(shared_ptr<threadState>);
    virtual void execute(shared_ptr<threadState>){};
    const string getName() {
        return name;
    };

    virtual void stop();
    // Set the next node pointer
    virtual void setNextNode(unordered_map<string, node*>&, vector<shared_ptr<node>>&);
    string name;
    string nextName;
    // Generate a dot file for generating a graph.
    virtual std::pair<std::string, std::string> generateDotGraph();
    virtual void logStats();                                    // print out the stats
    virtual bsoncxx::document::value getStats(bool withReset);  // bson object with stats
    virtual void resetStats() {
        myStats.reset();
    };
    virtual void recordMicros(std::chrono::microseconds time) {
        myStats.recordMicros(time);
    }
    virtual void recordException() {
        myStats.recordException();
    }
    // convenience for testing
    int getCount() {
        return (myStats.getCount());
    }

protected:
    node* nextNode;
    std::atomic<bool> stopped;
    string text;
    Stats myStats;
};

unique_ptr<node> makeUniqeNode(YAML::Node);
shared_ptr<node> makeSharedNode(YAML::Node);
}