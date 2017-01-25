/*******************************************************************\

Module: JAVA Bytecode Library Functions

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include <iostream>

#include <util/arith_tools.h>
#include <util/suffix.h>
#include <util/prefix.h>

#include <util/expr_util.h>
#include <util/std_code.h>
#include <util/std_expr.h>
#include <util/std_types.h>

#include "java_types.h"
#include "java_library_functions.h"

void java_library_functionst::implement_array_clone()
{
  symbolt &symbol=symbol_table.lookup(array_clone_id);
  std::cout << "INFO: change " << symbol.name
            << " type " << symbol.type.id()
            << std::endl;

  assert(symbol.type.id()==ID_code);
  code_blockt code;
  code_typet &code_type=to_code_type(symbol.type);
  code_typet::parameterst &parameters(code_type.parameters());
  parameters.resize(0);
  typet type=java_type_from_string("[Ljava/lang/Object;");
  pointer_typet object_ref_type(type);

  const typet &return_type=java_type_from_string("[Lenumtest;");

  std::cout << "INFO: return type " << code_type.return_type().id()
            << std::endl;

  code_typet::parametert this_p(object_ref_type);
  irep_idt base_name="this";
  irep_idt id="java::array[reference].clone:()Ljava/lang/Object::this";

  this_p.set_this();
  this_p.set_base_name(base_name);
  this_p.set_identifier(id);
  parameters.insert(parameters.begin(), this_p);

  parameter_symbolt parameter_symbol;
  parameter_symbol.base_name=base_name;
  parameter_symbol.mode=ID_java;
  parameter_symbol.name=id;
  parameter_symbol.type=type;
  symbol_table.add(parameter_symbol);

  symbolt length_sym;
  length_sym.name="array-length";
  length_sym.base_name="array-length";
  length_sym.mode=ID_java;
  symbol_table.add(length_sym);
  length_sym.type=java_int_type();
  constant_exprt enum_entries=from_integer(3, java_int_type());

  const auto &length_sym_expr=length_sym.symbol_expr();

  const pointer_typet ref_type=
    java_array_type('a');

  side_effect_exprt java_new_array(ID_java_new_array, ref_type);
  java_new_array.copy_to_operands(enum_entries);
  java_new_array.type().subtype()
    .set(ID_C_element_type, java_type_from_string("Lenumtest;"));

  irep_idt arr_base_name="tmp_array1_id";
  irep_idt arr_identifier="tmp_array1_id";

  auxiliary_symbolt tmp_symbol;
  tmp_symbol.base_name=arr_base_name;
  tmp_symbol.is_static_lifetime=false;
  tmp_symbol.mode=ID_java;
  tmp_symbol.name=arr_identifier;
  tmp_symbol.type=ref_type;
  symbol_table.add(tmp_symbol);

  symbol_exprt tmp_var(arr_identifier, ref_type);
  tmp_var.set(ID_C_base_name, arr_base_name);

  code.copy_to_operands(code_assignt(tmp_var, java_new_array));
  code.copy_to_operands(code_returnt(tmp_var));

  symbol.value=code;
}

bool java_library_functionst::is_stub_array_clone(const irep_idt &method_name)
{
  if(method_name==array_clone_id)
  {
    const symbolt &symbol=symbol_table.lookup(method_name);
    assert(symbol.type.id()==ID_code && "method does not have code type");
    return symbol.value.id()==ID_nil;
  }
  return false;
}

void java_library_functionst::implement_stubbed_library_functions()
{
  for(auto &symb : symbol_table.symbols)
  {
    if(is_stub_array_clone(symb.first))
      implement_array_clone();
  }
}
