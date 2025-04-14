


Also, Borland C++ 3 Overlays?



struct with funcion points
    with trailing arbitrary data array
array of function pointers



e.g.,
void WZD_main(void)
    EMM_Pages_Reserved = 158;
    EMS_SetMinKB(2700);
    RAM_SetMinKB(583);
    Init_Drivers()
        |-> EMM_Startup()
    // mov     ax, 5901h
    // int     67h
    // mov     [bp+EMS_Pages_Left], bx
    EMS_Pages_Left = STU_INT(0x67, 5901);

    if(EMS_Pages_Left != 0)
    {
        g_EmmHndl_OVERXYZ = EMM_MakeNamedHandle(EMS_Pages_Left, ehn_OVERXYZ);
        EMM_ReleaseHandle(g_EmmHndl_OVERXYZ);
        __OvrInitEms(0, 0, EMS_Pages_Left);
    }
    else
    {
        g_EmmHndl_OVERXYZ = 0;  // ¿ NULLL ?
    }






constexpr uint16_t MemPageSize = 4096;

typedef int32_t MemHandle;

#define NULL_HANDLE			0xffff

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

static EMM_Handle emm_handles[EMM_MAX_HANDLES];

static uint8_t EMM_AllocateMemory(uint16_t pages,uint16_t & dhandle,bool can_allocate_zpages) {
	/* Check for 0 page allocation */
	if (!pages) {
		if (!can_allocate_zpages) return EMM_ZERO_PAGES;
	}
	/* Check for enough free pages */
	if ((MEM_FreeTotal()/ 4) < pages) { return EMM_OUT_OF_LOG;}
	uint16_t handle = 1;
	/* Check for a free handle */
	while (emm_handles[handle].pages != NULL_HANDLE) {
		if (++handle >= EMM_MAX_HANDLES) {return EMM_OUT_OF_HANDLES;}
	}
	MemHandle mem = 0;
	if (pages) {
		mem = MEM_AllocatePages(pages*4,false);
		if (!mem) E_Exit("EMS:Memory allocation failure");
	}
	emm_handles[handle].pages = pages;
	emm_handles[handle].mem = mem;
	/* Change handle only if there is no error. */
	dhandle = handle;
	return EMM_NO_ERROR;
}

...
`switch (reg_al) { /* ... */ }`
...
regs.h

union GenReg32 {
	uint32_t dword[1] = {};
	uint16_t word[2];
	uint8_t byte[4];
};

extern CPU_Regs cpu_regs;

struct CPU_Regs {
	GenReg32 regs[8] = {};
	GenReg32 ip      = {};
	uint32_t flags   = 0;
	constexpr void ApplyTestFlags(const CpuTestFlags& requested_flags);
};

inline constexpr uint8_t &reg_al = cpu_regs.regs[REGI_AX].byte[BL_INDEX];
inline constexpr uint8_t &reg_ah = cpu_regs.regs[REGI_AX].byte[BH_INDEX];
inline constexpr uint16_t &reg_ax = cpu_regs.regs[REGI_AX].word[W_INDEX];
inline constexpr uint32_t &reg_eax = cpu_regs.regs[REGI_AX].dword[DW_INDEX];

enum {
	REGI_AX = 0,
	REGI_CX,
	REGI_DX,
	REGI_BX,
	REGI_SP,
	REGI_BP,
	REGI_SI,
	REGI_DI,
};

enum {
	REGI_AL = 0,
	REGI_CL,
	REGI_DL,
	REGI_BL,
	REGI_AH,
	REGI_CH,
	REGI_DH,
	REGI_BH,
};

constexpr auto BH_INDEX = 1;
constexpr auto BL_INDEX = 0;











# RefMat

https://www.stanislavs.org/helppc/idx_interrupt.html

Interrupt Services DOS/BIOS/EMS/Mouse

BIOS break interrupt		BIOS clock services		BIOS communications		BIOS disk services		BIOS keyboard services	
BIOS printer services		BIOS serial services		BIOS system services		BIOS timer interrupt		BIOS video services	
critical error handler		Ctrl-Break		ctrl-c		DOS functions		DOS interrupts	
DOS multiplex		DOS scheduler		DOS services		EMS services		EMS status	
equipment flags		exceptions		exec function		INDOS
int 10	
int 10,0		int 10,1		int 10,10		int 10,11		int 10,12	
int 10,13		int 10,14		int 10,15		int 10,1a		int 10,1b	
int 10,1c		int 10,2		int 10,3		int 10,4		int 10,5	
int 10,6		int 10,7		int 10,8		int 10,9		int 10,a	
int 10,b		int 10,c		int 10,d		int 10,e		int 10,f	
int 10,fe		int 10,ff
int 11
int 12
int 13	
int 13,0		int 13,1		int 13,10		int 13,11		int 13,12	
int 13,13		int 13,14		int 13,15		int 13,16		int 13,17	
int 13,18		int 13,19		int 13,1a		int 13,2		int 13,3	
int 13,4		int 13,5		int 13,6		int 13,7		int 13,8	
int 13,9		int 13,a		int 13,b		int 13,c		int 13,d	
int 13,e		int 13,f		int 13,status		
int 14
int 14,0	
int 14,1		int 14,2		int 14,3		int 14,4		int 14,5	
int 14,status		

