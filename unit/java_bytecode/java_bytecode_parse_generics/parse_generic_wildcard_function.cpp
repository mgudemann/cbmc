#include <catch.hpp>

#include <util/config.h>
#include <util/cmdline.h>
#include <util/language.h>
#include <util/prefix.h>

#include <java_bytecode/java_bytecode_language.h>

#include <iostream>
#include <src/java_bytecode/load_java_class.h>

SCENARIO(
  "java_bytecode_parse_generic_wildcard",
  "[core][java_bytecode][java_bytecode_parse_generics]")
{
  const symbol_tablet &new_symbol_table=
    load_java_class(
      "WildcardGenericFunctions",
      "./java_bytecode/java_bytecode_parse_generics");

  std::string class_prefix="java::WildcardGenericFunctions";

  THEN("There should be a symbol for processSimpleGeneric")
  {
    const std::string func_name=".processSimpleGeneric";
    const std::string func_descriptor=":(LSimpleGeneric;)V";
    const std::string process_func_name=class_prefix+func_name+func_descriptor;

    REQUIRE(new_symbol_table.has_symbol(process_func_name));
  }

  THEN("There should be a symbol for processUpperBoundInterfaceGeneric")
  {
    const std::string func_name=".processUpperBoundInterfaceGeneric";
    const std::string func_descriptor=":(LSimpleGeneric;)V";
    const std::string process_func_name=class_prefix+func_name+func_descriptor;

    REQUIRE(new_symbol_table.has_symbol(process_func_name));
  }

  THEN("There should be a symbol for processUpperBoundClassGeneric")
  {
    const std::string func_name=".processUpperBoundClassGeneric";
    const std::string func_descriptor=":(LSimpleGeneric;)V";
    const std::string process_func_name=class_prefix+func_name+func_descriptor;

    REQUIRE(new_symbol_table.has_symbol(process_func_name));
  }

  THEN("There should be a symbol for processLowerBoundGeneric")
  {
    const std::string func_name=".processLowerBoundGeneric";
    const std::string func_descriptor=":(LSimpleGeneric;LFoo;)V";
    const std::string process_func_name=class_prefix+func_name+func_descriptor;

    REQUIRE(new_symbol_table.has_symbol(process_func_name));
  }
}
