#include "cpu.h"
#include "../../lib/iolib.h"

void check_cpu_features() {
    check_cpu_id();
    check_cpu_vendor();
    get_cpu_features();
}

// Letters are stored as int
// Example:
// ebx might look this: 1970169159, which is actually translated to 1110101 01101110 01100101 01000111
// Each 8 bytes group can represent an Ascii letter.
// Once extracted, we need to put them in the right order to get the cpu vendor
// See here for more information:
// https://wiki.osdev.org/CPUID
void display_cpu_vendor(int ebx, int edx, int ecx) {
    unsigned char ebx_l1 = ebx & 0xFF;
    unsigned char ebx_l2 = (ebx >> 8) & 0xFF;
    unsigned char ebx_l3 = (ebx >> 16) & 0xFF;
    unsigned char ebx_l4 = (ebx >> 24) & 0xFF;

    unsigned char edx_l1 = edx & 0xFF;
    unsigned char edx_l2 = (edx >> 8) & 0xFF;
    unsigned char edx_l3 = (edx >> 16) & 0xFF;
    unsigned char edx_l4 = (edx >> 24) & 0xFF;

    unsigned char ecx_l1 = ecx & 0xFF;
    unsigned char ecx_l2 = (ecx >> 8) & 0xFF;
    unsigned char ecx_l3 = (ecx >> 16) & 0xFF;
    unsigned char ecx_l4 = (ecx >> 24) & 0xFF;

    printd(KERN_INFO, "CPU is %c%c%c%c%c%c%c%c%c%c%c%c",
           ebx_l1, ebx_l2, ebx_l3, ebx_l4,
           edx_l1, edx_l2, edx_l3, edx_l4,
           ecx_l1, ecx_l2, ecx_l3, ecx_l4
    );
}

void display_cpu_features(int s1, int s2) {
    printd(KERN_INFO, "CPU features are: %d %d", s1, s2);
}