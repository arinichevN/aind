#include "aoid.h"

static void channel_serveAoidRequestSelf(void *vself, Aoid *oid, void *vserver, int command){
	Channel *self = (Channel *) vself;
	Acpls *server = (Acpls *) vserver;
	switch(command){
		case CMD_AOID_START:
			channel_start(self);
			acpls_reset(server);
			return;
		case CMD_AOID_STOP:
			channel_stop(self);
			acpls_reset(server);
			return;
		case CMD_AOID_RESET:
			channel_reset(self);
			acpls_reset(server);
			return;
		case CMD_AOID_GET_ACP_COMMAND_SUPPORTED:
			aoidServer_sendSupportedSSR(oid, server);
			return;
	}
	acpls_reset(server);
}

static void channel_serveAoidRequestNone(void *vself, Aoid *oid, void *vserver, int command) {
	Acpls *server = (Acpls *) vserver;
	switch(command){
		case CMD_AOID_GET_ACP_COMMAND_SUPPORTED:
			aoidServer_sendSupportedNone(oid, server);
			return;
	}
	acpls_reset(server);
}

static void channel_serveAoidRequestGGS(void *vself, Aoid *oid, void *vserver, int command, void (*sendRamParam)(Aoid *, Acpls *, Channel *), void (*sendNvramParam)(Aoid *, Acpls *, const ChannelParam *, yn_t), int (*setNvramParam)(Acpls *, ChannelParam *)){
	Channel *self = (Channel *) vself;
	Acpls *server = (Acpls *) vserver;
	switch(command){
		case CMD_AOID_GET_RAM_VALUE:
			sendRamParam(oid, server, self);
			return;
		case CMD_AOID_GET_NVRAM_VALUE:{
				yn_t success = NO;
				ChannelParam param;
				memset(&param, 0, sizeof param);
				if(pmem_getChannelParam(&param, self->ind)){
					success = YES;
				}
				sendNvramParam(oid, server, &param, success);
			}
			return;
		case CMD_AOID_SET_NVRAM_VALUE:{
				ChannelParam param;
				if(!pmem_getChannelParam(&param, self->ind)){
					acpls_reset(server);
					return;
				}
				if(!setNvramParam(server, &param)){
					acpls_reset(server);
					return;
				}
				if(channelParam_check(&param) == ERROR_NO){
					pmem_saveChannelParam(&param, self->ind);
				}
			}
			acpls_reset(server);
			return;
		case CMD_AOID_GET_ACP_COMMAND_SUPPORTED:
			aoidServer_sendSupportedGGS(oid, server);
			return;
	}
	acpls_reset(server);
}

static void channel_sendRamParamId(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel->id, YES);}
static void channel_sendNvramParamId(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->id, success);}
static int channel_setNvramParamId(Acpls *server, ChannelParam *param){int v;	if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->id = v; return 1;}
static void channel_serveAoidRequestId(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamId, channel_sendNvramParamId, channel_setNvramParamId);}

static void channel_sendRamParamEnable(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel->enable, YES);}
static void channel_sendNvramParamEnable(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->enable, success);}
static int channel_setNvramParamEnable(Acpls *server, ChannelParam *param){int v; if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->enable = (yn_t) v; return 1;}
static void channel_serveAoidRequestEnable(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamEnable, channel_sendNvramParamEnable, channel_setNvramParamEnable);}

static void channel_sendRamParamDisplayKind(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel_getDisplayKind(channel), YES);}
static void channel_sendNvramParamDisplayKind(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->display_kind, success);}
static int channel_setNvramParamDisplayKind(Acpls *server, ChannelParam *param){int v; if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->display_kind = (dk_t) v; return 1;}
static void channel_serveAoidRequestDisplayKind(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamDisplayKind, channel_sendNvramParamDisplayKind, channel_setNvramParamDisplayKind);}

static void channel_sendRamParamDisplayP1(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel_getDisplayP1(channel), YES);}
static void channel_sendNvramParamDisplayP1(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->display_p1, success);}
static int channel_setNvramParamDisplayP1(Acpls *server, ChannelParam *param){int v; if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->display_p1 = v; return 1;}
static void channel_serveAoidRequestDisplayP1(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamDisplayP1, channel_sendNvramParamDisplayP1, channel_setNvramParamDisplayP1);}

