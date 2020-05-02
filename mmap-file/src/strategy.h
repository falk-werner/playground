#ifndef STRATEGY_H
#define STRATEGY_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef char * 
strategy_fn(
    char const * filename);

extern strategy_fn *
strategy_get(
    char const * name);

extern char *
strategy_mmap(
    char const * filename);

extern char *
strategy_readall(
    char const * filename);

extern char *
strategy_buffered(
    char const * filename);

#ifdef __cplusplus
}
#endif

#endif
