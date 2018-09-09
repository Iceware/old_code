#pragma once
#include <string>
#include "location.h"
namespace analib
{

enum class DECL_KIND
{
  DECL_STRUCT = 1,        //struct
  DECL_UNION,             //union
  DECL_CLASS,             //class
  DECL_ENUM,              //enum
  DECL_FUNCTION,          // function
  DECL_VAR,               //variable
  DECL_CLASS_METHOD,      //class method
  DECL_NAMESPACE,         //namespace
  DECL_CLASS_CONSTRUCTOR, //class constructor
  DECL_CLASS_DESTRUCTOR,  //class desctructor
};
//declaration class
//int a=0;
//class A{}
//void foo(){}
class Decl
{
public:
  Decl(const std::string name, DECL_KIND decl_type, Location location) : name_(name), decl_type_(decl_type), location_(location) {}

  std::string getName();
  DECL_KIND getDeclKind();
  Location getLocation();

private:
  //name of the decaration,ie. funtion name,class name
  std::string name_;
  //type of the declaration
  DECL_KIND decl_type_;
  //the location of this declaration
  Location location_;
};

//represent a variable declaration
class VarDecl : public Decl
{
  
};

} // namespace analib