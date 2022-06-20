#ifndef __EQUIPMENT_INC_H__
#define __EQUIPMENT_INC_H__

#include "../stdafx.h"

#include <list>
#include <sstream>
#include <memory>

namespace Composite {

class CompositeEquipment;
//class CompositeEquipment::Ptr;

class Equipment {
public:
    Equipment(std::string name) { name_ = name; }
    virtual ~Equipment() { }
    CLASS_PTR(Equipment);

    // ����ӿ�Ϊ������Composite��Ľӿ�Add&Remove;���ٹ������в���Ҫ�Ľӿ�
    virtual std::shared_ptr<CompositeEquipment> GetComposite() { return nullptr; }
    virtual std::string toString() { return name_; }

    // ����������ӿڶ���...

private:
    std::string name_;
    // other member...
};

class CompositeEquipment : public Equipment,
                  public std::enable_shared_from_this<CompositeEquipment> {
public:
    CompositeEquipment(std::string name);
    ~CompositeEquipment();
    CLASS_PTR(CompositeEquipment);

    void Add(Equipment::Ptr&& p);
    void Remove(Equipment::Ptr&& p);

    CompositeEquipment::Ptr GetComposite() override;
    std::string toString() override;

private:
    std::list<Equipment::Ptr> composites_;
};

class FloppyDisk : public Equipment {
public:
    FloppyDisk(std::string name) : Equipment(name) { }
};

void test();

}

#endif // __EQUIPMENT_INC_H__
