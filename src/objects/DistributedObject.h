// Filename: DistributedObject.h
#pragma once
namespace bamboo {


// Foward declarations
class TypeData;

class DistributedObject {
  protected:
    DistributedObject(const TypeData&);
  public:
    virtual ~DistributedObject() {}
    virtual void initialize(const TypeData& args) = 0;
};


} // close namespace bamboo
