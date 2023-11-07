

MoO2 Module: paragrph
near exact




void Mark_Paragraph(int16_t x, int16_t y, int16_t full_width, char * string)

loops over the paragraph string, until '\0'

would seem to need to calculate the width of a portion of the string up to the width
needs to acomodate string control codes
looks to be checking the byte after and the byte before
not sure about the exclusion areas


exclusion area can result in the line being skipped


if not skipped

