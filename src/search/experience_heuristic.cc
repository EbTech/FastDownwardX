#include "experience_heuristic.h"

#include "globals.h"
#include "option_parser.h"
#include "plugin.h"
#include "state.h"

#include "state_registry.h"
static const int EPS = 2;
static const int MAX_COST_VALUE = 100000000;

ExperienceHeuristic::ExperienceHeuristic(const Options &opts)
    : Heuristic(opts) {
}

ExperienceHeuristic::~ExperienceHeuristic() {
}

void ExperienceHeuristic::initialize() {
    cout << "Initializing experience graph heuristic..." << endl;
    hE = std::vector<int>(123, MAX_COST_VALUE);
    std::vector<std::vector<int> > dist(hE.size(), std::vector<int>(hE.size(), MAX_COST_VALUE));
    std::vector<bool> visited(hE.size(), false);
    hE[0] = 0;
    int expand = -1;
    while (expand != -1) {
        visited[expand] = true;
        for (int i = 0; i < hE.size(); i++) {
	    hE[i] = min(hE[i], hE[expand] + dist[expand][i]); // only if !visited[i], forget dist and directly call base_heuristic->compute_heuristic(expand, i)
        }
        expand = -1;
	for (int i = 0; i < hE.size(); i++) {
	    if (!visited[i] && (expand == -1 || hE[i] < hE[expand])) {
	        expand = i;
	    }
        }
    }
}

int ExperienceHeuristic::compute_heuristic(const State &state) {
    int h = state.get_id().hash(); // once virtual functions are resolved, change to: base_heuristic->compute_heuristic(state);
    std::vector<std::pair<int,int> > real_goal = g_goal;
    for (int i = 0; i < e_nodes.size(); i++) {
	State e_state = g_state_registry->lookup_state(e_nodes[i]);
	for (int j = 0; j < g_goal.size(); j++) {
	    g_goal[j].second = e_state[j];
	}
        //h = min(h, base_heuristic->compute_heuristic(e_state) + hE[i]);
    }
    g_goal = real_goal;
    return h;
}

static Heuristic *_parse(OptionParser &parser) {
    parser.document_synopsis("Goal count heuristic", "");
    parser.document_language_support("action costs", "supported");
    parser.document_language_support("conditional effects", "supported");
    parser.document_language_support("axioms", "supported");
    parser.document_property("admissible", "no");
    parser.document_property("consistent", "no");
    parser.document_property("safe", "yes");
    parser.document_property("preferred operators", "no");

    Heuristic::add_options_to_parser(parser);
    Options opts = parser.parse();
    if (parser.dry_run())
        return 0;
    else
        return new ExperienceHeuristic(opts);
}


static Plugin<Heuristic> _plugin("experience", _parse);
