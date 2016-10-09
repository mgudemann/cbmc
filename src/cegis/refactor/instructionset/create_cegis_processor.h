/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseli@cs.ox.ac.uk

 \*******************************************************************/

#ifndef CEGIS_REFACTOR_INSTRUCTIONSET_CREATE_CEGIS_PROCESSOR_H_
#define CEGIS_REFACTOR_INSTRUCTIONSET_CREATE_CEGIS_PROCESSOR_H_

#include <set>

#include <util/type.h>
#include <goto-programs/goto_functions.h>

#include <cegis/cegis-util/goto_range.h>

/**
 * @brief
 *
 * @details
 *
 * @param range
 *
 * @return
 */
std::set<typet> collect_context_types(const goto_ranget &range);

/**
 * @brief
 *
 * @details
 *
 * @param st
 * @param gf
 * @param state_types
 * @param var_slots_per_state_type
 * @param context_types
 *
 * @return
 */
std::string create_cegis_processor(
    symbol_tablet &st,
    goto_functionst &gf,
    const std::set<typet> &state_types,
    size_t var_slots_per_state_type,
    const std::set<typet> &context_types);

#endif /* CEGIS_REFACTOR_INSTRUCTIONSET_CREATE_CEGIS_PROCESSOR_H_ */