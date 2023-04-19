#include "../stdex/include/span.hpp"
#include "../stdex/include/core.h"

        struct strawman
        {
            int x;
            int y;
            bool z;
            int w;
        };

        struct naked_span
        {
            char* p;
            std::size_t n;
        };

        struct strawman_span
        {
            strawman* p;
            std::size_t n;
        };
int main(void)
{
    {


        STATIC_ASSERT(sizeof(stdex::span<char, 0>) <= sizeof(char*), _);
        STATIC_ASSERT(sizeof(stdex::span<const char, 0>) <= sizeof(const char*), _);
        STATIC_ASSERT(sizeof(stdex::span<strawman, 0>) <= sizeof(strawman*), _);
        STATIC_ASSERT(sizeof(stdex::span<strawman, 1>) <= sizeof(strawman*), _);
        STATIC_ASSERT(sizeof(stdex::span<char>) <= sizeof(naked_span), _);
        STATIC_ASSERT(sizeof(stdex::span<strawman>) <= sizeof(strawman_span), _);
    }

    return 0;
}