// Filename: DistributedObject.h
#pragma once
namespace bamboo {


// Foward declarations
class TypeData;

class DistributedObject {
  protected:
    inline DistributedObject() {}
    inline DistributedObject(const TypeData& args) { initialize(args); }

  public:
    virtual ~DistributedObject() {}
    virtual void initialize(const TypeData& args) = 0;
};


} // close namespace bamboo
