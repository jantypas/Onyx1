#include "Onyx1Core/VMemMgr.h"

int main() {
    auto vmemptr = new VMemMgr();
    auto memmgr = vmemptr->initialize(1024, 1024);
    return 0;
}
