#include "equipment.h"

namespace Composite {

CompositeEquipment::CompositeEquipment(std::string name)
    : Equipment(name) {
}

CompositeEquipment::~CompositeEquipment() {
    composites_.clear();
}

void CompositeEquipment::Add(Equipment::Ptr&& p) {
    composites_.push_back(p);
}

void CompositeEquipment::Remove(Equipment::Ptr&& p) {
    for (auto it = composites_.begin(); it != composites_.end(); ++it) {
        if (*it == p) {
            composites_.erase(it);
            break;
        }
    }
}

CompositeEquipment::Ptr CompositeEquipment::GetComposite() {
    return shared_from_this();
}

std::string CompositeEquipment::toString() {
    std::stringstream ss;
    ss << Equipment::toString() << ", composites: [";
    for (auto& c : composites_) {
        ss << c->toString() << ",";
    }
    ss << "]";
    return ss.str();
}


void test() {
    std::cout << "\n\n Composite Pattern." << std::endl;

    Equipment::Ptr equipment = std::make_shared<CompositeEquipment>("CE");
    auto composite = equipment->GetComposite();
    if (composite) {
        composite->Add(std::make_shared<FloppyDisk>("FloppyDisk1"));
        composite->Add(std::make_shared<FloppyDisk>("FloppyDisk2"));
        composite->Add(std::make_shared<FloppyDisk>("FloppyDisk3"));
    }

    std::cout << equipment->toString() << std::endl;
}

}

