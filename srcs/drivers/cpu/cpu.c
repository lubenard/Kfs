#include "cpu.h"
#include "../../lib/iolib.h"

void check_cpu_features() {
    check_cpu_id();
    check_cpu_vendor();
    get_cpu_features();
}

// This function is called via ASM function check_cpu_vendor
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

void check_ecx_features(int ecx) {
	if (ecx & CPUID_FEAT_ECX_SSE3)
		printd(KERN_NORMAL, "SSE3, ");
	if (ecx & CPUID_FEAT_ECX_PCLMUL)
		printd(KERN_NORMAL, "PCLMUL, ");
	if (ecx & CPUID_FEAT_ECX_DTES64)
		printd(KERN_NORMAL, "DTES64, ");
	if (ecx & CPUID_FEAT_ECX_MONITOR)
		printd(KERN_NORMAL, "MONITOR, ");
	if (ecx & CPUID_FEAT_ECX_DS_CPL)
		printd(KERN_NORMAL, "DS_CPl, ");
	if (ecx & CPUID_FEAT_ECX_VMX)
		printd(KERN_NORMAL, "VMX, ");
	if (ecx & CPUID_FEAT_ECX_SMX)
		printd(KERN_NORMAL, "SMX, ");
	if (ecx & CPUID_FEAT_ECX_EST)
		printd(KERN_NORMAL, "EST, ");
	if (ecx & CPUID_FEAT_ECX_TM2)
		printd(KERN_NORMAL, "TM2, ");
	if (ecx & CPUID_FEAT_ECX_SSSE3)
		printd(KERN_NORMAL, "SSSE3, ");
	if (ecx & CPUID_FEAT_ECX_CID)
		printd(KERN_NORMAL, "CID, ");
	if (ecx & CPUID_FEAT_ECX_SDBG)
		printd(KERN_NORMAL, "SDBG, ");
	if (ecx & CPUID_FEAT_ECX_FMA)
		printd(KERN_NORMAL, "FMA, ");
	if (ecx & CPUID_FEAT_ECX_CX16)
		printd(KERN_NORMAL, "CX16, ");
	if (ecx & CPUID_FEAT_ECX_XTPR)
		printd(KERN_NORMAL, "XPTR, ");
	if (ecx & CPUID_FEAT_ECX_PDCM)
		printd(KERN_NORMAL, "PDCM, ");
	if (ecx & CPUID_FEAT_ECX_PCID)
		printd(KERN_NORMAL, "PCID, ");
	if (ecx & CPUID_FEAT_ECX_DCA)
		printd(KERN_NORMAL, "DCA, ");
	if (ecx & CPUID_FEAT_ECX_SSE4_1)
		printd(KERN_NORMAL, "SSE4_1, ");
	if (ecx & CPUID_FEAT_ECX_SSE4_2)
		printd(KERN_NORMAL, "SSE4_2, ");
	if (ecx & CPUID_FEAT_ECX_X2APIC)
		printd(KERN_NORMAL, "X2APIC, ");
	if (ecx & CPUID_FEAT_ECX_MOVBE)
		printd(KERN_NORMAL, "MOVBE, ");
	if (ecx & CPUID_FEAT_ECX_POPCNT)
		printd(KERN_NORMAL, "POPCNT, ");
	if (ecx & CPUID_FEAT_ECX_TSC)
		printd(KERN_NORMAL, "TSC, ");
	if (ecx & CPUID_FEAT_ECX_AES)
		printd(KERN_NORMAL, "AES, ");
	if (ecx & CPUID_FEAT_ECX_XSAVE)
		printd(KERN_NORMAL, "XSAVE, ");
	if (ecx & CPUID_FEAT_ECX_OSXSAVE)
		printd(KERN_NORMAL, "OSXSAVE, ");
	if (ecx & CPUID_FEAT_ECX_AVX)
		printd(KERN_NORMAL, "AVX, ");
	if (ecx & CPUID_FEAT_ECX_F16C)
		printd(KERN_NORMAL, "F16C, ");
	if (ecx & CPUID_FEAT_ECX_RDRAND)
		printd(KERN_NORMAL, "RDRAND, ");
	if (ecx & CPUID_FEAT_ECX_HYPERVISOR)
		printd(KERN_NORMAL, "HYPERVISOR, ");
}

