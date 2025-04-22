#include "bitmap.h"            //函数定义
#include "global.h"            //global.h 不清楚
#include "string.h"            //memset函数要用
#include "interrupt.h"         
#include "print.h"             
#include "debug.h"

#define BITMAP_MASK 1


void bitmap_init(struct bitmap* btmp)
{
    if (btmp == NULL || btmp->bits == NULL) {
        put_str("[-] bitmap_init: NULL pointer exception\n");
        return;
    }
    memset(btmp->bits, 0, btmp->btmp_bytes_len);
    // put_str("[+] bitmap_init: Bitmap initialized successfully\n");
    return;
}

bool bitmap_scan_test(struct bitmap* btmp, uint32_t bit_idx)  //一个8位的数 bit_idx/8 找数组下标 %得索引下的具体位置
{
    if (btmp == NULL || btmp->bits == NULL) {
        put_str("[-] bitmap_scan_test: NULL pointer exception\n");
        return false;
    }

    uint32_t byte_idx = bit_idx / 8;
    uint32_t byte_pos = bit_idx % 8;

    if (byte_idx >= btmp->btmp_bytes_len) {
        put_str("[-] bitmap_scan_test: Bitmap index out of bounds\n");
        return false;
    }

    bool result = (btmp->bits[byte_idx] & (BITMAP_MASK << byte_pos));

}

int bitmap_scan(struct bitmap* btmp, uint32_t cnt)
{
    if (btmp == NULL || btmp->bits == NULL) {
        printf("\n[-] "__FILE__" [Line: %d][func:%s]: btmp is invalid!\n", __LINE__, __func__);
        return -1;
    }
    uint32_t first_find_idx = 0;
    while (first_find_idx < btmp->btmp_bytes_len && btmp->bits[first_find_idx] == 0xff)
        ++first_find_idx;

    if (first_find_idx == btmp->btmp_bytes_len) {
        put_str("[-] bitmap_scan: No free bits found\n");
        return -1;
    }

    uint32_t find_pos = 0;
    while ((btmp->bits[first_find_idx] & (BITMAP_MASK << find_pos)))
        ++find_pos;

    if (cnt == 1) {
        int result = find_pos + 8 * first_find_idx;
        // put_str("[+] bitmap_scan: Found 1 free bit at position = ");
        // put_int(result);
        // put_char('\n');
        return result;
    }

    uint32_t ret_pos = find_pos + 8 * first_find_idx + 1;
    uint32_t tempcnt = 1;
    uint32_t endpos = btmp->btmp_bytes_len * 8;

    while (ret_pos < endpos) {
        if (!bitmap_scan_test(btmp, ret_pos)) ++tempcnt;
        else tempcnt = 0;

        if (tempcnt == cnt) {
            int result = ret_pos - tempcnt + 1;
            // put_str("[+] bitmap_scan: Found continuous free bits, start position = ");
            // put_int(result);
            // put_char('\n');
            return result;
        }
        ++ret_pos;
    }

    put_str("[-] bitmap_scan: No continuous free bits found\n");
    return -1;	
}

void bitmap_set(struct bitmap* btmp, uint32_t bit_idx, int8_t value)
{
    ASSERT(value == 0 || value == 1);

    if (btmp == NULL || btmp->bits == NULL) {
        put_str("[-] bitmap_set: NULL pointer exception\n");
        return;
    }

    uint32_t byte_idx = bit_idx / 8;
    uint32_t byte_pos = bit_idx % 8;

    if (byte_idx >= btmp->btmp_bytes_len) {
        put_str("[-] bitmap_set: Bitmap index out of bounds\n");
        return;
    }

    if (value)
        btmp->bits[byte_idx] |= (BITMAP_MASK << byte_pos);
    else
        btmp->bits[byte_idx] &= ~(BITMAP_MASK << byte_pos);

    // put_str("[+] bitmap_set: Set bit ");
    // put_int(bit_idx);
    // put_str(" to ");
    // put_int(value);
    // put_char('\n');
    return;
}
