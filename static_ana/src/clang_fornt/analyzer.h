#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <unordered_set>
#include <functional>
#include <glog/logging.h>

#include <assert.h>

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

#include "clang-c/Index.h"
#include "../lib/all.h"
#include "compiledatabase.h"
namespace fs = std::filesystem;

namespace analib
{

class ClangAnalyzer;
class clangVisitor
    : public clang::RecursiveASTVisitor<clangVisitor>
{
public:
  explicit clangVisitor(clang::ASTContext *Context, ClangAnalyzer *ana);

  // bool VisitStmt(clang::Stmt *s);

  bool VisitDecl(clang::Decl *s);

  bool TraverseDecl(clang::Decl *D);

  // bool VisitCXXRecordDecl(clang::CXXRecordDecl *Declaration)
  // {

  //   clang::FullSourceLoc FullLocation = Context->getFullLoc(Declaration->getLocStart());
  //   if (FullLocation.isValid())
  //     LOG(INFO) << "Found declaration at "
  //               << FullLocation.getSpellingLineNumber() << ":"
  //               << FullLocation.getSpellingColumnNumber() << "\n";

  //   return true;
  // }

private:
  clang::ASTContext *Context;
  ClangAnalyzer *ana_;
};

//analyzer build using clang ast
class ClangAnalyzer
{

public:
  //@param file the file you want to analyze
  //@param args the command line arguments that would be past to the clang exe, see clang_parseTranslationUnit2()
  ClangAnalyzer(const fs::path &file, const fs::path &target_dir, const std::vector<std::string> &args, const fs::path &out_path);
  ClangAnalyzer(const ClangAnalyzer &rhs);
  ~ClangAnalyzer();

  //inital the analyzer
  //this method used for source code which has a
  //source code avaliable entry point
  ANA_ERR_CODE init_main();

  //only analyze specific functions
  ANA_ERR_CODE init_function() { return ANA_ERR_CODE::SUCCESS; };

  CodeBlockGraph getCodeBlockGraph() const;

private:
  friend class clangVisitor;
  friend CXChildVisitResult namespaceVisitor(CXCursor c, CXCursor parent, CXClientData client_data);
  friend CXChildVisitResult cursorVisitorHelper(CXCursor c, CXCursor parent, CXClientData client_data);

private:
  //path for init args
  fs::path main_file_;
  //only analyze file in this dirctory
  fs::path target_dir_;
  std::vector<std::string> args_;

  //target files, programe only check the files in this container;
  StringSet target_files_;

  //entry point model only
  //the entry point function
  StringSet entry_points_;

  //global and heap memory for this analyzer
  GlobalMemory *global_memory_;
  HeapMemory *heap_memory_;

  //code graph
  CodeBlockGraph code_graph_;

  //store all declarations
  BigTable big_table_;

  //bug and error reportor
  BugReporter bug_reporter_;

  //the output path for this analysis
  fs::path out_path_;
  //compiledatabse helper
  CompileDatabaseHelper compile_database_helper_;
};
} // namespace analib
