#include "quickjs.h"
#include <stdio.h>
#include <string.h>
#include "./fib.c"
#include "quickjs-libc.h"

static JSValue js_print(JSContext *ctx, JSValueConst this_val,
                        int argc, JSValueConst *argv)
{
    int i;
    const char *str;
    for (i = 0; i < argc; i++)
    {
        if (i != 0)
            puts(" ");
        str = JS_ToCString(ctx, argv[i]);
        puts(str);
        JS_FreeCString(ctx, str);
    }
}

static JSValue js_fib(JSContext *ctx, JSValueConst this_val,
                      int argc, JSValueConst *argv)
{
    int n, res;
    if (JS_ToInt32(ctx, &n, argv[0]))
        return JS_EXCEPTION;
    res = fib(n);
    return JS_NewInt32(ctx, res);
}

void eval(const char *str)
{
    char *s;
    JSRuntime *runtime = JS_NewRuntime();
    JSContext *ctx = JS_NewContext(runtime);

    JSValue global_obj = JS_GetGlobalObject(ctx);
    JS_SetPropertyStr(ctx, global_obj, "fib",
                      JS_NewCFunction(ctx, js_fib, "fib", 1));
    JS_SetPropertyStr(ctx, global_obj, "log",
                      JS_NewCFunction(ctx, js_print, "log", 1));
    JSValue result =
        JS_Eval(ctx, str, strlen(str), "<evalScript>", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(result))
    {
        JSValue realException = JS_GetException(ctx);
        s = JS_ToCString(ctx, realException);
        printf("result:\n%s\n", s);
        return;
    }
    JSValue json = JS_JSONStringify(ctx, result, JS_UNDEFINED, JS_UNDEFINED);
    JS_FreeValue(ctx, result);
    s = JS_ToCString(ctx, json);
    printf("result:\n%s\n", s);
    int n = JS_ExecutePendingJob(runtime, &ctx);
    printf("n: %d\n", n);
}

int main()
{
    printf("hello\n");
    char *code = "async function f(){return await 1;};  f();log('---'); log(fib(10));log('---');";
    printf("code:%s\n", code);
    eval(code);
    return 0;
}
