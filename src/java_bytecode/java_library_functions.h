/*******************************************************************\

Module: JAVA Bytecode Library Functions

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_JAVA_BYTECODE_JAVA_LIBRARY_FUNCTIONS_H
#define CPROVER_JAVA_BYTECODE_JAVA_LIBRARY_FUNCTIONS_H

#include <util/irep.h>
#include <util/symbol_table.h>

class java_library_functionst
{
  const std::string array_clone_id=
    "java::array[reference].clone:()Ljava/lang/Object;";

  symbol_tablet &symbol_table;
  bool is_stub_array_clone(const irep_idt &);
 public:
  explicit java_library_functionst(symbol_tablet &_symbol_table) :
  symbol_table(_symbol_table)
  {
  }

  void implement_array_clone();
  void implement_stubbed_library_functions();
};


#endif // CPROVER_JAVA_BYTECODE_JAVA_LIBRARY_FUNCTIONS_H
