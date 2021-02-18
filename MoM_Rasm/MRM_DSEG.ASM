
;dseg:27FF
strCONFIG_MOM   db 'CONFIG.MOM',0

;dseg:280A
strRunInstall   db 'Run INSTALL to configure MASTER OF MAGIC.',0Ah,0Ah,0


;dseg:3D26
;(20)CR&LN (20)\r\n
strCRLF20       db 0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah,0Dh,0Ah0Dh,0Ah,0

;dseg:3D7B
strInsfMem1     db 'Insufficient memory. You need at least ',0
;dseg:3DA3
strInsfMem2     db 'K free. Try removing all TSR',27h,'s.',0
