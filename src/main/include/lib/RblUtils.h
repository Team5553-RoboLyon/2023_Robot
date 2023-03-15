#pragma once

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NF64
// +
// +--+ PI
#define NF64_PI 3.14159265358979323846       // PI
#define NF64_2PI 6.28318530717958647693      // 2*PI
#define NF64_PI_2 1.57079632679489661923     // PI/2
#define NF64_PI_3 1.04719755119659774615     // PI/3
#define NF64_PI_4 0.78539816339744830962     // PI/4
#define NF64_1_PI 0.31830988618379067154     // 1/PI
#define NF64_1_2PI 0.15915494309189533577    // 1/(2*PI)
#define NF64_2_PI 0.63661977236758134308     // 2/PI
#define NF64_1_SQRTPI 0.56418958354775628695 // 1/sqrt(PI)
#define NF64_2_SQRTPI 1.12837916709551257390 // 2/sqrt(PI)
// +
// +--+ SQRT(2)
// +
#define NF64_SQRT2 1.41421356237309504880   // Sqrt(2)
#define NF64_1_SQRT2 0.70710678118654752440 // 1/Sqrt(2)
#define NF64_SQRT2_2 0.70710678118654752440 // Sqrt(2)/2

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Quick MACROS functions
#define NDEGtoRAD(deg) ((NF64_PI / 180.0) * (deg)) // Deg -> Radian
#define NRADtoDEG(rad) ((180.0 / NF64_PI) * (rad)) // Radian -> Deg

#define NABS(a) (((a) < 0) ? -(a) : (a))
#define NMAX(a, b) (((a) > (b)) ? (a) : (b))
#define NMIN(a, b) (((a) < (b)) ? (a) : (b))
// #define NROUND(fval)	( ( (fval) >= 0.0 ) ? ((Ns32)((fval) + 0.5)) : ((Ns32)((fval) - 0.5)) )
#define NSIGN(a) (((a) < 0) ? -1 : 1)
#define NCLAMP(mn, a, mx) (((a) < (mn)) ? (mn) : ((a) > (mx)) ? (mx) \
                                                              : (a))
#define NLERP(a, b, t) (a + (b - a) * t)

// ########################## table de  bites ##########################
#define BITSET(val, bit_id) ((val) |= (1 << (bit_id)))
#define BITCLEAR(val, bit_id) ((val) &= ~(1 << (bit_id)))
// #define BITGET(val,bit_id)        ((val) &  (1 << (bit_id)))
#define BITGET(val, bit_id) (((val) >> (bit_id)) & 1) // 0 or 1