static void channel_sendRamParamDisplayP2(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel_getDisplayP2(channel), YES);}
static void channel_sendNvramParamDisplayP2(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->display_p2, success);}
static int channel_setNvramParamDisplayP2(Acpls *server, ChannelParam *param){int v; if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->display_p2 = v; return 1;}
static void channel_serveAoidRequestDisplayP2(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamDisplayP2, channel_sendNvramParamDisplayP2, channel_setNvramParamDisplayP2);}

static void channel_sendRamParamDisplayP3(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel_getDisplayP3(channel), YES);}
static void channel_sendNvramParamDisplayP3(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->display_p3, success);}
static int channel_setNvramParamDisplayP3(Acpls *server, ChannelParam *param){int v; if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->display_p3 = v; return 1;}
static void channel_serveAoidRequestDisplayP3(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamDisplayP3, channel_sendNvramParamDisplayP3, channel_setNvramParamDisplayP3);}

static void channel_sendRamParamDisplayTextAlignment(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel->display_text_alignment, YES);}
static void channel_sendNvramParamDisplayTextAlignment(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->display_text_alignment, success);}
static int channel_setNvramParamDisplayTextAlignment(Acpls *server, ChannelParam *param){int v;	if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->display_text_alignment = (talign_t) v; return 1;}
static void channel_serveAoidRequestDisplayTextAlignment(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamDisplayTextAlignment, channel_sendNvramParamDisplayTextAlignment, channel_setNvramParamDisplayTextAlignment);}

static void channel_sendRamParamSerialId(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel->serial_id, YES);}
static void channel_sendNvramParamSerialId(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->serial_id, success);}
static int channel_setNvramParamSerialId(Acpls *server, ChannelParam *param){int v;	if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->serial_id = v; return 1;}
static void channel_serveAoidRequestSerialId(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamSerialId, channel_sendNvramParamSerialId, channel_setNvramParamSerialId);}

static void channel_sendRamParamMode(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel->mode, YES);}
static void channel_sendNvramParamMode(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->mode, success);}
static int channel_setNvramParamMode(Acpls *server, ChannelParam *param){int v;	if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->mode = (channel_mode) v; return 1;}
static void channel_serveAoidRequestMode(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamMode, channel_sendNvramParamMode, channel_setNvramParamMode);}

static void channel_sendRamParamRemoteId(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel->remote_id, YES);}
static void channel_sendNvramParamRemoteId(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->remote_id, success);}
static int channel_setNvramParamRemoteId(Acpls *server, ChannelParam *param){int v;	if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->remote_id = v; return 1;}
static void channel_serveAoidRequestRemoteId(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamRemoteId, channel_sendNvramParamRemoteId, channel_setNvramParamRemoteId);}

static void channel_sendRamParamAcpCommand(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel->acp_command, YES);}
static void channel_sendNvramParamAcpCommand(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->acp_command, success);}
static int channel_setNvramParamAcpCommand(Acpls *server, ChannelParam *param){int v; if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->acp_command = v; return 1;}
static void channel_serveAoidRequestAcpCommand(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamAcpCommand, channel_sendNvramParamAcpCommand, channel_setNvramParamAcpCommand);}

