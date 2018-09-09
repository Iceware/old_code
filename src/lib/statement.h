#pragma once
#include <list>
#include "context.h"
#include "err/error.h"
namespace analib
{
class Expr;
class VarDecl;

enum class STMT_KIND
{
  //commen statement
  //asm stmt, do not support this stmt currently
  STMT_ASM,
  STMT_COMPOUND,
  STMT_CPP_CONTINUE,
  STMT_CPP_BREAK,
  STMT_EXPR,
  STMT_DECL,
  STMT_FOR,
  STMT_GOTO,
  STMT_IF,
  STMT_LABAL,

  //Stmt for cpp
  //for(int:vector)
  STMT_CPP_FORRANGE,
  STMT_CPP_TRY,
  STMT_CPP_DO,

};

//statement class
//a=100
//*p=&a
//while,for,sitch
class Stmt
{
public:
  virtual ANA_ERR_CODE run(Context *context) = 0;

  STMT_KIND getStmtKind();

  Location getLocation();

private:
  STMT_KIND kind_;
  //the location of this statement
  Location location_;
};

class CompoundStmt final : public Stmt
{
public:
  virtual ANA_ERR_CODE run(Context *context) override;

  std::list<Stmt *>::iterator getBodyBegin();
  std::list<Stmt *>::iterator getBodyEnd();

private:
  std::list<Stmt *> body_;
};

//for(init;cond;inc),
//init cond inc will be null if not specified in source
class ForStmt : public Stmt
{
public:
  ForStmt(Stmt *Init, Expr *cond, VarDecl *condVar, Expr *inc, Stmt *body, Location location);

  virtual ANA_ERR_CODE run(Context *context);

private:
  enum
  {
    INIT,
    CONDVAR,
    COND,
    INC,
    BODY,
    END_EXPR
  };
  Stmt *SubExprs[END_EXPR];
};

} // namespace analib