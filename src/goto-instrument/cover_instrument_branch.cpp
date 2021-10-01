/*******************************************************************\

Module: Coverage Instrumentation

Author: Daniel Kroening

\*******************************************************************/

/// \file
/// Coverage Instrumentation for Branches

#include "cover_basic_blocks.h"
#include "cover_filter.h"
#include "cover_instrument.h"

#include <typeinfo>
#include <iostream>
#include <string>

void cover_branch_instrumentert::instrument(
  const irep_idt &function_id,
  goto_programt &goto_program,
  goto_programt::targett &i_it,
  const cover_blocks_baset &basic_blocks,
  const assertion_factoryt &make_assertion) const
{
  if(is_non_cover_assertion(i_it))
    i_it->turn_into_skip();

  const bool is_function_entry_point =
    i_it == goto_program.instructions.begin();
  const bool is_conditional_goto = i_it->is_goto() && !i_it->guard.is_true();
  if(!is_function_entry_point && !is_conditional_goto)
    return;

  if(!goal_filters(i_it->source_location))
    return;

  if(is_function_entry_point)
  {
    // we want branch coverage to imply 'entry point of function'
    // coverage
    std::string comment = "entry point";

    source_locationt source_location = i_it->source_location;

    goto_programt::targett t = goto_program.insert_before(
      i_it, make_assertion(false_exprt(), source_location));
    initialize_source_location(t, comment, function_id);
  }

  if(is_conditional_goto)
  {
    std::string b =
      std::to_string(basic_blocks.block_of(i_it) + 1); // start with 1
    std::string true_comment = "block " + b + " branch true";
    std::string false_comment = "block " + b + " branch false";

    const std::string& name = typeid(basic_blocks).name();
    const std::string& java_name = "cover_basic_blocks_javat";

    const bool is_java =
      0 == name.compare(name.size() - java_name.size(), java_name.size(), java_name);

    if (is_java) {
      exprt guard = i_it->guard;

      const cover_basic_blocks_javat &java_basic_blocks =
        dynamic_cast<const cover_basic_blocks_javat &>(basic_blocks);
      // man müsste hier bei Java den bytecode index des jeweiligen targets nehmen
      // und der nächsten GOTO instruction

      std::size_t target_block_nr = java_basic_blocks.block_of(i_it->targets.front());
      std::size_t next_block_nr = java_basic_blocks.block_of(std::next(i_it));

      std::cout << "target " << i_it->targets.front()->location_number
                << " block number " << target_block_nr
                << " block location " << java_basic_blocks.source_location_of(target_block_nr).as_string()
                << std::endl;
      std::cout << "next " << std::next(i_it)->location_number
                << " block number " << next_block_nr
                << " block location " << java_basic_blocks.source_location_of(next_block_nr).as_string()
                << std::endl;


      source_locationt source_location = i_it->source_location;

      goto_program.insert_before_swap(i_it);
      *i_it = make_assertion(not_exprt(guard), source_location);
      initialize_source_location(i_it, true_comment, function_id);

      goto_program.insert_before_swap(i_it);
      *i_it = make_assertion(guard, source_location);
      initialize_source_location(i_it, false_comment, function_id);

      std::advance(i_it, 2);
    } else {

      exprt guard = i_it->guard;
      source_locationt source_location = i_it->source_location;

      goto_program.insert_before_swap(i_it);
      *i_it = make_assertion(not_exprt(guard), source_location);
      initialize_source_location(i_it, true_comment, function_id);

      goto_program.insert_before_swap(i_it);
      *i_it = make_assertion(guard, source_location);
      initialize_source_location(i_it, false_comment, function_id);

      std::advance(i_it, 2);
    }
  }
}
