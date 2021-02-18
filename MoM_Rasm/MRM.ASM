	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif
	?debug	V 300h
	?debug	S "mrm.cpp"
	?debug	C E92CAC5152076D726D2E637070
	?debug	C E96A7B4E521B433A5C424F524C414E44435C494E434C5544455C61+
	?debug	C 6C6C6F632E68
	?debug	C E96A7B4E521B433A5C424F524C414E44435C494E434C5544455C5F+
	?debug	C 646566732E68
	?debug	C E96A7B4E521B433A5C424F524C414E44435C494E434C5544455C5F+
	?debug	C 6E756C6C2E68
	?debug	C E96A7B4E521B433A5C424F524C414E44435C494E434C5544455C62+
	?debug	C 696F732E2068
	?debug	C E96A7B4E521B433A5C424F524C414E44435C494E434C5544455C63+
	?debug	C 6F6E696F2E68
	?debug	C E96A7B4E521B433A5C424F524C414E44435C494E434C5544455C73+
	?debug	C 7464696F2E68
	?debug	C E96A7B4E521A433A5C424F524C414E44435C494E434C5544455C74+
	?debug	C 696D652E68
	?debug	C E96A7B4E5205444F532E48
MRM_TEXT	segment byte public 'CODE'
MRM_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:MRM_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_DATA	segment word public 'DATA'
_Repetitions	label	word
	db	2 dup (?)
_g_RequiredMem	label	word
	db	2 dup (?)
_strCONFIG_MOM	label	dword
	dd	DGROUP:s@
_strRunInstall	label	dword
	dd	DGROUP:s@+11
_DATA	ends
MRM_TEXT	segment byte public 'CODE'
	?debug	C E801076D726D2E6370702CAC5152
   ;	
   ;	void print_startup_information()
   ;	
	?debug	L 64
	assume	cs:MRM_TEXT
