#ifndef EXPERIENCE_HEURISTIC_H
#define EXPERIENCE_HEURISTIC_H

#include "heuristic.h"
#include "state_id.h"

class ExperienceHeuristic : public Heuristic {
protected:
    Heuristic* base_heuristic;
    std::vector<StateID> e_nodes;
    std::vector<int> hE;
    virtual void initialize();
    virtual int compute_heuristic(const State &state);
public:
    ExperienceHeuristic(const Options &options);
    ~ExperienceHeuristic();
};

#endif
