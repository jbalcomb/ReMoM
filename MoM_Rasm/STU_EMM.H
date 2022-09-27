#ifndef STU_EMM_H
#define STU_EMM_H

/*
    [INT 67 - Expanded Memory Specification](https://www.stanislavs.org/helppc/int_67.html)
*/


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

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
unsigned int INT_67_4B(void);   /*  INT 67,4B  Get Handle Count                            */
/*  INT 67,4C  Get Page Count for Handle                   */
/*  INT 67,4D  Get Page Count for All Handles              */
/*  INT 67,4E  Get/Set Page Map Context                    */
/*  INT 67,4F  Get/Set Partial Page Map                    */
/*  INT 67,50  Map/Unmap Multiple Handle Pages             */
/*  INT 67,51  Reallocate Pages                            */
/*  INT 67,52  Get/Set Handle Attributes                   */
/*  INT 67,53  Get/Set Handle Name                         */
void INT_67_53(void);
void INT_67_53_00(void);
void INT_67_53_01(void);
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

#ifdef __cplusplus
}
#endif

#endif /* STU_EMM_H */
