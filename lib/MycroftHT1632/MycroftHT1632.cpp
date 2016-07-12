#include "MycroftHT1632.h"

void MycroftHT1632::drawImagePgm(const char * img, char width, char height, char x, char y, int offset){
  char length = width*height/4;
  char mask;

  // Sanity checks
  if(y + height < 0 || x + width < 0 || y > COM_SIZE || x > OUT_SIZE)
    return;
  // After looking at the rest of this function, you may need one.

  // Copying Engine.
  // You are not expected to understand this.
  for(char i=0; i<width; ++i) {
    char carryover_y = 0; // Simply a copy of the last 4-bit word of img.
    char carryover_num = (y - (y & ~ 3)); // Number of digits carried over
    bool carryover_valid = false; // If true, there is data to be carried over.

    char loc_x = i + x;
    if(loc_x < 0 || loc_x >= OUT_SIZE) // Skip this column if it is out of range.
      continue;
    for(char j=0; j < (carryover_valid ? (height+4):height) ; j+=4) {
      char loc_y = j + y;
      if(loc_y <= -4 || loc_y >= COM_SIZE) // Skip this row if it is out of range.
        continue;
      // Direct copying possible when render is on boundaries.
      // The bit manipulation here is designed to copy from img only the relevant sections.

      // This mask is only not used when emptying the cache (for copying to non-4-bit aligned spaces)

      //if(j<height)
      //  mask = (height-loc_y >= 4)?0b00001111:(0b00001111 >> (4-(height-j))) & 0b00001111; // Mask bottom

      if(loc_y % 4 == 0) {
          mask = (height-loc_y >= 4)?0b00001111:(0b00001111 >> (4-(height-j))) & 0b00001111;
          int idx = (int)ceil((float)height/4.0f)*i + j/4 + offset;
          mem[_tgtBuffer][GET_ADDR_FROM_X_Y(loc_x,loc_y)] = (mem[_tgtBuffer][GET_ADDR_FROM_X_Y(loc_x,loc_y)] & (~mask) & 0b00001111) | (((char) pgm_read_byte(&(img[idx]))) & mask) | MASK_NEEDS_REWRITING;
      } else {
        // If carryover_valid is NOT true, then this is the first set to be copied.    (char) pgm_read_byte(&(anim[idx][j]))
        //   If loc_y > 0, preserve the contents of the pixels above, copy to mem, and then copy remaining
        //   data to the carry over buffer. If y loc_y < 0, just copy data to carry over buffer.
        //   It is expected that this section is only reached when j == 0.
        // COPY START
        if(!carryover_valid) {
          if(loc_y > 0) {
            mask = (height-loc_y >= 4)?0b00001111:(0b00001111 >> (4-(height-j))) & 0b00001111; // Mask bottom
            mask = (0b00001111 << carryover_num) & mask; // Mask top
            int idx = (int)ceil((float)height/4.0f)*i + j/4 + offset;
            mem[_tgtBuffer][GET_ADDR_FROM_X_Y(loc_x,loc_y)] = (mem[_tgtBuffer][GET_ADDR_FROM_X_Y(loc_x,loc_y)] & (~mask) & 0b00001111) | ((((char) pgm_read_byte(&(img[idx]))) << carryover_num) & mask) | MASK_NEEDS_REWRITING;
          }
          carryover_valid = true;
        } else {
          // COPY END
          if(j >= height) {
            // Its writing one line past the end.
            // Use this line to get rid of the final carry-over.
            mask = (0b00001111 >> (4 - carryover_num)) & 0b00001111; // Mask bottom
            mem[_tgtBuffer][GET_ADDR_FROM_X_Y(loc_x,loc_y)] = (mem[_tgtBuffer][GET_ADDR_FROM_X_Y(loc_x,loc_y)] & (~mask) & 0b00001111) | (carryover_y >> (4 - carryover_num) & mask) | MASK_NEEDS_REWRITING;
          // COPY MIDDLE
          } else {
            // There is data in the carry-over buffer. Copy that data and the values from the current cell into mem.
            // The inclusion of a carryover_num term is to account for the presence of the carryover data  when calculating the bottom clipping.
            mask = (height-loc_y >= 4)?0b00001111:(0b00001111 >> (4-(height+carryover_num-j))) & 0b00001111; // Mask bottom
            int idx = (int)ceil((float)height/4.0f)*i + j/4 + offset;
            mem[_tgtBuffer][GET_ADDR_FROM_X_Y(loc_x,loc_y)] = (mem[_tgtBuffer][GET_ADDR_FROM_X_Y(loc_x,loc_y)] & (~mask) & 0b00001111) | ((((char) pgm_read_byte(&(img[idx]))) << carryover_num) & mask) | (carryover_y >> (4 - carryover_num) & mask) | MASK_NEEDS_REWRITING;
          }
        }
        int idx = (int)ceil((float)height/4.0f)*i + j/4 + offset;
        carryover_y = ((char) pgm_read_byte(&(img[idx])));
      }
    }
  }
}

void MycroftHT1632::drawTextPgm(const char text [], int x, int y, const char font [], const char font_width [], char font_height, int font_glyph_step, char gutter_space) {
  int curr_x = x;
  char i = 0;
  char currchar;

  // Check if string is within y-bounds
  if(y + font_height < 0 || y >= COM_SIZE)
    return;

  while(true){
    if(text[i] == '\0')
      return;

    currchar = text[i] - 32;
    if(currchar >= 65 && currchar <= 90) // If character is lower-case, automatically make it upper-case
      currchar -= 32; // Make this character uppercase.
    if(currchar < 0 || currchar >= 64) { // If out of bounds, skip
      ++i;
      continue; // Skip this character.
    }
    // Check to see if character is not too far right.
    if(curr_x >= OUT_SIZE)
      break; // Stop rendering - all other characters are no longer within the screen

    // Check to see if character is not too far left.
    if(curr_x + font_width[currchar] + gutter_space >= 0){
      drawImagePgm(font, font_width[currchar], font_height, curr_x, y,  currchar*font_glyph_step);

      // Draw the gutter space
      for(char j = 0; j < gutter_space; ++j)
        drawImagePgm(font, 1, font_height, curr_x + font_width[currchar] + j, y, 0);

    }

    curr_x += font_width[currchar] + gutter_space;
    ++i;
  }
}