@print_startup_information$qv	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	sub	sp,16
	?debug	B
   ;	
   ;	{
   ;	    long bios_time;
   ;	    _bios_timeofday(_TIME_GETCLOCK, &bios_time);
   ;	
	?debug	L 67
	push	ss
	lea	ax,word ptr [bp-4]
	push	ax
	xor	ax,ax
	push	ax
	call	far ptr __bios_timeofday
	add	sp,6
   ;	
   ;	    clrscr() ;
   ;	
	?debug	L 68
	call	far ptr _clrscr
   ;	
   ;	    cprintf("The number of clock ticks since midnight is:\r\n");
   ;	
	?debug	L 69
	push	ds
	mov	ax,offset DGROUP:s@+55
	push	ax
	call	far ptr _cprintf
	pop	cx
	pop	cx
   ;	
   ;	    cprintf("The number of seconds since midnight is:\r\n");
   ;	
	?debug	L 70
	push	ds
	mov	ax,offset DGROUP:s@+102
	push	ax
	call	far ptr _cprintf
	pop	cx
	pop	cx
   ;	
   ;	    cprintf("The number of minutes since midnight is:\r\n");
   ;	
	?debug	L 71
	push	ds
	mov	ax,offset DGROUP:s@+145
	push	ax
	call	far ptr _cprintf
	pop	cx
	pop	cx
   ;	
   ;	    cprintf("The number of hours since midnight is:\r\n");
   ;	
	?debug	L 72
	push	ds
	mov	ax,offset DGROUP:s@+188
	push	ax
	call	far ptr _cprintf
	pop	cx
	pop	cx
   ;	
   ;	    cprintf("\r\nPress any key to quit:");
   ;	
	?debug	L 73
	push	ds
	mov	ax,offset DGROUP:s@+229
	push	ax
	call	far ptr _cprintf
	pop	cx
	pop	cx
	jmp	@1@86
@1@58:
   ;	
   ;	    while(!kbhit()) {
   ;	        _bios_timeofday(_TIME_GETCLOCK, &bios_time);
   ;	
	?debug	L 75
	push	ss
	lea	ax,word ptr [bp-4]
	push	ax
	xor	ax,ax
	push	ax
	call	far ptr __bios_timeofday
	add	sp,6
   ;	
   ;	        gotoxy (50,1);
   ;	
	?debug	L 76
	mov	ax,1
	push	ax
	mov	ax,50
	push	ax
	call	far ptr _gotoxy
	pop	cx
	pop	cx
   ;	
   ;	        cprintf("%lu", bios_time);
   ;	
	?debug	L 77
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	ds
	mov	ax,offset DGROUP:s@+254
	push	ax
	call	far ptr _cprintf
	add	sp,8
   ;	
   ;	        gotoxy(50,2);
   ;	
	?debug	L 78
	mov	ax,2
	push	ax
	mov	ax,50
	push	ax
	call	far ptr _gotoxy
	pop	cx
	pop	cx
   ;	
   ;	        cprintf("%.4f", bios_time / CLK_TCK);
   ;	
	?debug	L 79
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	mov	word ptr [bp-8],dx
	mov	word ptr [bp-6],ax
	fild	dword ptr [bp-8]
	fdiv	qword ptr DGROUP:s@+263
	sub	sp,8
	fstp	qword ptr [bp-24]
	push	ds
	mov	ax,offset DGROUP:s@+258
	push	ax
	fwait	
	call	far ptr _cprintf
	add	sp,12
   ;	
   ;	        gotoxy(50,3);
   ;	
	?debug	L 80
	mov	ax,3
	push	ax
	mov	ax,50
	push	ax
	call	far ptr _gotoxy
	pop	cx
	pop	cx
   ;	
   ;	        cprintf("%.4f", bios_time / CLK_TCK / 60);
   ;	
	?debug	L 81
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	mov	word ptr [bp-8],dx
	mov	word ptr [bp-6],ax
	fild	dword ptr [bp-8]
	fdiv	qword ptr DGROUP:s@+263
	fdiv	dword ptr DGROUP:s@+276
	sub	sp,8
	fstp	qword ptr [bp-24]
	push	ds
	mov	ax,offset DGROUP:s@+271
	push	ax
	fwait	
	call	far ptr _cprintf
	add	sp,12
   ;	
   ;	        gotoxy(50,4);
   ;	
	?debug	L 82
	mov	ax,4
	push	ax
	mov	ax,50
	push	ax
	call	far ptr _gotoxy
	pop	cx
	pop	cx
   ;	
   ;	        cprintf("%.4f", bios_time / CLK_TCK / 3600);
   ;	
	?debug	L 83
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	mov	word ptr [bp-8],dx
	mov	word ptr [bp-6],ax
	fild	dword ptr [bp-8]
	fdiv	qword ptr DGROUP:s@+263
	fdiv	dword ptr DGROUP:s@+285
	sub	sp,8
	fstp	qword ptr [bp-24]
	push	ds
	mov	ax,offset DGROUP:s@+280
	push	ax
	fwait	
	call	far ptr _cprintf
	add	sp,12
@1@86:
	?debug	L 74
	call	far ptr _kbhit
	or	ax,ax
	jne	@@0
	jmp	@1@58
@@0:
   ;	
   ;	    }
   ;	    
   ;	    printf("\n\n\n");
   ;	
	?debug	L 86
	push	ds
	mov	ax,offset DGROUP:s@+289
	push	ax
	call	far ptr _printf
	pop	cx
	pop	cx
   ;	
   ;	    
   ;	    void * heapbase;
   ;	    printf("The stack length is: %u\n", _stklen);
   ;	
	?debug	L 89
	push	word ptr DGROUP:__stklen
	push	ds
	mov	ax,offset DGROUP:s@+293
	push	ax
	call	far ptr _printf
	add	sp,6
   ;	
   ;	    printf("The heap length is: %u\n", _heaplen);
   ;	
	?debug	L 90
	push	word ptr DGROUP:__heaplen
	push	ds
	mov	ax,offset DGROUP:s@+318
	push	ax
	call	far ptr _printf
	add	sp,6
   ;	
   ;	    
   ;	    heapbase = sbrk(0);
   ;	
	?debug	L 92
	xor	ax,ax
	push	ax
	call	far ptr _sbrk
	pop	cx
	mov	word ptr [bp-10],dx
	mov	word ptr [bp-12],ax
   ;	
   ;	    printf("The heap base is: 0x%x\n", heapbase);
   ;	
	?debug	L 93
	push	word ptr [bp-10]
	push	word ptr [bp-12]
	push	ds
	mov	ax,offset DGROUP:s@+342
	push	ax
	call	far ptr _printf
	add	sp,8
   ;	
   ;	    
   ;	    /*
   ;	        _psp is the starting address of the program in memory.
   ;	        The top of the stack is the end of the program.
   ;	        Using _SS and _SP together we can get the end of the stack.
   ;	        (_SS + ((_SP/16) - _psp)
   ;	    */
   ;	    /*
   ;	        _psp
   ;	            Function    Contains the segment address of the program segment prefix (PSP) for the
   ;	                        current program.
   ;	            Syntax      extern unsigned int _psp;
   ;	            Declared in dos.h
   ;	        The PSP is a DOS process descriptor; it contains initial DOS information
   ;	        about the program.
   ;	        Refer to the DOS Programmer's Reference Manual for more information on
   ;	        the PSP.
   ;	    */
   ;	    printf("The _psp is: 0x%x\n", _psp);
   ;	
	?debug	L 112
	push	word ptr DGROUP:__psp
	push	ds
	mov	ax,offset DGROUP:s@+366
	push	ax
	call	far ptr _printf
	add	sp,6
   ;	
   ;	    
   ;	    char far * dta;
   ;	    dta = getdta();
   ;	
	?debug	L 115
	call	far ptr _getdta
	mov	word ptr [bp-14],dx
	mov	word ptr [bp-16],ax
   ;	
   ;	    printf("The current disk transfer address is: %Fp\n", dta);
   ;	
	?debug	L 116
	push	word ptr [bp-14]
	push	word ptr [bp-16]
	push	ds
	mov	ax,offset DGROUP:s@+385
	push	ax
	call	far ptr _printf
	add	sp,8
   ;	
   ;	    
   ;	}
   ;	
	?debug	L 118
	mov	sp,bp
	pop	bp
	ret	
	?debug	C E318000400160200
	?debug	C E319000400160100
	?debug	C E6036474611802F0FF0008686561706261736519+
	?debug	C 02F4FF000962696F735F74696D650602FCFF00
	?debug	E
	?debug	E
