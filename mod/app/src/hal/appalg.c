#include "common.h"
#include "log.h"
#include "nnm.h"
#include "inc/appalg.h"
#include "inc/cfg.h"
#include "algtype.h"
#include "proto.h"
#include "inc/hsFace.h"
#include "vsf/frame_mgr.h"

typedef void* (*alg_create)(char *path);
typedef int (*alg_process)(void *args);
typedef int (*alg_destroy)(void *args);


typedef struct {
    int  type;
	char *path;	
    alg_create  create_func;
	alg_process process_func;
	alg_destroy destroy_func;
	UT_hash_handle hh;
} alg_workflow_t;

typedef struct {
    alg_workflow_t *pWorkflows;
    void *hface;
} app_alg_priv_t;

app_alg_priv_t appself = {0};

int app_alg_init(int type)
{
	
	vsf_frame_mgr_t *frame_mgr = vsf_createFrameMgr();
	proto_vsf_frame_cfg_t cfg = {.id = 2};

	frame_mgr->get(frame_mgr,&cfg);
	cfg.id = 2;
	cfg.enable = 1;
	cfg.format = VIDEO_FRAME_FORMAT_YUV420P_YVU;
	cfg.width = 640;
	cfg.height = 640;
	cfg.fps = 15;

	frame_mgr->set(frame_mgr,&cfg);
	frame_mgr->destroy(frame_mgr);

	//alg init
	alg_workflow_t *pWorkflow = NULL;
	HASH_FIND_INT( appself.pWorkflows, &type, pWorkflow);
	if(NULL != pWorkflow)
		appself.hface = pWorkflow->create_func(pWorkflow->path);
	return 0;
}
int app_alg_deinit(int type)
{
	//alg deinit
	alg_workflow_t *pWorkflow = NULL;
	HASH_FIND_INT( appself.pWorkflows, &type, pWorkflow);
	if(NULL != pWorkflow)
		pWorkflow->destroy_func(appself.hface);

	appself.hface = NULL;
	return 0;
}

static void __attribute__((constructor(102))) app_alg_constructor()
{
	//Configure the hash table according to the algorithm supported by the device
	
	int i;
	for(i = 0; i < cfg_get_member(alginfo)->num; i++)
	{
		alg_workflow_t *pWorkflow = NULL;
		pWorkflow = malloc(sizeof(alg_workflow_t));
		pWorkflow->type = cfg_get_member(alginfo)->cfgs[i].type;
		// to do this way to bind path is true?
		pWorkflow->path = cfg_get_member(alginfo)->cfgs[i].algpath;
		//to do bind Specific function is nod good
		pWorkflow->create_func = hs_fd_create;
		pWorkflow->destroy_func = hs_fd_destroy;
		pWorkflow->process_func = NULL;
		HASH_ADD_INT(appself.pWorkflows,type,pWorkflow);
	}
	
}

static void __attribute__((destructor(102))) app_alg_destructor()
{
	alg_workflow_t *current_user, *tmp;

    HASH_ITER(hh, appself.pWorkflows, current_user, tmp) {
        HASH_DEL(appself.pWorkflows,current_user);  /* delete it (users advances to next) */
        free(current_user);            /* free it */
    }
	appself.pWorkflows = NULL;
	

}