int 15
int 15,0		int 15,1		int 15,2	
int 15,20		int 15,21		int 15,3		int 15,40		int 15,41	
int 15,42		int 15,43		int 15,44		int 15,4f		int 15,80	
int 15,81		int 15,82		int 15,83		int 15,84		int 15,85	
int 15,86		int 15,87		int 15,88		int 15,89		int 15,90	
int 15,91		int 15,c0		int 15,c1		int 15,c2		int 15,c3	
int 15,c4		int 15,f		

int 16		int 16,0		int 16,1	
int 16,10		int 16,11		int 16,12		int 16,2		int 16,3	
int 16,4		int 16,5		int 17		int 17,0		int 17,1	
int 17,2		int 17,status		int 19		int 1a		int 1a,0	
int 1a,1		int 1a,2		int 1a,3		int 1a,4		int 1a,5	
int 1a,6		int 1a,7		int 1a,8		int 1a,80		int 1a,9	
int 1a,a		int 1a,b		int 1b		int 1c		int 1d	
int 1e		int 1f		int 20		int 21		int 21,0	
int 21,1		int 21,10		int 21,11		int 21,12		int 21,13	
int 21,14		int 21,15		int 21,16		int 21,17		int 21,19	
int 21,1a		int 21,1b		int 21,1c		int 21,1f		int 21,2	
int 21,21		int 21,22		int 21,23		int 21,24		int 21,25	
int 21,26		int 21,27		int 21,28		int 21,29		int 21,2a	
int 21,2b		int 21,2c		int 21,2d		int 21,2e		int 21,2f	
int 21,3		int 21,30		int 21,31		int 21,32		int 21,33	
int 21,34		int 21,35		int 21,36		int 21,37		int 21,38	
int 21,39		int 21,3a		int 21,3b		int 21,3c		int 21,3d	
int 21,3e		int 21,3f		int 21,4		int 21,40		int 21,41	
int 21,42		int 21,43		int 21,44		int 21,44,0		int 21,44,1	
int 21,44,2		int 21,44,3		int 21,44,4		int 21,44,5		int 21,44,6	
int 21,44,7		int 21,44,8		int 21,44,9		int 21,44,a		int 21,44,b	
int 21,44,c		int 21,44,d		int 21,44,e		int 21,44,f		int 21,45	
int 21,46		int 21,47		int 21,48		int 21,49		int 21,4a	
int 21,4b		int 21,4c		int 21,4d		int 21,4e		int 21,4f	
int 21,5		int 21,50		int 21,51		int 21,52		int 21,53	
int 21,54		int 21,55		int 21,56		int 21,57		int 21,58	
int 21,59		int 21,5a		int 21,5b		int 21,5c		int 21,5d	
int 21,5e		int 21,5f		int 21,6		int 21,60		int 21,62	
int 21,63		int 21,64		int 21,65		int 21,66		int 21,67	
int 21,68		int 21,69		int 21,6c		int 21,7		int 21,8	
int 21,9		int 21,a		int 21,b		int 21,c		int 21,d	
int 21,e		int 21,f		int 21,f8		int 22		int 23	
int 24		int 25		int 26		int 27		int 28	
int 29		int 2a		int 2a,0		int 2a,1		int 2a,3	
int 2a,4		int 2a,5		int 2a,6		int 2a,80		int 2a,81	
int 2a,82		int 2a,84		int 2a,87		int 2e		int 2f	
int 2f,0		int 2f,1		int 2f,2		int 2f,2e		int 2f,3	
int 2f,4		int 2f,5		int 2f,6		int 2f,8		int 2f,80	
int 33		int 33,0		int 33,1		int 33,10		int 33,13	
int 33,14		int 33,15		int 33,16		int 33,17		int 33,18	
int 33,19		int 33,1a		int 33,1b		int 33,1c		int 33,1d	
int 33,1e		int 33,1f		int 33,2		int 33,20		int 33,21	
int 33,22		int 33,23		int 33,24		int 33,3		int 33,4	
int 33,5		int 33,6		int 33,7		int 33,8		int 33,9	
int 33,a		int 33,b		int 33,c		int 33,d		int 33,e	
int 33,f		int 4a		int 5		int 67		int 67,40	
int 67,41		int 67,42		int 67,43		int 67,44		int 67,45	
int 67,46		int 67,47		int 67,48		int 67,49		int 67,4a	
int 67,4b		int 67,4c		int 67,4d		int 67,4e		int 67,4f	
int 67,50		int 67,51		int 67,52		int 67,53		int 67,54	
int 67,55		int 67,56		int 67,57		int 67,58		int 67,59	
int 67,5a		int 67,5b		int 67,5c		int 67,5d		int 67,60	
int 70		int 8		int 9		int table		interrupt table	
INVARS		IOCTL,0		IOCTL,1		IOCTL,2		IOCTL,3	
IOCTL,4		IOCTL,5		IOCTL,6		IOCTL,7		IOCTL,8	
IOCTL,9		IOCTL,a		IOCTL,b		IOCTL,c		IOCTL,d	
IOCTL,e		IOCTL,f		IRQ		keyboard interrupt		mouse interrupt	
mouse services		multiplex interrupt		printer status		system interrupts		user timer routine	
video interrupt		video modes	
