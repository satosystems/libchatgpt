#include <sstream>
#include <string>
#include <vector>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "chatgpt.h"

using json = nlohmann::json;

class cb_bucket {
public:
    explicit cb_bucket(chatgpt_callback cb, void *param) : cb(cb), param(param) {};
    chatgpt_callback cb;
    void *param;
    std::string id;
};

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *clientp) {
    auto bucket = static_cast<cb_bucket *>(clientp);
    const size_t data_length = size * nmemb;
    std::string s;
    s.append(ptr, data_length);

    // Chunks may be sent consecutively, so they are split and processed
    std::vector<std::string> lines;
    std::istringstream iss(s);

    while (std::getline(iss, s)) {
        lines.push_back(s);
    }

    for (const std::string& line : lines) {
        if (line.starts_with("data: {")) {
            json j = json::parse(line.substr(5)); // 5 means sizeof("data: ") - 1
            if (bucket->id.empty()) {
                bucket->id = j["id"];
                bucket->cb(chatgpt_callback_kind_id, bucket->id.c_str(), bucket->param);
            }
            json choice = j["choices"][0];
            if (!choice.is_null()) {
                json content = choice["delta"]["content"];
                if (!content.is_null()) {
                    std::string content_s = content.get<std::string>();
                    bucket->cb(chatgpt_callback_kind_content, content_s.c_str(), bucket->param);
                }
            }
        } else if (line.starts_with("data: [DONE]")) {
            bucket->cb(chatgpt_callback_kind_stop, nullptr, bucket->param);
        }
    }

    return data_length;
}

void chatgpt_completions(const char *api_key, int length, chatgpt_message messages[], chatgpt_callback cb, void *param) {
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();

    if (curl == nullptr) {
        cb(chatgpt_callback_kind_error, "curl_easy_init() failed.", param);
        return;
    }

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    cb_bucket bucket(cb, param);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bucket);

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    std::string authorization = "Authorization: Bearer ";
    authorization.append(api_key);
    headers = curl_slist_append(headers, authorization.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    json j;
    j["model"] = "gpt-3.5-turbo";
    j["stream"] = true;
    for (int i = 0; i < length; i++) {
        j["messages"][i]["role"] = messages[i].role;
        j["messages"][i]["content"] = messages[i].content;
    }
    const std::string js = j.dump();

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, js.c_str());

    const CURLcode rc = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    if (rc != CURLE_OK) {
        cb(chatgpt_callback_kind_error, "curl_easy_perform() failed.", param);
    }
}
