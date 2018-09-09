#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "statement.h"
#include "decl.h"
namespace analib
{
class TableEntry
{
public:
private:
};
//this table store all the function and class definition in
//the source code;
class BigTable
{
public:
  BigTable(){};

  void addFunction(const std::string &function_name, Decl *decl);
  void addFunction(const std::string &name_space, const std::string &function_name, Decl *decl);

  void addClass(const std::string &class_name, Decl *decl);
  void addClass(const std::string &name_space, const std::string &class_name, Decl *decl);

  Decl *getFunction(const std::string &function_name);
  Decl *getFunction(const std::string &name_space, const std::string &function_name);

private:
  typedef std::unordered_map<std::string, std::vector<Stmt *>> StmtTable;
  typedef std::unordered_map<std::string, StmtTable> NamespaceTable;
  NamespaceTable table_;
};
} // namespace analib