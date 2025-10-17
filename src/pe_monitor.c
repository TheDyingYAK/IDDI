#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <time.h>
#include <openssl/md5.h>
#include <errno.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))
#define LOG_FILE "/var/log/pe_monitor.log"

typedef struct {
    int wd;
    char path[PATH_MAX];
} WatchDescriptor;

WatchDescriptor *watches = NULL;
int num_watches = 0;
int max_watches = 0;

void add_watch(int fd, const char *path);
void recursive_add_watches(int fd, const char *path);
int is_pe_file(const char *filepath);
void compute_md5(const char *filepath, char *md5_str);
void log_pe_file(const char *filepath);

int main() {
    int fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        exit(1);
    }

    // Initial recursive watches
    recursive_add_watches(fd, "/home");
    recursive_add_watches(fd, "/root");

    char buffer[BUF_LEN];
    FILE *log_fp = fopen(LOG_FILE, "a");
    if (!log_fp) {
        perror("fopen log file");
        exit(1);
    }
    fclose(log_fp);  // Just to create/ensure it exists with permissions

    while (1) {
        int length = read(fd, buffer, BUF_LEN);
        if (length < 0) {
            perror("read");
            exit(1);
        }

        int i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len) {
                // Find the full path
                char fullpath[PATH_MAX] = {0};
                for (int j = 0; j < num_watches; j++) {
                    if (watches[j].wd == event->wd) {
                        snprintf(fullpath, PATH_MAX, "%s/%s", watches[j].path, event->name);
                        break;
                    }
                }

                if (event->mask & IN_CREATE) {
                    struct stat st;
                    if (stat(fullpath, &st) == 0) {
                        if (S_ISDIR(st.st_mode)) {
                            // New directory, add recursive watches
                            recursive_add_watches(fd, fullpath);
                        } else {
                            // New file, check if PE
                            if (is_pe_file(fullpath)) {
                                log_pe_file(fullpath);
                            }
                        }
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    close(fd);
    free(watches);
    return 0;
}

void add_watch(int fd, const char *path) {
    int wd = inotify_add_watch(fd, path, IN_CREATE);
    if (wd < 0) {
        if (errno == EACCES) {
            fprintf(stderr, "Permission denied for %s\n", path);
        } else {
            perror("inotify_add_watch");
        }
        return;
    }

    if (num_watches >= max_watches) {
        max_watches = max_watches ? max_watches * 2 : 1024;
        watches = realloc(watches, max_watches * sizeof(WatchDescriptor));
    }

    watches[num_watches].wd = wd;
    strncpy(watches[num_watches].path, path, PATH_MAX);
    num_watches++;
}

void recursive_add_watches(int fd, const char *path) {
    add_watch(fd, path);

    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char subpath[PATH_MAX];
            snprintf(subpath, PATH_MAX, "%s/%s", path, entry->d_name);
            recursive_add_watches(fd, subpath);
        }
    }
    closedir(dir);
}

int is_pe_file(const char *filepath) {
    FILE *fp = fopen(filepath, "rb");
    if (!fp) return 0;

    char magic[2];
    if (fread(magic, 1, 2, fp) != 2) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    return (magic[0] == 'M' && magic[1] == 'Z');
}

void compute_md5(const char *filepath, char *md5_str) {
    FILE *fp = fopen(filepath, "rb");
    if (!fp) {
        strcpy(md5_str, "ERROR");
        return;
    }

    MD5_CTX ctx;
    MD5_Init(&ctx);

    unsigned char buffer[1024];
    size_t len;
    while ((len = fread(buffer, 1, 1024, fp)) > 0) {
        MD5_Update(&ctx, buffer, len);
    }
    fclose(fp);

    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_Final(digest, &ctx);

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(&md5_str[i*2], "%02x", digest[i]);
    }
    md5_str[MD5_DIGEST_LENGTH * 2] = '\0';
}

void log_pe_file(const char *filepath) {
    char md5[MD5_DIGEST_LENGTH * 2 + 1];
    compute_md5(filepath, md5);

    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm);

    FILE *log_fp = fopen(LOG_FILE, "a");
    if (log_fp) {
        fprintf(log_fp, "%s - File: %s - MD5: %s\n", timestamp, filepath, md5);
        fclose(log_fp);
    } else {
        perror("fopen log file");
    }
}
