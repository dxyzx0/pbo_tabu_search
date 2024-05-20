//
// Created by psw on 5/20/24.
//

#include "HeuristicsRandom.h"

#include <utility>
HeurResult HeuristicsRandom::heuristic()
{
	return HEUR_NOTFIND;
}
HeuristicsRandom::HeuristicsRandom(shared_ptr< Problem > prob, shared_ptr< Settings > settings) : Heuristics(std::move(prob),
	std::move(settings))
{

}
