/* tinfl.h
 * Exposes the function of tinfl.c this library needs.
 * This file is in the public domain; see the trailer of tinfl.c for details.
 */

#ifndef TINFL_H
#define TINFL_H

#define TINFL_DECOMPRESS_MEM_TO_MEM_FAILED ((size_t)(-1))
size_t tinfl_decompress_mem_to_mem(void *pOut_buf, size_t out_buf_len, const void *pSrc_buf, size_t src_buf_len, int flags);

#endif
