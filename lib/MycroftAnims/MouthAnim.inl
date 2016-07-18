
template<size_t N>
MouthAnim::MouthAnim(const char (&ANIM)[N][16], const int MS_PER_FRAME) :
ANIM(&(ANIM[0])), NUM_FRAMES(N), MS_PER_FRAME(MS_PER_FRAME) { }
