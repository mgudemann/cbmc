/*******************************************************************\

 Module: Unit tests for parsing generic type signatures

 Author: DiffBlue Limited. All rights reserved.

\*******************************************************************/

#include <catch.hpp>
#include <iostream>
#include <java_bytecode/java_types.h>

SCENARIO(
  "java_bytecode_parse_generic_signatures",
  "[core][java_bytecode][java_bytecode_parse_generic_signatures]")
{
  const std::string signature("Lfoo<LBar<T>;>;");

  THEN("Type parameter needs correct name")
  {
    const typet type=java_type_from_string(signature);
    REQUIRE(is_java_generic_type(type));
    const java_generic_typet &generic_type=to_java_generic_type(type);
    REQUIRE(generic_type.generic_type_variables().size()==1);
    const symbol_typet &type_var=generic_type.generic_type_variables().front().type_variable();
    std::cout << "type var " << id2string(type_var.get_identifier());

    const typet &t=generic_type.generic_type_variables().front().subtype();
    REQUIRE(is_java_generic_parameter(t));
  }
}
