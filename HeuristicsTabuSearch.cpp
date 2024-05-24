//
// Created by psw on 5/21/24.
//

#include "HeuristicsTabuSearch.h"
#include <ranges>
#include <spdlog/spdlog.h>

HeurResult HeuristicsTabuSearch::heuristic() {
    IntegerType bestObj = set->bestObj;
    shared_ptr<IntVec> bestSol = set->bestSol;
    HeurResult res = HeurResult::HEUR_NOTFIND;
    for (long i = 0; i < nTryPerRound; i++) {
        spdlog::info("(*) Tabu search try {}", i);
        long nVar = prob->getNVar();
        shared_ptr<IntVec> x = gen_rnd_vec(nVar, nVar / 2);
        auto [Ax_b_ineq, Ax_b_eq] = calAxb(*x);
        auto score_x = score(*Ax_b_ineq, *Ax_b_eq);

        IntegerType best_score = score_x;
        shared_ptr<IntVec> best_x = x;

        // record consecutive non-improving iterations
        long nConsecutiveNonImproving = 0;

        spdlog::info("(*) Initial score = {}", score_x.get_str());
        for (long j = 0; j < nTabuIterMax; j++) {
            spdlog::info("(**) Tabu search iteration {}", j, " score = {}", score_x.get_str());
            if (isFeasible(*Ax_b_ineq, *Ax_b_eq)) {
                for (long l = 0; l < nVar; l++) {
                    spdlog::info("(**) {}", (*x)[l].get_str());
                }
                auto crntObj = prob->getObj(*x);
                spdlog::info("(**) Found a feasible solution with obj = {}", crntObj.get_str());
                if (crntObj < bestObj) {
                    spdlog::info("(**) Found a better solution with obj = {}", crntObj.get_str());
                    bestObj = crntObj;
                    // update best solution using a copy of x
                    set->bestSol = make_shared<IntVec>(*x);
                    res = HeurResult::HEUR_FINDBESTSOL;
                    cout << "Found a better solution with obj = " << bestObj << endl;
                } else {
                    res = res == HeurResult::HEUR_FINDBESTSOL ? res : HeurResult::HEUR_FINDBADSOL;
                }
            }
            spdlog::info("(**) Infeasible solution, start local search");
            auto best_score_j = score_x;
            long best_k = -1;
            for (long k = 0; k < nVar; k++) {
                auto [score_k, Ax_b_ineq_k, Ax_b_eq_k] = score_with_info(k, *x, *Ax_b_ineq, *Ax_b_eq);
//                spdlog::info("(**) Move {} score = {}", k, score_k.get_str());
                if ((score_k > best_score_j && tabuList[k] <= j) || (score_k > best_score)) {  // greedy
//                    spdlog::info("(**) Move {} is better", k);
                    best_score_j = score_k;
                    best_k = k;
                    Ax_b_ineq = Ax_b_ineq_k;
                    Ax_b_eq = Ax_b_eq_k;

                    if (score_k > best_score) {
                        spdlog::info("(**) Move {} is the current best with score = {}", k, score_k.get_str());
                        best_score = score_k;
                        best_x = make_shared<IntVec>(*x);
                    }
                }
            }
            spdlog::info("(**) Best move {} score = {}", best_k, best_score_j.get_str());
            if (best_k != -1) {
                spdlog::info("(**) Apply move {}", best_k);
                (*x)[best_k] = 1 - (*x)[best_k];
                tabuList[best_k] = j + nTabuTenure;
                score_x = best_score_j;
            } else {
                // randomly select a move
                nConsecutiveNonImproving++;
                nConsecutiveNonImproving = nConsecutiveNonImproving > nVar ? nVar : nConsecutiveNonImproving;

                // select nConsecutiveNonImproving moves randomly
                for (int l = 0; l < nConsecutiveNonImproving; l++) {
                    long k = rand() % nVar;
                    spdlog::info("(**) Randomly select move {}", k);
                    (*x)[k] = 1 - (*x)[k];
                    tabuList[k] = j + nTabuTenure;
                    auto [score_k, Ax_b_ineq_k, Ax_b_eq_k] = score_with_info(k, *x, *Ax_b_ineq, *Ax_b_eq);
                    score_x = score_k;
                    Ax_b_ineq = Ax_b_ineq_k;
                    Ax_b_eq = Ax_b_eq_k;
                    spdlog::info("(**) Randomly select move {} score = {}", k, score_k.get_str());
                }
            }

        }
//		long hash = move->at(0) * prob->nVar + move->at(1);
//		if (tabuList.find(hash) == tabuList.end() || tabuList[hash] < i)
//		{
//			HeurResult res = prob->evaluate_move(move);
//			if (res.obj < prob->objVal)
//			{
//				prob->update_move(move);
//				prob->objVal = res.obj;
//				tabuList[hash] = i + nTabuTenure;
//			}
//		}
    }

    return res;
}

