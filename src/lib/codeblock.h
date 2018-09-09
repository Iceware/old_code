#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include "statement.h"
#include "context.h"
#include "location.h"

namespace analib
{

class CodeBlock;

//reperesent the link between codeblocks
class Edge
{
public:
  Edge(CodeBlock *start, unsigned int start_offset, CodeBlock *end, unsigned int end_offset) : start_(start), start_offset_(start_offset), end_(end), end_offset_(end_offset){};

  CodeBlock *getStart() const;
  unsigned int getStartOffset() const;

  CodeBlock *getEnd() const;
  unsigned int getEndOffset() const;

private:
  CodeBlock *start_;
  unsigned int start_offset_;
  CodeBlock *end_;
  unsigned int end_offset_;
};

//store the one part of runtime status of programe
class CodeBlock
{
public:
  CodeBlock();
  void linkTo(unsigned int start_offset, const CodeBlock *block, unsigned int end_offset);

  //return the location of the first stmt in the block
  Location getStartLocation() const;
  Location getEndLocation() const;

  bool operator==(const CodeBlock &rhs) const;

private:
  //the source codes in this block;
  CompoundStmt *stmts_;

  //store the init context when enter this codeblock
  Context *context_;

  //this block is the start of the edge
  std::vector<Edge> starts_;
  //this block is the end of the edge
  std::vector<Edge> ends_;
};

//store all existing codeblock
class CodeBlockTable
{
public:
  CodeBlockTable();
  void addBlock(CodeBlock *block);

  CodeBlock *findBlock(const Location &location);

private:
  std::unordered_map<Location, CodeBlock *> blocks_;
};

class CodeBlockGraph
{
public:
  CodeBlockGraph() : root_(nullptr){};
  CodeBlockGraph(const CodeBlockGraph &rhs);

  void setRoot(CodeBlock *root);
  void addCodeBlock(CodeBlock *block);

  bool hasRoot();

private:
  CodeBlock *root_;
};
} // namespace analib
