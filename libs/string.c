#include "string.h"

inline void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
    while (len-- > 0)
        *dest++ = *src++;
}

inline void memset(void *dest, uint8_t val, uint32_t len)
{
    uint8_t *dst = (uint8_t *)dest;

    while (len-- > 0)
        *dst++ = val;
}

inline void bzero(void *dest, uint32_t len)
{
    memset(dest, 0, len);
}

//长度存在问题
inline int strcmp(const char *str1, const char *str2)
{
    while ((*str1 != 0) && (*str2 != 0))
    {
        if (*str1++ == *str2++)
            continue;
        else
            return FALSE;
    }
    if (*str1 == *str2)
        return TRUE;
    else
        return FALSE;
}

//不检查dest的容量限制
inline char *strcpy(char *dest, const char *src)
{
    char *rec = dest;

    while (*src)
        *dest++ = *src++;
    *dest = '\0';

    return rec;
}

inline char *strcat(char *dest, const char *src)
{
    char *end = dest;

    while (*end++);

    while (*src)
        *end++ = *src++;
    *end = '\0';

    return dest;
}

inline int strlen(const char *src)
{
    int i = 0;

    while (*src++)
        ++i;

    return i;
}