@print_startup_information$qv	endp
   ;	
   ;	int main()
   ;	
	?debug	L 121
	assume	cs:MRM_TEXT
_main	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	?debug	B
   ;	
   ;	{
   ;	    print_startup_information();
   ;	
	?debug	L 123
	push	cs
	call	near ptr @print_startup_information$qv
   ;	
   ;	    
   ;	    printf("\n\n\n");
   ;	
	?debug	L 125
	push	ds
	mov	ax,offset DGROUP:s@+428
	push	ax
	call	far ptr _printf
	pop	cx
	pop	cx
   ;	
   ;	    
   ;	    char * varFileName;
   ;	    int result;
   ;	    
   ;	    /*
   ;	    int i;
   ;	    Repetitions = 10;
   ;	    StartingValue = 2;
   ;	    printf("i: %d\n", i);
   ;	    printf("Repetitions: %d\n", Repetitions);
   ;	    printf("StartingValue: %d\n", StartingValue);
   ;	    printf("DoTotal(): %d\n", DoTotal());
   ;	    */
   ;	    
   ;	    /*
   ;	    printf("REQUIRED_MEM: %d\n", REQUIRED_MEM);
   ;	    printf("g_RequiredMem: %d\n", g_RequiredMem);
   ;	    ReqdMem(REQUIRED_MEM);
   ;	    printf("g_RequiredMem: %d\n", g_RequiredMem);
   ;	    */
   ;	
   ;	    result = ST_DosFindFile(strCONFIG_MOM, varFileName);
   ;	
	?debug	L 147
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr DGROUP:_strCONFIG_MOM+2
	push	word ptr DGROUP:_strCONFIG_MOM
	call	far ptr _ST_DosFindFile
	add	sp,8
	mov	si,ax
   ;	
   ;	    printf("ST_DosFindFile(): %d\n\n", result);
   ;	
	?debug	L 148
	push	si
	push	ds
	mov	ax,offset DGROUP:s@+432
	push	ax
	call	far ptr _printf
	add	sp,6
   ;	
   ;	    
   ;	    result = f040101_DosFindFile(strCONFIG_MOM, varFileName);
   ;	
	?debug	L 150
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr DGROUP:_strCONFIG_MOM+2
	push	word ptr DGROUP:_strCONFIG_MOM
	call	far ptr _f040101_DosFindFile
	add	sp,8
	mov	si,ax
   ;	
   ;	    printf("f040101_DosFindFile(): %d\n\n", result);
   ;	
	?debug	L 151
	push	si
	push	ds
	mov	ax,offset DGROUP:s@+455
	push	ax
	call	far ptr _printf
	add	sp,6
   ;	
   ;	    
   ;	    result = f010109_main();
   ;	
	?debug	L 153
	call	far ptr _f010109_main
	mov	si,ax
   ;	
   ;	    /*printf("f010109_main(): %d\n\n", result);*/
   ;	    
   ;	    return 0;
   ;	
	?debug	L 156
	xor	ax,ax
	jmp	short @2@58
@2@58:
   ;	
   ;	}
   ;	
	?debug	L 157
	pop	si
	mov	sp,bp
	pop	bp
	ret	
	?debug	C E606726573756C74040406000B76617246696C65+
	?debug	C 4E616D651802FCFF00
	?debug	E
	?debug	E