void check_edx_features(int edx) {
	if (edx & CPUID_FEAT_EDX_FPU)
		printd(KERN_NORMAL, "FPU, ");
	if (edx & CPUID_FEAT_EDX_VME)
		printd(KERN_NORMAL, "VME, ");
	if (edx & CPUID_FEAT_EDX_DE)
		printd(KERN_NORMAL, "DE, ");
	if (edx & CPUID_FEAT_EDX_PSE)
		printd(KERN_NORMAL, "PSE, ");
	if (edx & CPUID_FEAT_EDX_TSC)
		printd(KERN_NORMAL, "TSC, ");
	if (edx & CPUID_FEAT_EDX_MSR)
		printd(KERN_NORMAL, "MSR, ");
	if (edx & CPUID_FEAT_EDX_PAE)
		printd(KERN_NORMAL, "PAE, ");
	if (edx & CPUID_FEAT_EDX_MCE)
		printd(KERN_NORMAL, "MCE, ");
	if (edx & CPUID_FEAT_EDX_CX8)
		printd(KERN_NORMAL, "CX8, ");
	if (edx & CPUID_FEAT_EDX_APIC)
		printd(KERN_NORMAL, "APIC, ");
	if (edx & CPUID_FEAT_EDX_SEP)
		printd(KERN_NORMAL, "SEP, ");
	if (edx & CPUID_FEAT_EDX_MTRR)
		printd(KERN_NORMAL, "MTRR, ");
	if (edx & CPUID_FEAT_EDX_PGE)
		printd(KERN_NORMAL, "PGE, ");
	if (edx & CPUID_FEAT_EDX_MCA)
		printd(KERN_NORMAL, "MCA, ");
	if (edx & CPUID_FEAT_EDX_CMOV)
		printd(KERN_NORMAL, "CMOV, ");
	if (edx & CPUID_FEAT_EDX_PAT)
		printd(KERN_NORMAL, "PAT, ");
	if (edx & CPUID_FEAT_EDX_PSE36)
		printd(KERN_NORMAL, "PSE36, ");
	if (edx & CPUID_FEAT_EDX_PSN)
		printd(KERN_NORMAL, "PSN, ");
	if (edx & CPUID_FEAT_EDX_CLFLUSH)
		printd(KERN_NORMAL, "CLFLUSH, ");
	if (edx & CPUID_FEAT_EDX_DS)
		printd(KERN_NORMAL, "DS, ");
	if (edx & CPUID_FEAT_EDX_ACPI)
		printd(KERN_NORMAL, "ACPI, ");
	if (edx & CPUID_FEAT_EDX_MMX)
		printd(KERN_NORMAL, "MMX, ");
	if (edx & CPUID_FEAT_EDX_FXSR)
		printd(KERN_NORMAL, "FXSR, ");
	if (edx & CPUID_FEAT_EDX_SSE)
		printd(KERN_NORMAL, "SSE, ");
	if (edx & CPUID_FEAT_EDX_SSE2)
		printd(KERN_NORMAL, "SSE2, ");
	if (edx & CPUID_FEAT_EDX_SS)
		printd(KERN_NORMAL, "SS, ");
	if (edx & CPUID_FEAT_EDX_HTT)
		printd(KERN_NORMAL, "HTT, ");
	if (edx & CPUID_FEAT_EDX_TM)
		printd(KERN_NORMAL, "TM, ");
	if (edx & CPUID_FEAT_EDX_IA64)
		printd(KERN_NORMAL, "IA64, ");
	if (edx & CPUID_FEAT_EDX_PBE)
		printd(KERN_NORMAL, "PBE, ");
}

void display_cpu_features(int edx, int ecx) {
	printd(KERN_NORMAL, "CPU features are: ");
	check_edx_features(edx);
	check_ecx_features(ecx);
	printd(KERN_NORMAL, "\n");
}
