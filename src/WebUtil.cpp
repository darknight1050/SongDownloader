#include "WebUtil.hpp"

#include "CustomLogger.hpp"

#include "curl.h"
#include "easy.h"

#define TIMEOUT 10

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
    getLogger().info("JSON: %s", data.c_str());
    rapidjson::Document document;
    document.Parse(data);
    if(document.HasParseError() || !document.IsObject())
        return std::nullopt;
    return document;
}

long WebUtil::Get(std::string_view url, std::string& val) {
    // Init curl
    auto* curl = curl_easy_init();
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: */*");
    // Set headers
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 

    curl_easy_setopt(curl, CURLOPT_URL, url.data());

    // Don't wait forever, time out after TIMEOUT seconds.
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT);

    // Follow HTTP redirects if necessary.
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);

    long httpCode(0);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void*>(&val));
    static const char* USER_AGENT = (std::string("QuestSongDownloader/") + VERSION + " (+https://github.com/darknight1050/SongDownloader)").c_str();
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

long WebUtil::GetToFile(std::string_view url, const std::string& file) {
    auto fileStream = fopen(file.c_str(), "w");
    // Init curl
    auto* curl = curl_easy_init();
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: */*");
    // Set headers
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 

    curl_easy_setopt(curl, CURLOPT_URL, url.data());

    // Don't wait forever, time out after TIMEOUT seconds.
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT);

    // Follow HTTP redirects if necessary.
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);

    long httpCode(0);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void*>(fileStream));
    static const char* USER_AGENT = (std::string("QuestSongDownloader/") + VERSION + " (+https://github.com/darknight1050/SongDownloader)").c_str();
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
    fclose(fileStream);
    return httpCode;
}