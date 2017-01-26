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

size_t java_library_functionst::gensym_counter=0;

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
  typet ref_array_type=java_array_type('a');
  pointer_typet object_ref_type=to_pointer_type(ref_array_type);

  code_typet::parametert this_p(object_ref_type);
  irep_idt base_name="this";
  irep_idt id="java::array[reference].clone:()Ljava/lang/Object;::this";

  this_p.set_this();
  this_p.set_base_name(base_name);
  this_p.set_identifier(id);
  parameters.insert(parameters.begin(), this_p);

  symbol_exprt tmp_length("java::tmp_length", java_int_type());
  tmp_length.set(ID_C_base_name, "java::tmp_length");

  // get length of original array
  const dereference_exprt array(this_p, this_p.type().subtype());
  assert(this_p.type().subtype().id()==ID_symbol);
  const member_exprt length(array, "length", java_int_type());
  code.copy_to_operands(code_assignt(tmp_length, length));

  parameter_symbolt parameter_symbol;
  parameter_symbol.base_name=base_name;
  parameter_symbol.mode=ID_java;
  parameter_symbol.name=id;
  parameter_symbol.type=ref_array_type;
  symbol_table.add(parameter_symbol);

  side_effect_exprt java_new_array(ID_java_new_array, ref_array_type);
  java_new_array.copy_to_operands(tmp_length);
  java_new_array.type().subtype()
    .set(ID_C_element_type, java_type_from_string("Ljava/lang/Object;"));

  irep_idt arr_base_name="java::tmp_array1_id";
  irep_idt arr_identifier="java::tmp_array1_id";

  symbol_exprt tmp_var(arr_identifier, ref_array_type);
  tmp_var.set(ID_C_base_name, arr_base_name);

  code.copy_to_operands(code_assignt(tmp_var, java_new_array));

  // copy elements
  irep_idt iter_name="array_clone_iter";
  symbolt counter=java_gensym(iter_name, tmp_length.type());
  counter.type=tmp_length.type();
  exprt counter_expr=counter.symbol_expr();

  exprt java_zero=from_integer(0, java_int_type());
  code.copy_to_operands(code_assignt(counter_expr, java_zero));

  std::string head_name=as_string(counter.base_name)+"_header";
  code_labelt init_head_label(head_name, code_skipt());
  code_gotot goto_head(head_name);

  code.copy_to_operands(init_head_label);

  std::string done_name=as_string(counter.base_name)+"_done";
  code_labelt init_done_label(done_name, code_skipt());
  code_gotot goto_done(done_name);

  code_ifthenelset done_test;
  done_test.cond()=equal_exprt(counter_expr, tmp_length);
  done_test.then_case()=goto_done;

  code.copy_to_operands(done_test);

  exprt java_one=from_integer(1, java_int_type());
  code_assignt incr(counter_expr, plus_exprt(counter_expr, java_one));

  code.copy_to_operands(incr);
  code.copy_to_operands(goto_head);
  code.copy_to_operands(init_done_label);

  code.copy_to_operands(code_returnt(
    typecast_exprt(
      tmp_var,
      java_type_from_string("Ljava/lang/Object;"))));

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


symbolt java_library_functionst::java_gensym(irep_idt &prefix, typet &type)
{
  std::string new_name=
    "java::"+id2string(prefix)+"_"+std::to_string(gensym_counter);

  symbolt new_symbol;
  new_symbol.base_name=new_name;
  new_symbol.name=new_name;
  new_symbol.type=type;
  new_symbol.mode=ID_java;
  gensym_counter++;

  assert(!symbol_table.has_symbol(new_name) &&
         "symbol must be unique");
  symbol_table.add(new_symbol);

  return new_symbol;
}
