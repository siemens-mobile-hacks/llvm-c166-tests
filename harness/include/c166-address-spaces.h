#ifndef C166_ADDRESS_SPACES_H
#define C166_ADDRESS_SPACES_H

/*
 * Shared TASKING/Clang ISS sources use the TASKING qualifier positions.  Keep
 * the spelling adapter in the test harness, not in Clang's predefined macro
 * set: LLVM-facing translation units exercise the public c166_* attributes,
 * while TASKING continues to parse its native keywords.
 */
#ifdef __clang__
#define _far __attribute__((c166_far))
#define _near __attribute__((c166_near))
#define _xnear __attribute__((c166_xnear))
#define _huge __attribute__((c166_huge))
#define _shuge __attribute__((c166_shuge))
#endif

#endif
