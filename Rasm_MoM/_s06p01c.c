
void VGA_SetTextMode(void)
{
asm mov ax, 0x03
asm int 0x10
}
