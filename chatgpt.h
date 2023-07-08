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

#define chatgpt_role_system "system"
#define chatgpt_role_user "user"

typedef struct {
    const char *role;
    const char *content;
} chatgpt_message;

typedef void (* chatgpt_callback)(const chatgpt_callback_kind kind, const char *s, void *param);

void chatgpt_completions(const char *api_key, int length, chatgpt_message messages[], chatgpt_callback cb, void *param = nullptr);

#ifdef __cplusplus
}
#endif
