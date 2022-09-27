
#include "STU_EMM.H"

/*
    Hey, Visual Studio Code. Sshhh....
*/
#define __VSCODE__ 1
#ifdef __VSCODE__
unsigned int _AH;
unsigned int _AL;
unsigned int _AX;
unsigned int _BH;
unsigned int _BL;
unsigned int _BX;
unsigned int _CH;
unsigned int _CL;
unsigned int _CX;
unsigned int _DH;
unsigned int _DL;
unsigned int _DX;
unsigned int _SI;
unsigned int _DI;
unsigned int _DS;
unsigned int _ES;
#endif

/*  INT 67,40  Get EMM Status                              */
/*  INT 67,41  Get Page Frame Base Address                 */
/*  INT 67,42  Get Page Counts                             */
/*  INT 67,43  Get Handle and Allocate Pages               */
/*  INT 67,44  Map Logical Page Into Physical Page Window  */
/*  INT 67,45  Release Handle and Memory Pages             */
/*  INT 67,46  Get EMM Version                             */
/*  INT 67,47  Save Page Map Context                       */
/*  INT 67,48  Restore Page Map Context                    */
/*  INT 67,49  Get I/O Port Addresses                      */
/*  INT 67,4A  Get Logical to Physical Page Mapping        */

/*
    [INT 67,4B - Get Handle Count (LIM EMS)](https://www.stanislavs.org/helppc/int_67-4b.html)
*/
unsigned int INT_67_4B(void)
{
    unsigned int EMM_Handle_Count;

    _AH = 0x4B;                 // INT 67,4B - Get Handle Count
    geninterrupt(0x67);         // EMS INT
    EMM_Handle_Count = _BX;

    return EMM_Handle_Count;
}

/*  INT 67,4C  Get Page Count for Handle                   */
/*  INT 67,4D  Get Page Count for All Handles              */
/*  INT 67,4E  Get/Set Page Map Context                    */
/*  INT 67,4F  Get/Set Partial Page Map                    */
/*  INT 67,50  Map/Unmap Multiple Handle Pages             */
/*  INT 67,51  Reallocate Pages                            */
/*  INT 67,52  Get/Set Handle Attributes                   */
/*  INT 67,53  Get/Set Handle Name                         */

/*
    [INT 67,53 Get/Set Handle Name (LIM EMS 4.0+)](https://www.stanislavs.org/helppc/int_67-53.html)
*/
void INT_67_53(void)
{
    const char * EMM_Handle_Name;
    unsigned int EMM_Handle_Number;
    _AH = 0x53;                 // INT 67,53 Get/Set Handle Name
    _AL = 0x00;
    geninterrupt(0x67);         // EMS INT
}
void INT_67_53_00(void)
{
    const char * EMM_Handle_Name;
    unsigned int EMM_Handle_Number;
    _AH = 0x53;                 // INT 67,53 Get/Set Handle Name
    _AL = 0x00;                 // Function Get Handle Name
    geninterrupt(0x67);         // EMS INT
}
void INT_67_53_01(void)
{
    const char * EMM_Handle_Name;
    unsigned int EMM_Handle_Number;
    _AH = 0x53;                 // INT 67,53 Get/Set Handle Name
    _AL = 0x01;                 // Function Set Handle Name
    geninterrupt(0x67);         // EMS INT
}

/*  INT 67,54  Get Handle Directory                        */
/*  INT 67,55  Alter Page Map and Jump                     */
/*  INT 67,56  Alter Page Map and Call                     */
/*  INT 67,57  Move/Exchange Memory Region                 */
/*  INT 67,58  Get Mappable Physical Address Array         */
/*  INT 67,59  Get Expanded Memory Hardware Information    */
/*  INT 67,5A  Allocate Standard/Raw Pages                 */
/*  INT 67,5B  Alternate Map Register Set DMA Registers    */
/*  INT 67,5C  Prepare Expanded Memory for Warm Boot       */
/*  INT 67,5D  Enable/Disable OS Functions                 */
/*  INT 67,60  Get Physical Window Array                   */