_main	endp
	?debug	C E9
	?debug	C FA00000000
MRM_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'CONFIG.MOM'
	db	0
	db	'Run INSTALL to configure MASTER OF MAGIC.'
	db	10
	db	10
	db	0
	db	'The number of clock ticks since midnight is:'
	db	13
	db	10
	db	0
	db	'The number of seconds since midnight is:'
	db	13
	db	10
	db	0
	db	'The number of minutes since midnight is:'
	db	13
	db	10
	db	0
	db	'The number of hours since midnight is:'
	db	13
	db	10
	db	0
	db	13
	db	10
	db	'Press any key to quit:'
	db	0
	db	'%lu'
	db	0
	db	'%.4f'
	db	0
	db	'3333332@%.4f'
	db	0
	db	0
	db	0
	db	'pB%.4f'
	db	0
	db	0
	db	0
	db	'aE'
	db	10
	db	10
	db	10
	db	0
	db	'The stack length is: %u'
	db	10
	db	0
	db	'The heap length is: %u'
	db	10
	db	0
	db	'The heap base is: 0x%x'
	db	10
	db	0
	db	'The _psp is: 0x%x'
	db	10
	db	0
	db	'The current disk transfer address is: %Fp'
	db	10
	db	0
	db	10
	db	10
	db	10
	db	0
	db	'ST_DosFindFile(): %d'
	db	10
	db	10
	db	0
	db	'f040101_DosFindFile(): %d'
	db	10
	db	10
	db	0
_DATA	ends
MRM_TEXT	segment byte public 'CODE'
MRM_TEXT	ends
	public	_main
	public	@print_startup_information$qv
	public	_strRunInstall
	public	_strCONFIG_MOM
	public	_g_RequiredMem
	public	_Repetitions
	extrn	_f040101_DosFindFile:far
	extrn	_f010109_main:far
	extrn	_ST_DosFindFile:far
	extrn	_getdta:far
_pokeb	equ	pokeb
_poke	equ	poke
_peekb	equ	peekb
_peek	equ	peek
	extrn	__stklen:word
	extrn	__heaplen:word
	extrn	__psp:word
	extrn	_printf:far
	extrn	_cprintf:far
	extrn	_kbhit:far
	extrn	_gotoxy:far
	extrn	_clrscr:far
	extrn	__bios_timeofday:far
	extrn	_sbrk:far
