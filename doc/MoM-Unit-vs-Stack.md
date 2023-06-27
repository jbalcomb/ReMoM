

MoM vs. MoX
? MoO2: "ship" -> "fleet" ?
"unit" -> "stack"

? elsewhere, "troop", "army" ?





The Unit Index of the Unit in the Unit Stack

array of 9 int_16 
int16_t active_unit_stack_unit_indices
count of indices in the array
~ UNIT_STACK_UNIT_COUNT_MAX
? There is only ever 1 'Unit Stack' ?



//drake178
"STK_" - ? touches the _unit_stack and/ot _unit_stack_count ?


WIZARDS.EXE
ovr061

ovr062


WZD ovr061



WZD ovr061

STK_GetMovableUnits
fills the return arguments with a count and list of
the units in the selected stack that can move
(according to the global UNIT_Stack array)
