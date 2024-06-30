#pragma once
#include <cmath>

class AlgUtil
{
public:

    static int FindNumberInAreaArr(float * areaArr, float testNum, int start, int end)
    {
        if (areaArr[start] > testNum)
        {
            return start;
        }

        if (areaArr[end] <= testNum)
        {
            return end;
        }
        int mid = (int)std::floor((start + end) / 2) ;
      
        if(areaArr[mid] < testNum && testNum < areaArr[mid+1])
        {
            return mid+1;
        }

        if (mid == start)
        {
            return -1;
        }

        if (areaArr[mid] > testNum)
        {
            return FindNumberInAreaArr(areaArr, testNum, start, mid);
        } else {
            return FindNumberInAreaArr(areaArr, testNum, mid, end);
        }
    }
};