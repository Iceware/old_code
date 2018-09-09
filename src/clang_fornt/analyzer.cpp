#include "analyzer.h"
namespace analib
{
#ifdef EXECUTEABLE_NAME
#else
#define EXECUTEABLE_NAME "exe"
#endif
//-----------------------------------------------------------------

clangVisitor::clangVisitor(clang::ASTContext *Context, ClangAnalyzer *ana)
    : Context(Context), ana_(ana)
{
  // std::cout << "new visitor" << std::endl;
  assert(ana_ != nullptr);
}

// bool clangVisitor::VisitStmt(clang::Stmt *s)
// {
//   clang::FullSourceLoc FullLocation = Context->getFullLoc(s->getLocStart());
//   if (FullLocation.isValid() && FullLocation.getFileEntry() != nullptr)
//   {
//     std::string name = FullLocation.getFileEntry()->getName();
//     if (ana_->target_files_.find(name) != ana_->target_files_.end())
//       std::cout << "Found Stmt at "
//                 << name << " "
//                 << FullLocation.getSpellingLineNumber() << ":"
//                 << FullLocation.getSpellingColumnNumber() << "\n";
//   }
//   return true;
// }

bool clangVisitor::TraverseDecl(clang::Decl *D)
{
  // std::cout << "TraverseDecl\n";
  // RecursiveASTVisitor<clangVisitor>::TraverseDecl(D);
  //the default method will visit the ast tree recursively
  VisitDecl(D);
  return true; //return false avoid dig in
}

bool clangVisitor::VisitDecl(clang::Decl *decl)
{
  // if (clang::NamespaceDecl::classof(decl))
  // {
  //   ana_->
  // }

  clang::FullSourceLoc FullLocation = Context->getFullLoc(decl->getLocStart());
  if (FullLocation.isValid() && FullLocation.getFileEntry() != nullptr)
  {
    std::string name = FullLocation.getFileEntry()->getName();
    if (ana_->target_files_.find(name) != ana_->target_files_.end())
    {
      std::cout << "Found Decl at "
                << name << " "
                << FullLocation.getSpellingLineNumber() << ":"
                << FullLocation.getSpellingColumnNumber() << "\n";
      decl->dumpColor();
    }
  }
  return true;
}
//-----------------------------------------------------------------

namespace
{
// static llvm::cl::OptionCategory clangToolingCategory("static ana");
//---------------------------------------------------------------------------------------
class clangConsumer : public clang::ASTConsumer
{
public:
  explicit clangConsumer(clang::ASTContext *Context, ClangAnalyzer *ana)
      : Visitor(Context, ana) {}

  // virtual void HandleTranslationUnit(clang::ASTContext &Context)
  // {
  //   Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  // }
  virtual bool HandleTopLevelDecl(clang::DeclGroupRef d) override
  {
    typedef clang::DeclGroupRef::iterator iter;
    for (iter b = d.begin(), e = d.end(); b != e; ++b)
    {
      Visitor.TraverseDecl(*b);
    }
    return true; // keep going
  }

private:
  clangVisitor Visitor;
};

class clangAction : public clang::ASTFrontendAction
{
public:
  clangAction(ClangAnalyzer *ana) : ana_(ana)
  {
    assert(ana_ != nullptr);
  }

  virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
      clang::CompilerInstance &Compiler, llvm::StringRef InFile)
  {
    return std::unique_ptr<clang::ASTConsumer>(
        new clangConsumer(&Compiler.getASTContext(), ana_));
  }

private:
  ClangAnalyzer *ana_;
};

template <typename T>
std::unique_ptr<clang::tooling::FrontendActionFactory> newFactory(ClangAnalyzer *ana)
{
  class SimpleFrontendActionFactory : public clang::tooling::FrontendActionFactory
  {
  public:
    SimpleFrontendActionFactory(ClangAnalyzer *ana) : ana_(ana) {}
    clang::FrontendAction *create() override { return new T(ana_); }

  private:
    ClangAnalyzer *ana_;
  };

  return std::unique_ptr<clang::tooling::FrontendActionFactory>(
      new SimpleFrontendActionFactory(ana));
}

//---------------------------------------------------------------------------------------
void dumpDirectory(const fs::path &target_dir, std::unordered_set<std::string> &result)
{
  for (auto &p : fs::directory_iterator(target_dir))
  {
    if (p.is_directory())
      dumpDirectory(p, result);
    else
    {
      result.insert(p.path().string());
    }
  }
}

std::string CXString_to_string(const CXString &str)
{
  std::string res(clang_getCString(str));
  clang_disposeString(str);
  return res;
}

std::ostream &operator<<(std::ostream &stream, const CXString &str)
{
  stream << clang_getCString(str);
  clang_disposeString(str);
  return stream;
}

} // namespace
//--------------------------------------------------------
// //client_date is ClangAnalyzer
// CXChildVisitResult namespaceVisitor(CXCursor c, CXCursor parent, CXClientData client_data)
// {
//   return CXChildVisit_Recurse;
// }
// //client_data is ClangAnalyzer
// CXChildVisitResult cursorVisitorHelper(CXCursor c, CXCursor parent, CXClientData client_data)
// {

