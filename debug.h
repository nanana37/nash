#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) \
    do { \
        fprintf(stderr, \
        "[%s] %s:%u # " fmt "\n", \
        __DATE__, __FILE__, __LINE__, \
        ##__VA_ARGS__); \
    } while(0)
#else
// Do nothing when DEBUG is not defined
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif /* DEBUG */

#endif /* _DEBUG_H_ */