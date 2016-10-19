/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
       return ~(~x | ~y);
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  int shift = n << 3;
  return (x >> shift) & 0xff;

}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  // shift 32 is undefined (result equal 1), !!
  int k = 32 + (~n);
  int mask = ((1 << k) <<1) + (~0);
  return (x >> n) & mask;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  /*
  // group 1's by 2
  int m = (0x55 << 8) + 0x55;
  int mask = (m << 16) + m;
  x = (x & mask) + ((x >> 1) & mask);
  
  m = (0x33 << 8) + 0x33;
  mask = (m << 16) + m;
  x = (x & mask) + ((x >> 2) & mask);

  m = (0x0f << 8) + 0x0f;
  mask = (m << 16) + m;
  x = (x & mask) + ((x >> 4) & mask);

  mask = (0xff << 16) + 0xff;
  x = (x & mask) + ((x >> 8) & mask);

  mask = (0xff << 8) + 0xff;
  x = (x & mask) + ((x >> 16) & mask);
  return x;
  */

  // less steps
  int m = 0x11 | (0x11 << 8);
  int mask = m | (m << 16);

  int s = x & mask;
  s += (x >> 1) & mask;
  s += (x >> 2) & mask;
  s += (x >> 3) & mask;

  s += s >> 16;

  mask = 0xf | (0xf << 8);
  s = (s & mask) + ((s >> 4) & mask);
  return ((s >> 8) + s) & 0x3f;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  int neg = ~x + 1;
  // this cover 0x80000000 xD
  int signChange = ((neg >> 31) | (x >> 31));
  return 0x1 & (~signChange);
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 0x1 << 31;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  int shift = 33 + (~n);
  int fit = (x << shift) >> shift;
  return !(x ^ fit);
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
  int sign = x >> 31;
  int signBit = sign & 0x1;
  int mask = (0x1<<n) + (~0);
  int odd = !!(x & mask);   // has 1's in the bits that gets shifted.
  //int abs = (x ^ sign) + signBit;
  return (x >> n) + (signBit & (!!n) & odd);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return (~x) + 1;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  return !!x & !(x>>31);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int res = y + (~x+1);
  int cmp = !(res>>31);
  int signX = (x >> 31) & 0x1;
  int signY = (y >> 31) & 0x1;
  return (cmp & !(signY & !signX)) | (signX & !signY);
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
  int b16, b8, b4, b2, b1, nozero;
  nozero = !!(x >> 16);
  b16 = nozero << 4;
  x = x >> b16;

  nozero = !!(x >> 8);
  b8 = nozero << 3;
  x = x >> b8;

  nozero = !!(x >> 4);
  b4 = nozero << 2;
  x = x >> b4;

  nozero = !!(x >> 2);
  b2 = nozero << 1;
  x = x >> b2;

  nozero = !!(x >> 1);
  b1 = nozero;
  x = x >> b1;

  return b1 + b2 + b4 + b8 + b16;
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
  // greater than 0xff << 23 , NaN
  int res = ((uf<<1)>>1) - (0xff << 23);
  int sign = (res >> 31) & 0x1;
  if (res && !sign)
    return uf;
  return uf ^ (0x1 << 31);
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  unsigned sign, base, exp, res, shift, lastBit, frac, last8;
  sign = (x >> 31) & 0x1;
  base = (sign << 31);
  if (x == 0x80000000)
    return 0xcf000000;
  if (sign)
    x = -x;
  if (x == 0)
    return 0;
  if (x == 1)
    return base + 0x3f800000;
  exp = 157;
  res = 0;
  shift = 0;
  x <<= 1;
  while (!(x & 0x80000000)) {
    x <<= 1;
    shift += 1;
    }
  res = x;
  exp = exp - shift;
  lastBit = (res >> 8) & 0x1;
  frac = (res << 1) >> 9;
  last8 = res & 0xff;
  if (!lastBit) {// last bit is 0
    if (last8 == 0x80 || !(res & 0x80)) // round: 1/2 or < 1/2 then down
      res = frac;
    else
      res = frac + 1;
  }
  else {
    if (last8 == 0x80 || (res & 0x80)) {// round: 1/2 or > 1/2 then up
      res = frac + 1;
      if (res & 0x800000) {
        exp += 1;
        res = 0;
      }
    }
    else
      res = frac;
  }
  return base + (exp << 23) + res;
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  unsigned exp, mask, frac;
  exp = (uf >> 23) & 0xff;
  frac = uf & 0x7fffff;
  mask = 0x1 << 31;

  if (exp == 0xff) // infinity or NaN
    return uf;
  if (exp == 0) {
    frac <<= 1;
    if (frac & 0x800000) {
      exp += 1;
      frac = frac & 0x7fffff;
    }
  }
  else
    exp++;

  return (uf & mask) | (exp << 23) | frac;
}
