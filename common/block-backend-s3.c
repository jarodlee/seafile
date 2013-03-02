#include "common.h"
#include "log.h"

#include <time.h>
#include <curl/curl.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

#include "utils.h"

#include "block-backend.h"
#include "obj-store.h"

#ifdef S3_BACKEND

#define AMAZON_S3_URL_SUFFIX    ".s3.amazonaws.com"

#define GET             "GET"

struct _BHandle {
    char    *block_id[41];
    int      rw_type;
};

typedef struct {
    const char *bucket_name;
    const char *key_id;
    const char *key;
} S3Priv;

BHandle *
block_backend_s3_open_block (BlockBackend *bend,
                               const char *block_id,
                               int rw_type)
{
    BHandle *handle;

    handle = g_new0(BHandle, 1);
    memcpy(handle->block_id, block_id, 41);
    handle->rw_type = rw_type;

    return handle;
}

static char *get_httpdate()
{
    static char atime[256];
    time_t now = time(NULL);
    struct tm *gtime;

    gtime = gmtime(&now);
    memset(atime, 0, sizeof(atime));
    strftime(atime, sizeof(atime), "%a, %d %b %Y %H:%M:%S +0000", gtime);
    return atime;
}

static char *
get_signautre(char *resource, int resource_size, char **date, const char method,
              const char *bucket, const char *block_id)
{
    char req[2048];
    char acl[32];
    char rrs[64];

    date = get_httpdate();
    memset(resource, 0, resource_size);

    snprintf(resource, resource_size, "%s/%s", bucket, block_id);
    acl[0] = 0;
    rrs[0] = 0;

    snprintf(req, sizeof(req), "%s\n\n%s\n%s\n%s%s/%s",
             method, "", *date, acl, rrs, resource);

    /* TODO: Handle virtual host */

    return NULL;
}

int
block_backend_s3_read_block (BlockBackend *bend, BHandle *handle,
                               void *buf, int len)
{
    S3Priv *priv = bend->be_priv;
    char resource[1024];
    char *signature, *date = NULL;

    if (handle->rw_type == BLOCK_WRITE) {
        g_warning("[S3 backend] Couldn't read a write-only block\n");
        return -1;
    }

    signature = get_signautre(resource, sizeof(resource), &date, GET,
                              priv->bucket_name, handle->block_id);

    return 0;
}

int
block_backend_s3_write_block (BlockBackend *bend,
                                BHandle *handle,
                                const void *buf, int len)
{
    return 0;
}

int
block_backend_s3_commit_block (BlockBackend *bend, BHandle *handle)
{
    return 0;
}

int
block_backend_s3_close_block (BlockBackend *bend, BHandle *handle)
{
    /* Nothing to do */
    return 0;
}

gboolean
block_backend_s3_block_exists (BlockBackend *bend, const char *block_sha1)
{
    return FALSE;
}

int
block_backend_s3_remove_block (BlockBackend *bend,
                                 const char *block_id)
{
    return 0;
}

BMetadata *
block_backend_s3_stat_block (BlockBackend *bend,
                               const char *block_id)
{
    return NULL;
}

BMetadata *
block_backend_s3_stat_block_by_handle (BlockBackend *bend,
                                         BHandle *handle)
{
    return NULL;
}

void
block_backend_s3_block_handle_free (BlockBackend *bend, BHandle *handle)
{
    g_free(handle);
}

int
block_backend_s3_foreach_block (BlockBackend *bend,
                                  SeafBlockFunc process,
                                  void *user_data)
{
    return 0;
}

BlockBackend *block_backend_s3_new (const char *bucket_name, const char *key_id,
                                    const char *key)
{
    BlockBackend *bend;
    S3Priv *priv;

    bend = g_new0(BlockBackend, 1);
    priv = g_new0(S3Priv, 1);
    bend->be_priv = priv;

    priv->bucket_name = g_strdup(bucket_name);
    priv->key_id = g_strdup(key_id);
    priv->key = g_strdup(key);

    bend->open_block = block_backend_s3_open_block;
    bend->read_block = block_backend_s3_read_block;
    bend->write_block = block_backend_s3_write_block;
    bend->commit_block = block_backend_s3_commit_block;
    bend->close_block = block_backend_s3_close_block;
    bend->exists = block_backend_s3_block_exists;
    bend->remove_block = block_backend_s3_remove_block;
    bend->stat_block = block_backend_s3_stat_block;
    bend->stat_block_by_handle = block_backend_s3_stat_block_by_handle;
    bend->block_handle_free = block_backend_s3_block_handle_free;
    bend->foreach_block = block_backend_s3_foreach_block;

    curl_global_init(CURL_GLOBAL_ALL);

    return bend;
err:
    g_free(priv);
    g_free(bend);

    return NULL;
}

#else

BlockBackend *block_backend_s3_new (const char *bucket_name, const char *key_id,
                                    const char *key);
{
    seaf_warning ("Amazon S3 backend is not enabled\n");
    return NULL;
}

#endif /* S3_BACKEND */
