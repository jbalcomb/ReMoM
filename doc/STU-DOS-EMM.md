

MOX/EMM.C
MOX/EMM.H
STU/STU_EMM.C
STU/STU_EMM.H

MAGIC.EXE       seg012; seg013;
WIZARDS.EXE     seg012; seg013;
    seg012  C code
    seg013  Assembly code










J:\STU_J\STU\DBWD\developc\__DOS_EMS_OPC\





EMM_MakeNamedHandle()


mov     ah, 43h
mov     bx, [bp+Page_Count]
int     67h                             ;  - LIM EMS - GET HANDLE AND ALLOCATE MEMORY
                                        ; BX = number of logical pages to allocate
                                        ; Return: AH = status
cmp     ah, 0
jnz     short loc_16B81

mov     [bp+EMM_Handle], dx


mov     ax, 5301h
mov     si, [bp+Handle_Name]
int     67h                             ;  - LIM EMS 4.0 - GET/SET HANDLE NAME
                                        ; AL = 00h get handle name / 01h set handle name,
                                        ; DS:SI -> 8-byte handle name, DX = handle
                                        ; Return: AH = status
cmp     ah, 0
jnz     short loc_16B81

mov     ax, [bp+EMM_Handle]

loc_16B81:
mov     ax, e_ST_FAILURE





J:\STU_J\STU\DBWD\develfnd\MPL13\SAMPCODE\MSJ\MSJV2_1\EMS\EMS.ALL

==============================================================================
==============================================================================

Figure 5: Check_Status Subprocedure
==============================================================================

check_status (emm_status)
unsigned int emm_status;
{
static char *emm_error_strings[] = {
  "no error",
  "EMM software malfunction",
  "EMM hardware malfunction",
  "RESERVED",
  "Invalid EMM handle",
  "Invalid EMM function code",
  "All EMM handles being used",
  "Save/restore page mapping context error",
  "Not enough expanded memory pages",
  "Not enough unallocated pages",
  "Can not allocate zero pages",
  "Logical page out of range",
  "Physical page out of range",
  "Page mapping hardware state save area full",
  "Page mapping hardware state save area already has handle",
  "No handle associated with the page mapping hardware state save area",
  "Invalid subfunction"
 };

/*  IF EMM error, THEN print error message and EXIT  */

   if (emm_status != 0)                    /*  IF EMM error...  */
    {
      emm_status -= 0x7F;                  /*  Make error code 
                                               zero-based  */
      printf ("\x07Abort: EMM error = ");  /*  Issue error prefix  */
      printf ("%s\n", emm_error_strings[emm_status]);
                                           /*  Issue actual error 
                                               message  */
      exit(0);                             /*  And then exit to 
                                               DOS  */
    }
}
==============================================================================






dosbox-staging
ems.cpp
static Bitu INT67_Handler(void)
switch (reg_ah)
    case 0x43:        /* Get Handle and Allocate Pages */
        reg_ah=EMM_AllocateMemory(reg_bx,reg_dx,false);
        break;
	case 0x53: // Set/Get Handlename
		reg_ah=GetSetHandleName();
		break;

static uint8_t EMM_AllocateMemory(uint16_t pages,uint16_t & dhandle,bool can_allocate_zpages)
    mem = MEM_AllocatePages(pages*4,false);
    emm_handles[handle].pages = pages;
    emm_handles[handle].mem = mem;
    dhandle = handle;

static uint8_t GetSetHandleName(void)
	switch (reg_al)
	case 0x00:	/* Get Handle Name */
		if (handle>=EMM_MAX_HANDLES || emm_handles[handle].pages==NULL_HANDLE) return EMM_INVALID_HANDLE;
		MEM_BlockWrite(SegPhys(es)+reg_di,emm_handles[handle].name,8);
		break;
	case 0x01:	/* Set Handle Name */
		if (handle>=EMM_MAX_HANDLES || emm_handles[handle].pages==NULL_HANDLE) return EMM_INVALID_HANDLE;
		MEM_BlockRead(SegPhys(es)+reg_di,emm_handles[handle].name,8);
		break;

/* These don't check for alignment, better be sure it's correct */
void MEM_BlockWrite(PhysPt pt, const void *data, size_t size);
void MEM_BlockRead(PhysPt pt, void *data, Bitu size);

C:\STU\developp\dosbox-staging\src\hardware\memory.cpp

void MEM_BlockRead(PhysPt pt,void * data,Bitu size) {
	uint8_t * write=reinterpret_cast<uint8_t *>(data);
	while (size--) {
		*write++=mem_readb_inline(pt++);
	}
}

paging.h
template <MemOpMode op_mode = MemOpMode::WithBreakpoints>
static inline uint8_t mem_readb_inline(const PhysPt address)
{
	if constexpr (op_mode == MemOpMode::WithBreakpoints) {
		DEBUG_UpdateMemoryReadBreakpoints<uint8_t>(address);
	}
	HostPt tlb_addr = get_tlb_read(address);
	if (tlb_addr) {
		return host_readb(tlb_addr + address);
	} else {
		return (get_tlb_readhandler(address))->readb(address);
	}
}




# RefMat

https://www.stanislavs.org/helppc/int_67.html

https://github.com/dosbox-staging/dosbox-staging/blob/main/src/ints/ems.cpp

J:\STU_J\STU\DBWD\develfnd\MPL13\SAMPCODE\MSJ\MSJV2_1\EMS\EMS.ALL
J:\STU_J\STU\DBWD\develfnd\MPL13\SAMPCODE\MSJ\MSJV3_1\LIM4\



https://www.stanislavs.org/helppc/int_67.html

INT 67 - Expanded Memory Specification

    For more information, see the following topics:

    INT 67,40  Get EMM Status
    INT 67,41  Get Page Frame Base Address
    INT 67,42  Get Page Counts
    INT 67,43  Get Handle and Allocate Pages
    INT 67,44  Map Logical Page Into Physical Page Window
    INT 67,45  Release Handle and Memory Pages
    INT 67,46  Get EMM Version
    INT 67,47  Save Page Map Context
    INT 67,48  Restore Page Map Context
    INT 67,49  Get I/O Port Addresses
    INT 67,4A  Get Logical to Physical Page Mapping
    INT 67,4B  Get Handle Count
    INT 67,4C  Get Page Count for Handle
    INT 67,4D  Get Page Count for All Handles
    INT 67,4E  Get/Set Page Map Context
    INT 67,4F  Get/Set Partial Page Map
    INT 67,50  Map/Unmap Multiple Handle Pages
    INT 67,51  Reallocate Pages
    INT 67,52  Get/Set Handle Attributes
    INT 67,53  Get/Set Handle Name
    INT 67,54  Get Handle Directory
    INT 67,55  Alter Page Map and Jump
    INT 67,56  Alter Page Map and Call
    INT 67,57  Move/Exchange Memory Region
    INT 67,58  Get Mappable Physical Address Array
    INT 67,59  Get Expanded Memory Hardware Information
    INT 67,5A  Allocate Standard/Raw Pages
    INT 67,5B  Alternate Map Register Set DMA Registers
    INT 67,5C  Prepare Expanded Memory for Warm Boot
    INT 67,5D  Enable/Disable OS Functions
    INT 67,60  Get Physical Window Array

    - function number is specified in AH
    - see    EMS STATUS


