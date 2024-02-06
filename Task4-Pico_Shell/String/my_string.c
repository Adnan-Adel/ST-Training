#include "my_string.h"

uint32_t str_length(const char* str)
{
    if(NULL == str)
    {
        return 0;
    }
    else
    {
        uint32_t index_i= 0;
        while(str[index_i] != '\0')
        {
            index_i++;
        }
        return index_i;
    }
}

char compare_str(const char* str1, const char* str2)
{
    if((NULL == str1) || (NULL == str2))
    {
        return 0;
    }
    else
    {
        uint32_t str1_length= str_length(str1);
        uint32_t str2_length= str_length(str2);
        if(str1_length != str2_length)
        {
            return 0;   // Not the same.
        }
        else
        {
            uint32_t index_i= 0;
            while(str1[index_i] != '\0')
            {
                if(str1[index_i] != str2[index_i])
                {
                    return 0;
                }
                index_i++;
            }
            return 1;   // the two strings are identical.
        }
    }
}

char* substring(const char* src_str, uint32_t left, uint32_t right)
{
    uint32_t dst_str_length= right - left + 1;
    uint32_t index_i= left;

    char* dst_str_ptr= (char*)malloc((dst_str_length+1) * sizeof(char));
    char* dst_str_start = dst_str_ptr; // Save the start pointer

    while((index_i <= right) && (src_str[index_i] != '\0'))
    {
        *dst_str_ptr= src_str[index_i];
        dst_str_ptr++;
        index_i++;
    }
    *dst_str_ptr= '\0';

    return dst_str_start; // Return the start pointer
}

uint8_t is_str_in_arr(const char* str, const char* arr[], uint32_t length)
{
    if((NULL == str) || (NULL == arr))
    {
        return 0;
    }
    else
    {
        uint32_t index_i= 0;
        uint8_t flag= 0;
        for(index_i= 0; index_i < length; index_i++)
        {
            flag= compare_str(str, arr[index_i]);
            if(flag == 1)
            {
                return 1;
            }
        }
        return 0;
    }
}

uint8_t is_char_in_arr(const char ch, const char arr[], uint32_t length)
{
    if(NULL == arr)
    {
        return 0;
    }
    else
    {
        uint32_t index_i= 0;
        for(index_i= 0; index_i < length; index_i++)
        {
            if(ch == arr[index_i])
            {
                return 1;
            }
        }
        return 0;
    }
}