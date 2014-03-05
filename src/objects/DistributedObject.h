// Filename: DistributedObject.h
#pragma once
namespace bamboo {


// Foward declarations
class ObjectData;

class DistributedObject {
  protected:
    DistributedObject(const ObjectData&);
  public:
    virtual ~DistributedObject() {}
    virtual void initialize(const ObjectData&) = 0;
};


} // close namespace bamboo
