//
// Created by jantypas on 10/31/21.
//

#include "VMgrSwap.h"
#include "Onyx1Const.h"

/**
 * open -- Open the swapping system.
 *
 * @return -- Return an error if we can't start it.
 */
bool VMgrSwap::open() {
    swapfile.open(SWAPFILE_NAME, std::ios::in|std::ios::out|std::ios::binary);
    return swapfile.is_open());
}

/**
 * closwe -- Close the swapping system.
 *
 * @return -- Terminate the swapping system
 */
bool VMgrSwap::close() {
    if (swapfile.is_open()) {
        swapfile.close();
        return true;
    } else {
        return false;
    }
}

/**
 * doSwapping -- Do the entire swapping operation
 *
 * @param desiredFreePages  -- How many pages would we like
 * @return                  -- Pages we actually swapped
 */
int32_t VMgrSwap::doSwapping(uint32_t desiredFreePages) {

}

