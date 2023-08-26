#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <curl/curl.h>

#define ARRAY_CARDINALITY(x)    (sizeof (x) / sizeof (*x))

static int const PROG_BAR_LENGHT = 30;
static double const PREDICT_WEIGHT = 0.4;

struct status_info {
    size_t total_bytes;
    size_t total_expected;
    double exp_bytes_per_url;
    size_t current_bytes;
    size_t urls_so_far;
    size_t total_urls;
};

static double predict_next (double last_prediction, double actual)
{
    return ((last_prediction * (1 - PREDICT_WEIGHT)) + (actual * PREDICT_WEIGHT)); 
}

static void update_bar (size_t percent_done, struct status_info *sinfo) 
{
	size_t const num_chars = percent_done * PROG_BAR_LENGHT / 100;
	printf ("\r[");
	for (size_t i = 0; i < num_chars; ++i) {
		putchar ('#');
	}
	
	for (size_t i = 0; i < PROG_BAR_LENGHT - num_chars; ++i) {
		putchar (' ');
	}
	printf ("] %ld%% Done (%zu MB)", percent_done, sinfo->total_bytes / 10000000);
	fflush (stdout);
} 

static size_t write_callback (char *buffer, size_t size, size_t nmemb, void *st_info) 
{
    (void) buffer;
    struct status_info *const status = st_info;
    size_t const bytes = size * nmemb;
    status->total_bytes += bytes;
    status->current_bytes += bytes;

    size_t const urls_left = (status->total_urls - status->urls_so_far);
    double estimate_current = status->exp_bytes_per_url;

    if (status->current_bytes > status->exp_bytes_per_url) {
        estimate_current = status->current_bytes * 4 / 3;
    }

    double const guess_next_prediction = predict_next (status->exp_bytes_per_url, estimate_current);
    size_t const estimated_total = status->total_bytes + (estimate_current - status->current_bytes) + ((urls_left - 1) * guess_next_prediction);
    size_t const percent_done = status->total_bytes * 100 / estimated_total;
    
    update_bar (percent_done, status);
    return bytes;
}
 
static bool download_url (const char *url, struct status_info *sinfo)
{
    CURL *curl = curl_easy_init();
    
    if (!curl) {
        return false;
    }

    curl_easy_setopt (curl, CURLOPT_URL, url);
    curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, sinfo);
    curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 2L);

    CURLcode ret = curl_easy_perform (curl);
    curl_easy_cleanup (curl);
    curl_global_cleanup();
    return ret == CURLE_OK;
}

int main (void) 
{
    static const char *const urls[] = {
        "https://people.cs.clemson.edu/~jsorber/img/garden.jpg",
        "https://cdimage.debian.org/cdimage/archive/10.9.0/amd64/iso-dvd/debian-10.9.0-amd64-DVD-1.iso",
        "https://images.pexels.com/photos/835887/pexels-photo-835887.jpeg",
        "https://unsplash.com/photos/4XdvnmtijZI/download?force=true",
        "https://unsplash.com/photos/MkImkLEuqcY/download?force=true"
    };
    struct status_info sinfo = {.total_urls = ARRAY_CARDINALITY (urls), .exp_bytes_per_url = 100000000.0};
    update_bar (0, &sinfo);
    
    for (size_t i = 0; i < ARRAY_CARDINALITY (urls); ++i) {
        sinfo.current_bytes = 0;
        download_url (urls[i], &sinfo);
        sinfo.urls_so_far++;
        sinfo.exp_bytes_per_url = predict_next (sinfo.exp_bytes_per_url, sinfo.current_bytes);
	}
	printf ("\n");
	return EXIT_FAILURE;
}
