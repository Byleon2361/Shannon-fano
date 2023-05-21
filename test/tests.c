#include "ctest.h"

#include <libShannon/compress.h>
#include <libShannon/decompress.h>
CTEST(SFCOMPRESS, SHANNONFANO_FUNCTION)
{
    Value arr[4];
    Value value1 = {9, 0, "", 'u'};
    Value value2 = {4, 3, "", 'b'};
    Value value3 = {3, 6, "", 'd'};
    Value value4 = {1, 7, "", 'm'};
    arr[0] = value1;
    arr[1] = value2;
    arr[2] = value3;
    arr[3] = value4;
    ShannonFano(&arr[3], &arr[0]);
    ASSERT_STR("0", arr[0].codeString);
    ASSERT_STR("10", arr[1].codeString);
    ASSERT_STR("110", arr[2].codeString);
    ASSERT_STR("111", arr[3].codeString);
}

CTEST(SFCOMPRESS, MID_FUNCTION)
{
    Value arr[4];
    Value value1 = {9, 0, "0", 'u'};
    Value value2 = {4, 3, "10", 'b'};
    Value value3 = {3, 6, "110", 'd'};
    Value value4 = {1, 7, "111", 'm'};
    arr[0] = value1;
    arr[1] = value2;
    arr[2] = value3;
    arr[3] = value4;
    Value *res = midFunc(&arr[3], &arr[0]);
    ASSERT_EQUAL('b', res->symbol);
}
CTEST(SFCOMPRESS, NEXTMID_FUNCTION)
{
    Value arr[4];
    Value value1 = {9, 0, "0", 'u'};
    Value value2 = {4, 3, "10", 'b'};
    Value value3 = {3, 6, "110", 'd'};
    Value value4 = {1, 7, "111", 'm'};
    arr[0] = value1;
    arr[1] = value2;
    arr[2] = value3;
    arr[3] = value4;
    Value *res = nextMidFunc(&arr[3], &arr[0], midFunc(&arr[3], &arr[0]));
    ASSERT_EQUAL('u', res->symbol);
}
// oddEvenSort(Value *arr, int size);
// int toByte(char *string);
CTEST(SORT, ODD_EVEN_SORT)
{
    Value arr[4];
    Value value1 = {9, 0, "0", 'u'};
    Value value2 = {4, 3, "10", 'b'};
    Value value3 = {3, 6, "110", 'd'};
    Value value4 = {1, 7, "111", 'm'};
    arr[0] = value4;
    arr[1] = value3;
    arr[2] = value2;
    arr[3] = value1;
    oddEvenSort(arr, 4);
    ASSERT_EQUAL('u', arr[0].symbol);
    ASSERT_EQUAL('b', arr[1].symbol);
    ASSERT_EQUAL('d', arr[2].symbol);
    ASSERT_EQUAL('m', arr[3].symbol);
}
CTEST(COUNT, TOBYTE)
{
    ASSERT_EQUAL(255, toByte("11111111"));
    ASSERT_EQUAL(43775, toByte("1010101011111111"));
    ASSERT_EQUAL(-1, toByte("2"));
}