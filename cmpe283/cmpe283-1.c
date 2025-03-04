/*  
 *  cmpe283-1.c - Kernel module for CMPE283 assignment 1
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <asm/msr.h>

#define MAX_MSG 80

/*
 * Model specific registers (MSRs) by the module.
 * See SDM volume 4, section 2.1
 */
#define IA32_VMX_PINBASED_CTLS	 0x481
#define IA32_VMX_PROCBASED_CTLS  0x482
#define IA32_VMX_PROCBASED_CTLS2 0x48B
#define IA32_VMX_EXIT_CTLS       0x483

/*
 * struct caapability_info
 *
 * Represents a single capability (bit number and description).
 * Used by report_capability to output VMX capabilities.
 */
struct capability_info {
	uint8_t bit;
	const char *name;
};


/*
 * Pinbased capabilities
 * See SDM volume 3, section 24.6.1
 */
struct capability_info pinbased[5] =
{
	{ 0, "External Interrupt Exiting" },
	{ 3, "NMI Exiting" },
	{ 5, "Virtual NMIs" },
	{ 6, "Activate VMX Preemption Timer" },
	{ 7, "Process Posted Interrupts" }
};

struct capability_info procbased[22] = 
{
	{ 2, "Interrupt Window Exiting" },
	{ 3, "Use TSC Offsetting" },
	{ 7, "HLT Exiting" },
	{ 9, "INVLPG Exiting" },
	{ 10, "MWAIT Exiting" },
	{ 11, "RDPMC Exiting" },
	{ 12, "RDTSC Exiting" },
	{ 15, "CR3 Load Exiting" },
	{ 16, "CR3 Store Exiting" },
	{ 17, "Activate Tertiary Controls" },
	{ 19, "CR8 Load Exiting" },
	{ 20, "CR8 Store Exiting" },
	{ 21, "Use TPR Shadow" },
	{ 22, "NMI Window Exiting" },
	{ 23, "MOV DR Exiting" },
	{ 24, "Unconditional I/O Exiting" },
	{ 25, "Use I/O Bitmaps" },
	{ 27, "Monitor Trap Flag" },
	{ 28, "Use MSR Bitmaps" },
	{ 29, "MONITOR exiting" },
	{ 30, "PAUSE exiting" },
	{ 31, "Activate Secondary Controls" }
};

struct capability_info secondary_procbased[27] = 
{
	{ 0, "Virtualize APIC Accesses" },
	{ 1, "Enable EPT" },
	{ 2, "Descriptor-table exiting" },
	{ 3, "Enable RDTSCP" },
	{ 4, "Virtualize x2APIC mode" },
	{ 5, "Enable VPID" },
	{ 6, "WBINVD exiting" },
	{ 7, "Unrestricted guest" },
	{ 8, "APIC-register virtualization" },
	{ 9, "Virtual-interrupt delivery" },
	{ 10, "PAUSE-loop exiting" },
	{ 11, "RDRAND exiting" },
	{ 12, "Enable INVPCID" },
	{ 13, "Enable VM functions" },
	{ 14, "VMCS shadowing" },
	{ 15, "Enable ENCLS exiting" },
	{ 16, "RDSEED exiting" },
	{ 17, "Enable PML" },
	{ 18, "EPT-violation #VE" },
	{ 19, "Conceal VMX from PT" },
	{ 20, "Enable XSAVES/XRSTORS" },
	{ 22, "Mode-based execute control for EPT" },
	{ 23, "Sub-page write permissions for EPT" },
	{ 24, "Intel PT uses guest physical addresses" },
	{ 25, "Use TSC scaling" },
	{ 26, "Enable user wait and pause" },
	{ 28, "Enable ENCLV exiting" },
};

struct capability_info vmexit[14] = 
{
	{ 2, "Save debug controls" },
	{ 9, "Host address-space size" },
	{ 12, "Load IA32_PERF_GLOBAL_ CTRL" },
	{ 15, "Acknowledge interrupt on exit" },
	{ 18, "Save IA32_PAT" },
	{ 19, "Load IA32_PAT" },
	{ 20, "Save IA32_EFER" },
	{ 21, "Load IA32_EFER" },
	{ 22, "Save VMXpreemption timer value" },
	{ 23, "Clear IA32_BNDCFGS" },
	{ 24, "Conceal VMX from PT" },
	{ 25, "Clear IA32_RTIT_CTL" },
	{ 28, "Load CET state" },
	{ 29, "Load PKRS" },
};

/*
 * report_capability
 *
 * Reports capabilities present in 'cap' using the corresponding MSR values
 * provided in 'lo' and 'hi'.
 *
 * Parameters:
 *  cap: capability_info structure for this feature
 *  len: number of entries in 'cap'
 *  lo: low 32 bits of capability MSR value describing this feature
 *  hi: high 32 bits of capability MSR value describing this feature
 */
void
report_capability(struct capability_info *cap, uint8_t len, uint32_t lo,
    uint32_t hi)
{
	uint8_t i;
	struct capability_info *c;
	char msg[MAX_MSG];

	memset(msg, 0, sizeof(msg));

	for (i = 0; i < len; i++) {
		c = &cap[i];
		snprintf(msg, 79, "  %s: Can set=%s, Can clear=%s\n",
		    c->name,
		    (hi & (1 << c->bit)) ? "Yes" : "No",
		    !(lo & (1 << c->bit)) ? "Yes" : "No");
		printk(msg);
	}
}

/*
 * detect_vmx_features
 *
 * Detects and prints VMX capabilities of this host's CPU.
 */
void
detect_vmx_features(void)
{
	uint32_t lo, hi;

	/* Pinbased controls */
	rdmsr(IA32_VMX_PINBASED_CTLS, lo, hi);
	pr_info("Pinbased Controls MSR: 0x%llx\n",
		(uint64_t)(lo | (uint64_t)hi << 32));
	report_capability(pinbased, 5, lo, hi);

    rdmsr(IA32_VMX_PROCBASED_CTLS, lo, hi);
	pr_info("Procbased Controls MSR: 0x%llx\n",
		(uint64_t)(lo | (uint64_t)hi << 32));
    report_capability(procbased, 22, lo, hi);

    rdmsr(IA32_VMX_PROCBASED_CTLS2, lo, hi);
	pr_info("Secondary Procbased Controls MSR: 0x%llx\n",
		(uint64_t)(lo | (uint64_t)hi << 32));
    report_capability(secondary_procbased, 27, lo, hi);

    rdmsr(IA32_VMX_EXIT_CTLS, lo, hi);
	pr_info("VM Exit Controls MSR: 0x%llx\n",
		(uint64_t)(lo | (uint64_t)hi << 32));
    report_capability(vmexit, 14, lo, hi);
}

/*
 * init_module
 *
 * Module entry point
 *
 * Return Values:
 *  Always 0
 */
int
init_module(void)
{
	printk(KERN_INFO "CMPE 283 Assignment 1 Module Start\n");

	detect_vmx_features();

	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return 0;
}

/*
 * cleanup_module
 *
 * Function called on module unload
 */
void
cleanup_module(void)
{
	printk(KERN_INFO "CMPE 283 Assignment 1 Module Exits\n");
}

MODULE_LICENSE("GPL v2");
