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
  code_typet &code_type=to_code_type(symbol.type);
  code_typet::parameterst &parameters(code_type.parameters());

  pointer_typet ref_array_type=java_array_type('a');
  typet enumtest_type=java_type_from_string("Lenumtest;");
  pointer_typet object_ref_type=to_pointer_type(ref_array_type);
  ref_array_type.subtype().set(ID_C_element_type, enumtest_type);
  std::cout << "INFO: ref_array_type subtype "
            << ref_array_type.subtype().pretty()
            << std::endl;

  code_typet::parametert &this_p=parameters[0];
  irep_idt base_name="this";
  irep_idt id="java::array[reference].clone:()Ljava/lang/Object;::this";

  this_p.set_base_name(base_name);
  this_p.set_identifier(id);
  if(!this_p.get_this())
    throw "must be this";

  symbolt new_this;
  new_this.base_name=base_name;
  new_this.name=id;
  new_this.type=object_ref_type;
  new_this.mode=ID_java;
  new_this.value=this_p;
  symbol_table.add(new_this);

  irep_idt tmp_length_name="tmp_length";
  typet java_int=java_int_type();
  typet java_object=java_type_from_string("Ljava/lang/Object;");
  symbolt tmp_length=java_gensym(tmp_length_name, java_int);
  symbol_exprt tmp_length_expr=tmp_length.symbol_expr();

  // get length of original array
  const dereference_exprt
    array(new_this.symbol_expr(), new_this.symbol_expr().type().subtype());
  assert(this_p.type().subtype().id()==ID_symbol);
  const member_exprt length(array, "length", java_int_type());

  side_effect_exprt java_new_array(ID_java_new_array, ref_array_type);
  java_new_array.copy_to_operands(tmp_length_expr);
  java_new_array.type().subtype()
    .set(ID_C_element_type, java_array_type('a'));

  std::cout << "INFO: ref_array_pointer is " << ref_array_type.id()
            << ref_array_type.subtype().pretty()
            << std::endl;

  irep_idt arr_base_name="java::tmp_array1_id";
  irep_idt arr_identifier="java::tmp_array1_id";

  symbol_exprt tmp_var(arr_identifier, ref_array_type);
  tmp_var.set(ID_C_base_name, arr_base_name);

  // copy elements
  irep_idt iter_name="array_clone_iter";
  symbolt counter=java_gensym(iter_name, java_int);
  exprt counter_expr=counter.symbol_expr();

  exprt java_zero=from_integer(0, java_int_type());

  std::string head_name=as_string(counter.base_name)+"_header";
  code_labelt init_head_label(head_name, code_skipt());
  code_gotot goto_head(head_name);

  std::string done_name=as_string(counter.base_name)+"_done";
  code_labelt init_done_label(done_name, code_skipt());
  code_gotot goto_done(done_name);

  code_ifthenelset done_test;
  done_test.cond()=equal_exprt(counter_expr, tmp_length_expr);
  done_test.then_case()=goto_done;

  exprt java_one=from_integer(1, java_int_type());
  code_assignt incr(counter_expr, plus_exprt(counter_expr, java_one));

  // access source array
  exprt source_array_data=
    member_exprt(
      array,
      "data",
      ref_array_type);

  exprt target_array_data=
    member_exprt(
      dereference_exprt(tmp_var, tmp_var.type().subtype()),
      "data",
      ref_array_type);

  irep_idt source_arr_name="source_arr";
  symbolt source_array=
    java_gensym(source_arr_name, java_reference_type(java_object));
  const symbol_exprt &source_array_expr=source_array.symbol_expr();

  irep_idt target_arr_name="target_arr";
  symbolt target_array=
    java_gensym(target_arr_name, target_array_data.type());
  exprt target_array_expr=
    typecast_exprt(target_array_expr, pointer_typet(enumtest_type));
//
  // if(init_array_expr.type()!=pointer_typet(element_type))
  // init_array_expr=



  std::cout << "INFO: target_array_expr "
            << target_array_expr.pretty()
            << std::endl;

  exprt source_cell=dereference_exprt(
    plus_exprt(source_array_expr, counter_expr, source_array_data.type()),
    source_array_data.type().subtype());

  exprt target_cell=dereference_exprt(
    plus_exprt(target_array_expr, counter_expr, target_array_data.type()),
    target_array_data.type().subtype());

  irep_idt tmp_elem_name="tmp_element";
  symbolt tmp_elem=java_gensym(tmp_elem_name, java_object);
  const symbol_exprt &tmp_elem_expr=tmp_elem.symbol_expr();

  code_blockt code;
  code.copy_to_operands(code_assignt(tmp_length_expr, length));
  code.copy_to_operands(code_assignt(tmp_var, java_new_array));
  code.copy_to_operands(code_assignt(counter_expr, java_zero));
  code.copy_to_operands(code_assignt(target_array_expr, target_array_data));
  code.copy_to_operands(init_head_label);
  code.copy_to_operands(done_test);
  // code.copy_to_operands(code_assignt(tmp_elem_expr, source_cell));
  // code.copy_to_operands(code_assignt(target_cell, tmp_elem_expr));
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

symbolt java_library_functionst::java_gensym(irep_idt &prefix, const typet &type)
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
