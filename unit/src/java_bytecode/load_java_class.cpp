//
// Created by thomas on 29/09/17.
//

#include <catch.hpp>
#include "load_java_class.h"

#include <util/language.h>
#include <util/message.h>
#include <java_bytecode/java_bytecode_language.h>
#include <iostream>

#include "util/suffix.h"
#include "util/invariant.h"

symbol_tablet load_java_class(
  const std::string java_class_name,
  const std::string &class_path)
{
  // We don't expect the .class suffix to allow us to check the name of the
  // class
  PRECONDITION(!has_suffix(java_class_name, ".class"));

  // Configure the path loading
  cmdlinet command_line;
  command_line.set("java-cp-include-files", class_path);
  config.java.classpath.clear();
  config.java.classpath.push_back(class_path);

  symbol_tablet new_symbol_table;

  std::unique_ptr<languaget>java_lang(new_java_bytecode_language());

  std::istringstream java_code_stream("ignored");
  null_message_handlert message_handler;

  // Configure the language, load the class files
  java_lang->get_language_options(command_line);
  java_lang->set_message_handler(message_handler);
  java_lang->parse(java_code_stream, java_class_name + ".class");
  java_lang->typecheck(new_symbol_table, "");
  java_lang->final(new_symbol_table);

  // Verify that the class was loaded
  const std::string class_symbol_name="java::"+java_class_name;
  REQUIRE(new_symbol_table.has_symbol(class_symbol_name));
  const symbolt &class_symbol=new_symbol_table.lookup(class_symbol_name);
  REQUIRE(class_symbol.is_type);
  const typet &class_type=class_symbol.type;
  REQUIRE(class_type.id()==ID_struct);

  // if this fails it indicates the class was not loaded
  // Check your working directory and the class path is correctly configured
  // as this often indicates that one of these is wrong.
  REQUIRE_FALSE(class_type.get_bool(ID_incomplete_class));
  return new_symbol_table;
}
