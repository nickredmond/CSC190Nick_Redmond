#ifdef ASSERT_ON
#define ASSERT(expr, ...)  do { const char* msg = #expr##" "##__VA_ARGS__; if (!(expr)) {LOG(Severe, msg ); END_LOG DebugBreak(); exit(1);}}while(0,0);
#else
#define ASSERT(expr, ...) {expr;};
#endif