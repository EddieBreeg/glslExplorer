#pragma once

#define glClearErrors   while(glGetError());

#if defined(__cplusplus)
extern "C"
#endif // __cplusplus
int glLogError(const char *funcName, const char *file, int lineNumber);


#define glCheckCall(expr)           \
    glClearErrors;                  \
    expr;                           \
    glLogError(#expr, __FILE__, __LINE__);  \


