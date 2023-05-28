#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    chatgpt_callback_kind_error,
    chatgpt_callback_kind_id,
    chatgpt_callback_kind_content,
    chatgpt_callback_kind_stop,
} chatgpt_callback_kind;

typedef void (* chatgpt_callback)(const chatgpt_callback_kind kind, const char *s);

void chatgpt_completions(const char *api_key, chatgpt_callback cb);

#ifdef __cplusplus
}
#endif
