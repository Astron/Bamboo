// Filename: DistributedObject.h
#pragma once
namespace bamboo {


// Foward declarations
class Value;

class DistributedObject {
  protected:
    inline DistributedObject() {}
    inline DistributedObject(const Value& args) { initialize(args); }

  public:
    virtual ~DistributedObject() {}
    virtual void initialize(const Value& args) = 0;
};


} // close namespace bamboo
