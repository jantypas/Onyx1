#include "Onyx1Core/VMemMgr.h"

int main() {
    VirtualPageObject po;
    std::vector<uint32_t> pageset;
    auto vmemptr = new VMemMgr();
    vmemptr->initialize(1024, 1024);
    auto result = vmemptr->allocateNewVirtualPageSet(po, 5, &pageset);
    for (auto ix : pageset) {
        printf("%d\n", ix);
    };
    vmemptr->terminate();
    return 0;
}