//   unsigned start, end;
//   CXFile file;

//   clang_getExpansionLocation(clang_getCursorLocation(c), &file, &start, &end, nullptr);
//   //check whether file is in the target_files_
//   std::string current_file = CXString_to_string(clang_getFileName(file));
//   ClangAnalyzer *ana = static_cast<ClangAnalyzer *>(client_data);
//   if (ana->target_files_.find(current_file) == ana->target_files_.end())
//     return CXChildVisit_Continue;

//   switch (clang_getCursorKind(c))
//   {
//   case CXCursor_Namespace:
//     break;
//   case CXCursor_ClassDecl:
//     break;
//   default:
//     break;
//   }

//   //if not put it into codetable
//   std::cout << start << "," << end << ' ' << clang_getCursorKindSpelling(clang_getCursorKind(c)) << ' ' << clang_getCursorSpelling(c) << '\n';
//   // std::cout << "parrent identifier: " << clang_getCString(clang_getCursorSpelling(parent)) << "\n";

//   return CXChildVisit_Recurse;
// }

//-----------------------------------------------------------------------

ClangAnalyzer::ClangAnalyzer(const fs::path &file, const fs::path &target_dir, const std::vector<std::string> &args, const fs::path &out_path) : main_file_(file), target_dir_(target_dir), args_(args), out_path_(out_path)
{
  out_path_ /= "out";
  // check whether the out dir exist
  if (!fs::exists(out_path_) || !fs::is_directory(out_path_))
  {
    LOG(INFO) << "Create out dir";
    fs::create_directories(out_path_);
  }

  compile_database_helper_.setOutPath(out_path_);
  //create the compile database file
  compile_database_helper_.addCompileEntry(main_file_.parent_path().string(), args_, main_file_.string());
  compile_database_helper_.create();
}
ClangAnalyzer::~ClangAnalyzer()
{
  //todo free all memory
}

ANA_ERR_CODE ClangAnalyzer::init_main()
{
  //add files to target_files_
  dumpDirectory(target_dir_, target_files_);
  //-----------------------------------------------
  // //parse clang ast
  // CXIndex index = clang_createIndex(0, 0);
  // CXTranslationUnit unit = clang_parseTranslationUnit(index, main_file_.string().c_str(), nullptr, 0, nullptr, 0, CXTranslationUnit_None);

  // //todo add diagnostic to check grammar
  // if (unit == nullptr)
  // {
  //   LOG(FATAL) << "unable to parse translation unit";
  //   return ANA_ERR_CODE::ERR_CLANG_PARSE;
  // }

  // CXCursor cursor = clang_getTranslationUnitCursor(unit);

  // //parse clang ast to codeblocks
  // clang_visitChildren(cursor, cursorVisitorHelper,
  //                     this);

  // //get start function from table
  // //if yes set it as the root of the codegraph
  // if (!code_graph_.hasRoot())
  //   ;
  // clang_disposeTranslationUnit(unit);
  // clang_disposeIndex(index);
  // return ANA_ERR_CODE::SUCCESS;
  //-----------------------------------------------

  //read all content
  // std::ifstream fs(main_file_);
  // if (!fs.is_open())
  //   return ANA_ERR_CODE::ERR_FILE_NOT_FUND;
  // std::ostringstream ss;
  // std::copy(std::istreambuf_iterator<char>(fs),
  //           std::istreambuf_iterator<char>(),
  //           std::ostreambuf_iterator<char>(ss));

  // clang::tooling::runToolOnCodeWithArgs(new clangAction(target_files_), ss.str(), {"-I " + target_dir_.string()}, main_file_.filename().string());
  //-----------------------------------------------------------------------------

  //read compile database
  std::string err_message;
  auto database = clang::tooling::CompilationDatabase::loadFromDirectory(out_path_.string(), err_message);

  clang::tooling::ClangTool Tool(*database, {main_file_.string()});
  assert(database == nullptr);
  Tool.run(newFactory<clangAction>(this).get());

  return ANA_ERR_CODE::SUCCESS;
}
} // namespace analib
