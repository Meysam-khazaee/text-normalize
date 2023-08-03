/*
 *  Generic text normalizer.
 */

#ifndef __TEXTNORMALIZE_H
#define __TEXTNORMALIZE_H

#define ASCII_FILE_BUFF_LENGTH 131072
#define MAX_ASCII_FILE_SIZE 20000000

#define MIN_3(x, y, z) ((x) < (y) ? ((x) < (z) ? (x) : (z)) : ((y) < (z) ? (y) : (z)))

class TextNormalize
{
private:
    unsigned char* out;
    size_t out_len;
    size_t out_pos;
    int space_written;
public:
    TextNormalize(unsigned char* out, size_t out_len);
    void normalizeReset();
    size_t normalizeBuffer(const unsigned char* buf, const size_t buf_len);
    // antimlaware: must uncomment in FPTA internal system
    //size_t normalizeMap(fmap_t* map, size_t offset);
};

#endif  \\ __TEXTNORMALIZE_H
