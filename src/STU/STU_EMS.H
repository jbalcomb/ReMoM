/*
    Simulation Texas Universe
    ~MS-DOS Interrupts
    IBM-PC
    Expanded Memory System (EMS)
    Expanded Memory Manager (EMM)

*/
#ifndef STU_EMS_H
#define STU_EMS_H

#include <stdint.h>


using Bitu   = uintptr_t;
using Bits   = intptr_t;

/* Upstream uses a macro named this way for formatting Bitu values.
 */
#define sBitfs(x) PRI ## x ## PTR

typedef int32_t MemHandle;

#define EMM_PAGEFRAME	0xE000
#define EMM_PAGEFRAME4K	((EMM_PAGEFRAME*16)/4096)
#define	EMM_MAX_HANDLES	200				/* 255 Max */
#define EMM_PAGE_SIZE	(16*1024U)
#define EMM_MAX_PAGES	(32 * 1024 / 16 )
#define EMM_MAX_PHYS	4				/* 4 16kb pages in pageframe */

#define EMM_VERSION			0x40
#define EMM_MINOR_VERSION	0x00
//#define EMM_MINOR_VERSION	0x30	// emm386 4.48
#define GEMMIS_VERSION		0x0001	// Version 1.0

#define EMM_SYSTEM_HANDLE	0x0000
#define NULL_HANDLE			0xFFFF
#define	NULL_PAGE			0xFFFF

#define ENABLE_VCPI 1
#define ENABLE_V86_STARTUP 0

#define EMM_VOLATILE 0
#define EMM_NONVOLATILE 1

/* EMM errors */
#define EMM_NO_ERROR			0x00
#define EMM_SOFT_MAL			0x80
#define EMM_HARD_MAL			0x81
#define EMM_INVALID_HANDLE		0x83
#define EMM_FUNC_NOSUP			0x84
#define EMM_OUT_OF_HANDLES		0x85
#define EMM_SAVEMAP_ERROR		0x86
#define EMM_OUT_OF_PHYS			0x87
#define EMM_OUT_OF_LOG			0x88
#define EMM_ZERO_PAGES			0x89
#define EMM_LOG_OUT_RANGE		0x8a
#define EMM_ILL_PHYS			0x8b
#define EMM_PAGE_MAP_SAVED		0x8d
#define EMM_NO_SAVED_PAGE_MAP	0x8e
#define EMM_INVALID_SUB			0x8f
#define EMM_ATTR_UNDEF			0x90
#define EMM_FEAT_NOSUP			0x91
#define EMM_MOVE_OVLAP			0x92
#define EMM_MOVE_OVLAPI			0x97
#define EMM_NOT_FOUND			0xa0

struct EMM_Mapping {
	uint16_t handle;
	uint16_t page;
};

struct EMM_Handle {
	uint16_t pages;
	MemHandle mem;
	char name[8];
	bool saved_page_map;
	EMM_Mapping page_map[EMM_MAX_PHYS];
};

static Bitu ems_type;

static EMM_Handle emm_handles[EMM_MAX_HANDLES];
static EMM_Mapping emm_mappings[EMM_MAX_PHYS];
static EMM_Mapping emm_segmentmappings[0x40];



#ifdef __cplusplus
extern "C" {
#endif





#ifdef __cplusplus
}
#endif

#endif /* STU_EMS_H */