tuple<shared_ptr<IntVec>, shared_ptr<IntVec> > HeuristicsTabuSearch::calAxb(IntVec &x) {
    auto A_ineq = prob->getA_ineq();
    auto A_eq = prob->getA_eq();

    auto b_ineq = prob->getB_ineq();
    auto b_eq = prob->getB_eq();

    auto Ax_b_ineq = make_shared<IntVec>((*A_ineq) * x - (*b_ineq));
    auto Ax_b_eq = make_shared<IntVec>((*A_eq) * x - (*b_eq));

    return make_tuple(Ax_b_ineq, Ax_b_eq);
}

bool HeuristicsTabuSearch::isFeasible(IntVec &Ax_b_ineq, IntVec &Ax_b_eq) {
    // test if Ax_b_ineq >= 0 and Ax_b_eq == 0
    if (!ranges::all_of(Ax_b_eq, [](auto i) { return i == 0; })) {
        return false;
    }
    if (!ranges::all_of(Ax_b_ineq, [](auto i) { return i >= 0; })) {
        return false;
    }
    return true;
}

IntegerType HeuristicsTabuSearch::infeasibility(IntVec &Ax_b_ineq, IntVec &Ax_b_eq) {
    IntegerType infeasibility = 0;
    for (const auto &i: Ax_b_ineq) {
        if (i < 0) {
            infeasibility += 1;
        }
    }
    for (const auto &i: Ax_b_eq) {
        if (i != 0) {
            infeasibility += 1;
        }
    }
    return infeasibility;
}

IntegerType HeuristicsTabuSearch::score(long &i, IntVec &x, IntVec &Ax_b_ineq, IntVec &Ax_b_eq) {
    auto col_ineq_i = prob->getA_ineq()->col(i) * (1 - 2 * (x[i] == 1));
    auto col_eq_i = prob->getA_eq()->col(i) * (1 - 2 * (x[i] == 1));
    auto Ax_b_ineq_i = (Ax_b_ineq + col_ineq_i).toDense();
    auto Ax_b_eq_i = (Ax_b_eq + col_eq_i).toDense();
    return -infeasibility(Ax_b_ineq_i, Ax_b_eq_i);
}

tuple<IntegerType, shared_ptr<IntVec>, shared_ptr<IntVec> >
HeuristicsTabuSearch::score_with_info(long &i, IntVec &x, IntVec &Ax_b_ineq, IntVec &Ax_b_eq) {
    auto col_ineq_i = prob->getA_ineq()->col(i) * (1 - 2 * (x[i] == 1));
    auto col_eq_i = prob->getA_eq()->col(i) * (1 - 2 * (x[i] == 1));
    auto score_i = score(i, x, Ax_b_ineq, Ax_b_eq);
    auto Ax_b_ineq_i = make_shared<IntVec>(std::move((Ax_b_ineq + col_ineq_i).toDense()));
    auto Ax_b_eq_i = make_shared<IntVec>(std::move((Ax_b_eq + col_eq_i).toDense()));
    return make_tuple(score_i, Ax_b_ineq_i, Ax_b_eq_i);
}


IntegerType HeuristicsTabuSearch::score(IntVec &Ax_b_ineq, IntVec &Ax_b_eq) {
    return -infeasibility(Ax_b_ineq, Ax_b_eq);
}

HeuristicsTabuSearch::HeuristicsTabuSearch(shared_ptr<Problem> prob, shared_ptr<Settings> settings) : HeuristicsRandom(
        prob, settings) {
    for (long i = 0; i < prob->getNVar(); i++) {
        tabuList[i] = 0;
    }
}
