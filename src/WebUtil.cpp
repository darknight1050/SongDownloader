#include "WebUtil.hpp"

#include "CustomLogger.hpp"

#include "curl.h"
#include "easy.h"

#define TIMEOUT 10
#define USER_AGENT (std::string("QuestSongDownloader/") + VERSION + " (+https://github.com/darknight1050/SongDownloader)").c_str()
    
std::size_t CurlWrite_CallbackFunc_StdString(void *contents, std::size_t size, std::size_t nmemb, std::string *s)
{
    std::size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch(std::bad_alloc &e) {
        //handle memory problem
        getLogger().critical("Failed to allocate string of size: %u", newLength);
        return 0;
    }
    return newLength;
}

std::optional<rapidjson::Document> WebUtil::GetJSON(std::string_view url) {
    std::string data;
    Get(url, data);
    rapidjson::Document document;
    document.Parse(data);
    if(document.HasParseError() || !document.IsObject())
        return std::nullopt;
    return document;
}

long WebUtil::Get(std::string_view url, std::string& val) {
    return Get(url, TIMEOUT, val);
}

long WebUtil::Get(std::string_view url, long timeout, std::string& val) {
    // Init curl
    auto* curl = curl_easy_init();
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: */*");
    // Set headers
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 

    curl_easy_setopt(curl, CURLOPT_URL, url.data());

    // Don't wait forever, time out after TIMEOUT seconds.
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

    // Follow HTTP redirects if necessary.
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);

    long httpCode(0);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void*>(&val));
    curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
 
    auto res = curl_easy_perform(curl);
    /* Check for errors */ 
    if (res != CURLE_OK) {
        getLogger().critical("curl_easy_perform() failed: %u: %s", res, curl_easy_strerror(res));
    }
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    return httpCode;
}

struct ProgressUpdateWrapper {
    std::function<void(float)> progressUpdate;
};

void WebUtil::GetAsync(std::string url, std::function<void(long, std::string)> finished, std::function<void(float)> progressUpdate) {
    GetAsync(url, TIMEOUT, finished, progressUpdate);
}

void WebUtil::GetAsync(std::string url, long timeout, std::function<void(long, std::string)> finished, std::function<void(float)> progressUpdate) {
    std::thread t (
        [url, timeout, progressUpdate, finished] {
            std::string val;
            // Init curl
            auto* curl = curl_easy_init();
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "Accept: */*");
            // Set headers
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 

            curl_easy_setopt(curl, CURLOPT_URL, url.data());

            // Don't wait forever, time out after TIMEOUT seconds.
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

            // Follow HTTP redirects if necessary.
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);

            ProgressUpdateWrapper* wrapper = new ProgressUpdateWrapper { progressUpdate };
            if(progressUpdate) {
                // Internal CURL progressmeter must be disabled if we provide our own callback
                curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
                curl_easy_setopt(curl, CURLOPT_XFERINFODATA, wrapper);
                // Install the callback function
                curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, 
                    +[] (void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
                        float percentage = (float)dlnow / (float)dltotal * 100.0f;
                        if(isnan(percentage))
                            percentage = 0.0f;
                        reinterpret_cast<ProgressUpdateWrapper*>(clientp)->progressUpdate(percentage);
                        return 0;
                    }
                );
            }

            long httpCode(0);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &val);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        
            auto res = curl_easy_perform(curl);
            /* Check for errors */ 
            if (res != CURLE_OK) {
                getLogger().critical("curl_easy_perform() failed: %u: %s", res, curl_easy_strerror(res));
            }
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
            curl_easy_cleanup(curl);
            delete wrapper;
            finished(httpCode, val);
        }
    );
    t.detach();
}

void WebUtil::GetJSONAsync(std::string url, std::function<void(long, bool, rapidjson::Document&)> finished) {
    GetAsync(url,
        [finished] (long httpCode, std::string data) { 
            rapidjson::Document document;
            document.Parse(data);
            finished(httpCode, document.HasParseError() || !document.IsObject(), document);
        }
    );
}