static void channel_sendRamParamTime(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendUlI(oid, server, channel->time, YES);}
static void channel_sendNvramParamTime(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendUlI(oid, server, param->time, success);}
static int channel_setNvramParamTime(Acpls *server, ChannelParam *param){unsigned long v; if(!acp_packGetCellUl(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->time = v; return 1;}
static void channel_serveAoidRequestTime(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamTime, channel_sendNvramParamTime, channel_setNvramParamTime);}

static void channel_sendRamParamFloatPrecision(Aoid *oid, Acpls *server, Channel *channel){aoidServer_sendII(oid, server, channel->float_precision, YES);}
static void channel_sendNvramParamFloatPrecision(Aoid *oid, Acpls *server, const ChannelParam *param, yn_t success){aoidServer_sendII(oid, server, param->float_precision, success);}
static int channel_setNvramParamFloatPrecision(Acpls *server, ChannelParam *param){int v; if(!acp_packGetCellI(server->acpl->buf, AOID_ACP_REQUEST_IND_PARAM1, &v)){return 0;} param->float_precision = v; return 1;}
static void channel_serveAoidRequestFloatPrecision(void *vself, Aoid *oid, void *vserver, int command){channel_serveAoidRequestGGS(vself, oid, vserver, command, channel_sendRamParamFloatPrecision, channel_sendNvramParamFloatPrecision, channel_setNvramParamFloatPrecision);}


void channelAoid_build(ChannelAoid *self, Aoid *next, Aoid *parent, void *vchannel, size_t *id){
	//OBJE_ID_SET_PARAM(OID, 							NEXT,								PARENT,					KIND,							DESCR,								FUNCTION, 										DATA,		ID)
	AOID_SET_PARAM(&self->main,							&self->id,							parent,					AOID_KIND_COMPLEX,				AOID_DESCRIPTION_CHANNEL,			channel_serveAoidRequestSelf,					vchannel,	*id)
	AOID_SET_PARAM(&self->id,							&self->enable,						&self->main,			AOID_KIND_ID_PARAM,				AOID_DESCRIPTION_ID,				channel_serveAoidRequestId,						vchannel,	*id)
	AOID_SET_PARAM(&self->enable,						&self->display_main,				&self->main,			AOID_KIND_YN_PARAM,				AOID_DESCRIPTION_ENABLE,			channel_serveAoidRequestEnable,					vchannel,	*id)
	AOID_SET_PARAM(&self->display_main,					&self->display_kind,				&self->main,			AOID_KIND_COMPLEX,				AOID_DESCRIPTION_DISPLAY,			channel_serveAoidRequestNone,						vchannel,	*id)
	AOID_SET_PARAM(&self->display_kind,					&self->display_p1,					&self->display_main,	AOID_KIND_DEVICE_KIND_PARAM,	AOID_DESCRIPTION_DEVICE_KIND,		channel_serveAoidRequestDisplayKind,			vchannel,	*id)
	AOID_SET_PARAM(&self->display_p1,					&self->display_p2,					&self->display_main,	AOID_KIND_UINT8_PARAM,			AOID_DESCRIPTION_PIN1,				channel_serveAoidRequestDisplayP1,				vchannel,	*id)
	AOID_SET_PARAM(&self->display_p2,					&self->display_p3,					&self->display_main,	AOID_KIND_UINT8_PARAM,			AOID_DESCRIPTION_PIN2,				channel_serveAoidRequestDisplayP2,				vchannel,	*id)
	AOID_SET_PARAM(&self->display_p3,					&self->display_text_alignment,		&self->display_main,	AOID_KIND_UINT8_PARAM,			AOID_DESCRIPTION_PIN3,				channel_serveAoidRequestDisplayP3,				vchannel,	*id)
	AOID_SET_PARAM(&self->display_text_alignment,		&self->serial_id,					&self->display_main,	AOID_KIND_TEXT_ALIGNMENT,		AOID_DESCRIPTION_TEXT_ALIGNMENT,	channel_serveAoidRequestDisplayTextAlignment,	vchannel,	*id)
	AOID_SET_PARAM(&self->serial_id,					&self->mode,						&self->main,			AOID_KIND_UINT8_PARAM,			AOID_DESCRIPTION_SERIAL_ID,			channel_serveAoidRequestSerialId,				vchannel,	*id)
	AOID_SET_PARAM(&self->mode,							&self->remote_id,					&self->main,			AOID_KIND_AIND_MODE_PARAM,		AOID_DESCRIPTION_MODE,				channel_serveAoidRequestMode,					vchannel,	*id)
	AOID_SET_PARAM(&self->remote_id,					&self->acp_command,					&self->main,			AOID_KIND_ID_PARAM,				AOID_DESCRIPTION_REMOTE_ID,			channel_serveAoidRequestRemoteId,				vchannel,	*id)
	AOID_SET_PARAM(&self->acp_command,					&self->time,						&self->main,			AOID_KIND_NOID_COMMAND_PARAM,	AOID_DESCRIPTION_NOID_COMMAND,		channel_serveAoidRequestAcpCommand,				vchannel,	*id)
	AOID_SET_PARAM(&self->time,							&self->float_precision,				&self->main,			AOID_KIND_TIMEMS_PARAM,			AOID_DESCRIPTION_TIMEMS,			channel_serveAoidRequestTime,					vchannel,	*id)
	AOID_SET_PARAM(&self->float_precision,				next,								&self->main,			AOID_KIND_UINT8_PARAM,			AOID_DESCRIPTION_PRECISION,			channel_serveAoidRequestFloatPrecision,			vchannel,	*id)
}
