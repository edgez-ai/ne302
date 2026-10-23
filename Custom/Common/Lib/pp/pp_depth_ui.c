/**
 * @file pp_depth_ui.c
 * @brief Pass through a uint8 monocular depth map (FastDepth and similar).
 */

#include "pp.h"
#include "cJSON.h"
#include "mem.h"
#include <string.h>

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t *depth;
} pp_depth_ui_params_t;

static int32_t init(const char *json_str, void **pp_params, void *nn_inst)
{
    pp_depth_ui_params_t *pp;
    uint32_t width = 224;
    uint32_t height = 224;

    (void)nn_inst;

    if (json_str != NULL) {
        cJSON *root = cJSON_Parse(json_str);
        if (root != NULL) {
            cJSON *params = cJSON_GetObjectItemCaseSensitive(root, "postprocess_params");
            if (params == NULL) {
                params = root;
            }
            if (cJSON_IsObject(params)) {
                cJSON *w = cJSON_GetObjectItemCaseSensitive(params, "width");
                cJSON *h = cJSON_GetObjectItemCaseSensitive(params, "height");
                if (cJSON_IsNumber(w) && w->valuedouble > 0) {
                    width = (uint32_t)w->valuedouble;
                }
                if (cJSON_IsNumber(h) && h->valuedouble > 0) {
                    height = (uint32_t)h->valuedouble;
                }
            }
            cJSON_Delete(root);
        }
    }

    pp = (pp_depth_ui_params_t *)hal_mem_alloc_any(sizeof(*pp));
    if (pp == NULL) {
        return -1;
    }
    memset(pp, 0, sizeof(*pp));
    pp->width = width;
    pp->height = height;
    pp->depth = (uint8_t *)hal_mem_alloc_large((size_t)width * height);
    if (pp->depth == NULL) {
        hal_mem_free(pp);
        return -1;
    }

    *pp_params = pp;
    return 0;
}

static int32_t deinit(void *pp_params)
{
    pp_depth_ui_params_t *pp = (pp_depth_ui_params_t *)pp_params;
    if (pp == NULL) {
        return 0;
    }
    if (pp->depth != NULL) {
        hal_mem_free(pp->depth);
    }
    hal_mem_free(pp);
    return 0;
}

static int32_t run(void *pInput[], uint32_t nb_input, void *pResult, void *pp_params, void *nn_inst)
{
    pp_depth_ui_params_t *pp = (pp_depth_ui_params_t *)pp_params;
    pp_result_t *result = (pp_result_t *)pResult;
    uint32_t size;

    (void)nn_inst;

    if (result == NULL || pp == NULL || pp->depth == NULL) {
        return -1;
    }
    memset(result, 0, sizeof(*result));
    if (nb_input < 1 || pInput == NULL || pInput[0] == NULL) {
        return -1;
    }

    size = pp->width * pp->height;
    memcpy(pp->depth, pInput[0], size);

    result->type = PP_TYPE_DEPTH;
    result->is_valid = 1;
    result->depth.depth = pp->depth;
    result->depth.width = pp->width;
    result->depth.height = pp->height;
    result->depth.size = size;
    return 0;
}

static int32_t set_confidence_threshold(void *params, float threshold)
{
    (void)params;
    (void)threshold;
    return 0;
}

static int32_t get_confidence_threshold(void *params, float *threshold)
{
    (void)params;
    if (threshold) {
        *threshold = 0.0f;
    }
    return 0;
}

static int32_t set_nms_threshold(void *params, float threshold)
{
    (void)params;
    (void)threshold;
    return 0;
}

static int32_t get_nms_threshold(void *params, float *threshold)
{
    (void)params;
    if (threshold) {
        *threshold = 0.0f;
    }
    return 0;
}

static const pp_vtable_t vt = {
    .init = init,
    .run = run,
    .deinit = deinit,
    .set_confidence_threshold = set_confidence_threshold,
    .get_confidence_threshold = get_confidence_threshold,
    .set_nms_threshold = set_nms_threshold,
    .get_nms_threshold = get_nms_threshold,
};

const pp_entry_t pp_entry_depth_ui = {
    .name = "pp_depth_ui",
    .vt = &vt
};