_s@	equ	s@
	?debug	C EA050C
	?debug	C E31A00000023040400
	?debug	C EC055F6D61696E1A1800
	?debug	C E31B00000023010400
	?debug	C EC1D407072696E745F737461727475705F696E66+
	?debug	C 6F726D6174696F6E2471761B1800
	?debug	C EC0E5F73747252756E496E7374616C6C180000
	?debug	C EC0E5F737472434F4E4649475F4D4F4D180000
	?debug	C EC0E5F675F52657175697265644D656D040000
	?debug	C EC0C5F52657065746974696F6E73040000
	?debug	C E31C00000023040400
	?debug	C EB145F663034303130315F446F7346696E644669+
	?debug	C 6C651C00
	?debug	C E31D00000023040400
	?debug	C EB0D5F663031303130395F6D61696E1D00
	?debug	C E31E00000023040400
	?debug	C EB0F5F53545F446F7346696E6446696C651E00
	?debug	C E31F00000023180400
	?debug	C EB075F6765746474611F00
	?debug	C E32000000023010400
	?debug	C E32100000023010400
	?debug	C E32200000023020400
	?debug	C E32300000023040400
	?debug	C E3240666696E645F742B001E01
	?debug	C E3250015001A02
	?debug	C E326000D001A02
	?debug	C E327056666626C6B2B001E08
	?debug	C E3280015001A02
	?debug	C E329000D001A02
	?debug	C E32A09646F73646174655F7405001E0F
	?debug	C E32B046461746504001E14
	?debug	C E32C09646F7374696D655F7404001E18
	?debug	C E32D0474696D6504001E1D
	?debug	C E32E0664657668647212001E22
	?debug	C E32F0008001A02
	?debug	C E33007666174696E666F06001E28
	?debug	C E3310A6469736B667265655F7408001E2D
	?debug	C E33205646672656508001E32
	?debug	C E33308444F534552524F5205001E37
	?debug	C E33407434F554E54525922001E3C
	?debug	C E3350005001A02
	?debug	C E3360002001A02
	?debug	C E3370002001A02
	?debug	C E3380002001A02
	?debug	C E3390002001A02
	?debug	C E33A0002001A02
	?debug	C E33B000A001A02
	?debug	C E33C04786663622C001E49
	?debug	C E33D0005001A02
	?debug	C E33E0366636225001E4E
	?debug	C E33F0008001A02
	?debug	C E3400003001A02
	?debug	C E341000A001A02
	?debug	C EB085F5F73746B6C656E0A00
	?debug	C EB095F5F686561706C656E0A00
	?debug	C EB055F5F7073700A00
	?debug	C E34202746D12001E59
	?debug	C E34300000023040401
	?debug	C EB075F7072696E74664300
	?debug	C E34400000023040401
	?debug	C EB085F637072696E74664400
	?debug	C E34500000023040400
	?debug	C EB065F6B626869744500
	?debug	C E34600000023010400
	?debug	C EB075F676F746F78794600
	?debug	C E34700000023010400
	?debug	C EB075F636C727363724700
	?debug	C E34809746578745F696E666F0B001E63
	?debug	C E349000000230A0400
	?debug	C EB105F5F62696F735F74696D656F666461794900
	?debug	C E34A075245475041434B14001E6F
	?debug	C E34B05535245475308001E7A
	?debug	C E34C045245475310001F7F
	?debug	C E34D08574F52445245475310001E8082
	?debug	C E34E08425954455245475308001E808B
	?debug	C E34F0A6469736B696E666F5F740E001E8094
	?debug	C E35000000023190400
	?debug	C EB055F7362726B5000
	?debug	C E3510B66617268656170696E666F0A001E809B
	?debug	C E352000400160101
	?debug	C E607636C6F636B5F740606000674696D655F7406+
	?debug	C 06000666706F735F740606000673697A655F740A+
	?debug	C 060009707472646966665F740606000666696E64+
	?debug	C 5F74240700056666626C6B27070009646F736461+
	?debug	C 74655F742A070004646174652B070009646F7374+
	?debug	C 696D655F742C07000474696D652D070006646576+
	?debug	C 6864722E070007666174696E666F3007000A6469+
	?debug	C 736B667265655F74310700056466726565320700+
	?debug	C 08444F534552524F5233070007434F554E545259+
	?debug	C 34070004786663623C0700036663623E07000274+
	?debug	C 6D42070009746578745F696E666F480700075245+
	?debug	C 475041434B4A07000553524547534B0700045245+
	?debug	C 47534C07000842595445524547534E070008574F+
	?debug	C 5244524547534D07000A6469736B696E666F5F74+
	?debug	C 4F07000B66617268656170696E666F510700
	?debug	F pokeb 32 24 0
	?debug	F poke 33 24 0
	?debug	F peekb 34 24 0
	?debug	F peek 35 24 0
	?debug	C E200087265736572766564250006617474726962+
	?debug	C 02000777725F74696D650A000777725F64617465+
	?debug	C 0A000473697A650600046E616D6526C02B000000
	?debug	C E2000B66665F726573657276656428000966665F+
	?debug	C 61747472696202000866665F6674696D650A0008+
	?debug	C 66665F66646174650A000866665F6673697A6506+
	?debug	C 000766665F6E616D6529C02B000000
	?debug	C E200036461790800056D6F6E7468080004796561+
	?debug	C 720A00096461796F667765656B08C005000000
	?debug	C E2000764615F7965617204000664615F64617902+
	?debug	C 000664615F6D6F6E02C004000000
	?debug	C E20004686F75720800066D696E75746508000673+
	?debug	C 65636F6E64080007687365636F6E6408C0040000+
	?debug	C 00
	?debug	C E2000674695F6D696E08000774695F686F757208+
	?debug	C 000774695F68756E6408000674695F73656308C0+
	?debug	C 04000000
	?debug	C E2000764685F6E65787406000764685F61747472+
	?debug	C 04000864685F73747261740A000864685F696E74+
	?debug	C 65720A000764685F6E616D652FC012000000
	?debug	C E2000866695F73636C757302000866695F666174+
	?debug	C 696402000866695F6E636C75730A000866695F62+
	?debug	C 7973656304C006000000
	?debug	C E2000E746F74616C5F636C7573746572730A000E+
	?debug	C 617661696C5F636C7573746572730A0013736563+
	?debug	C 746F72735F7065725F636C75737465720A001062+
	?debug	C 797465735F7065725F736563746F720AC0080000+
	?debug	C 00
	?debug	C E2000864665F617661696C0A000864665F746F74+
	?debug	C 616C0A000764665F627365630A000864665F7363+
	?debug	C 6C75730AC008000000
	?debug	C E2000B64655F6578746572726F7204000864655F+
	?debug	C 636C61737302000964655F616374696F6E020008+
	?debug	C 64655F6C6F63757302C005000000
	?debug	C E20007636F5F64617465040007636F5F63757272+
	?debug	C 350008636F5F7468736570360008636F5F646573+
	?debug	C 6570370008636F5F6474736570380008636F5F74+
	?debug	C 6D73657039000C636F5F637572727374796C6502+
	?debug	C 0009636F5F646967697473020007636F5F74696D+
	?debug	C 65020007636F5F63617365060008636F5F646173+
	?debug	C 65703A0007636F5F66696C6C3BC022000000
	?debug	C E20009786663625F666C6167020009786663625F+
	?debug	C 726573763D0009786663625F6174747202000878+
	?debug	C 6663625F6663623EC02C000000
	?debug	C E200096663625F64726976650200086663625F6E+
	?debug	C 616D653F00076663625F65787440000A6663625F+
	?debug	C 637572626C6B04000B6663625F72656373697A65+
	?debug	C 04000B6663625F66696C73697A65060008666362+
	?debug	C 5F646174650400086663625F7265737641000A66+
	?debug	C 63625F63757272656302000A6663625F72616E64+
	?debug	C 6F6D06C025000000
	?debug	C E20006746D5F736563040006746D5F6D696E0400+
	?debug	C 07746D5F686F7572040007746D5F6D6461790400+
	?debug	C 06746D5F6D6F6E040007746D5F79656172040007+
	?debug	C 746D5F77646179040007746D5F79646179040008+
	?debug	C 746D5F697364737404C012000000
	?debug	C E2000777696E6C65667408000677696E746F7008+
	?debug	C 000877696E726967687408000977696E626F7474+
	?debug	C 6F6D0800096174747269627574650800086E6F72+
	?debug	C 6D61747472080008637572726D6F646508000C73+
	?debug	C 637265656E68656967687408000B73637265656E+
	?debug	C 7769647468080004637572780800046375727908+
	?debug	C C00B000000
	?debug	C E20004725F61780A0004725F62780A0004725F63+
	?debug	C 780A0004725F64780A0004725F62700A0004725F+
	?debug	C 73690A0004725F64690A0004725F64730A000472+
	?debug	C 5F65730A0007725F666C6167730AC014000000
	?debug	C E2000265730A000263730A000273730A00026473+
	?debug	C 0AC008000000
	?debug	C E20001784D0001684EC010000000
	?debug	C E2000261780A000262780A000263780A00026478+
	?debug	C 0A000273690A000264690A000563666C61670A00+
	?debug	C 05666C6167730AC010000000
	?debug	C E20002616C0800026168080002626C0800026268+
	?debug	C 080002636C0800026368080002646C0800026468+
	?debug	C 08C008000000
	?debug	C E2000564726976650A0004686561640A00057472+
	?debug	C 61636B0A0006736563746F720A00086E73656374+
	?debug	C 6F72730A000662756666657219C00E000000
	?debug	C E2000370747252000473697A650C0006696E5F75+
	?debug	C 736504C00A000000
	end
